#!/usr/bin/env python3

import argparse
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile
from typing import Callable


API_BRANCH = "main"
COMMONLIB_BRANCH = "main"
COMMONLIB_PATH = "lib/dearmoddingui-api"
MAX_PUSH_ATTEMPTS = 3
BOT_NAME = "github-actions[bot]"
BOT_EMAIL = "41898282+github-actions[bot]@users.noreply.github.com"


class SyncError(RuntimeError):
    pass


def run_git(
    repository: Path,
    *arguments: str,
    environment: dict[str, str] | None = None,
    check: bool = True,
) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(
        ["git", "-C", str(repository), *arguments],
        check=False,
        capture_output=True,
        text=True,
        env=environment,
    )
    if check and result.returncode != 0:
        details = "\n".join(
            part.strip() for part in (result.stdout, result.stderr) if part.strip()
        )
        suffix = f"\n{details}" if details else ""
        raise SyncError(
            f"git {' '.join(arguments)} failed with exit code {result.returncode}{suffix}"
        )
    return result


def fetch_branch(repository: Path, branch: str) -> str:
    remote_ref = f"refs/remotes/origin/{branch}"
    run_git(
        repository,
        "fetch",
        "--no-tags",
        "origin",
        f"+refs/heads/{branch}:{remote_ref}",
    )
    return run_git(
        repository, "rev-parse", "--verify", f"{remote_ref}^{{commit}}"
    ).stdout.strip()


def require_full_commit(repository: Path, candidate: str) -> str:
    if not re.fullmatch(r"[0-9a-fA-F]{40}", candidate):
        raise SyncError(f"candidate must be a full 40-character Git commit SHA: {candidate}")
    resolved = run_git(
        repository,
        "rev-parse",
        "--verify",
        f"{candidate}^{{commit}}",
    ).stdout.strip()
    if resolved.lower() != candidate.lower():
        raise SyncError(f"candidate did not resolve exactly to itself: {candidate}")
    return resolved


def is_ancestor(repository: Path, older: str, newer: str) -> bool:
    result = run_git(
        repository,
        "merge-base",
        "--is-ancestor",
        older,
        newer,
        check=False,
    )
    if result.returncode == 0:
        return True
    if result.returncode == 1:
        return False
    details = result.stderr.strip() or result.stdout.strip()
    raise SyncError(
        f"cannot determine API ancestry between {older} and {newer}"
        + (f": {details}" if details else "")
    )


def read_gitlink(repository: Path, revision: str) -> str:
    result = run_git(
        repository,
        "ls-tree",
        "-z",
        revision,
        "--",
        COMMONLIB_PATH,
    ).stdout
    if not result:
        raise SyncError(
            f"{COMMONLIB_PATH} is missing from commonlibf4 {COMMONLIB_BRANCH}"
        )

    entries = [entry for entry in result.split("\0") if entry]
    if len(entries) != 1 or "\t" not in entries[0]:
        raise SyncError(f"cannot read a unique {COMMONLIB_PATH} entry")

    metadata, path = entries[0].split("\t", 1)
    fields = metadata.split()
    if path != COMMONLIB_PATH or len(fields) != 3:
        raise SyncError(f"cannot parse the {COMMONLIB_PATH} entry")

    mode, object_type, object_id = fields
    if mode != "160000" or object_type != "commit":
        raise SyncError(
            f"{COMMONLIB_PATH} must already be a gitlink, found {mode} {object_type}"
        )
    return object_id


def classify(existing: str, candidate: str, source: Path) -> str:
    if existing == candidate:
        return "same"
    if is_ancestor(source, existing, candidate):
        return "advance"
    if is_ancestor(source, candidate, existing):
        return "stale"
    raise SyncError(
        f"existing API pointer {existing} and candidate {candidate} have diverged"
    )


def create_update_commit(target: Path, base: str, candidate: str) -> str:
    with tempfile.TemporaryDirectory(prefix="sync-commonlibf4-") as temporary:
        index_path = Path(temporary) / "index"
        environment = os.environ.copy()
        environment["GIT_INDEX_FILE"] = str(index_path)
        environment["GIT_AUTHOR_NAME"] = BOT_NAME
        environment["GIT_AUTHOR_EMAIL"] = BOT_EMAIL
        environment["GIT_COMMITTER_NAME"] = BOT_NAME
        environment["GIT_COMMITTER_EMAIL"] = BOT_EMAIL

        run_git(target, "read-tree", base, environment=environment)
        run_git(
            target,
            "update-index",
            "--cacheinfo",
            f"160000,{candidate},{COMMONLIB_PATH}",
            environment=environment,
        )
        tree = run_git(target, "write-tree", environment=environment).stdout.strip()
        return run_git(
            target,
            "commit-tree",
            tree,
            "-p",
            base,
            "-m",
            "chore(api): advance dearmoddingui-api pointer",
            environment=environment,
        ).stdout.strip()


def is_concurrent_update_rejection(result: subprocess.CompletedProcess[str]) -> bool:
    target_ref = f"refs/heads/{COMMONLIB_BRANCH}"
    for line in result.stdout.splitlines():
        fields = line.split("\t")
        if len(fields) < 3 or fields[0].strip() != "!":
            continue
        if fields[1].rsplit(":", 1)[-1] != target_ref:
            continue
        summary = fields[2].lower()
        if "[rejected]" in summary and (
            "(non-fast-forward)" in summary or "(fetch first)" in summary
        ):
            return True
        if "[remote rejected]" in summary:
            if "(incorrect old value provided)" in summary:
                return True
            mismatch = (
                rf"cannot lock ref '{re.escape(target_ref)}': "
                r"is at [0-9a-f]{40} but expected [0-9a-f]{40}"
            )
            if re.search(mismatch, result.stdout + "\n" + result.stderr):
                return True
    return False


def push_failure_details(result: subprocess.CompletedProcess[str]) -> str:
    details = "\n".join(
        part.strip() for part in (result.stdout, result.stderr) if part.strip()
    )
    return details or f"git push exited with code {result.returncode}"


def sync_commonlibf4(
    source: Path,
    target: Path,
    candidate: str,
    *,
    before_push: Callable[[int], None] | None = None,
    log: Callable[[str], None] = print,
) -> str:
    source = source.resolve()
    target = target.resolve()
    candidate = require_full_commit(source, candidate)

    source_main = fetch_branch(source, API_BRANCH)
    if not is_ancestor(source, candidate, source_main):
        raise SyncError(
            f"candidate {candidate} is not in the API {API_BRANCH} history"
        )

    for attempt in range(1, MAX_PUSH_ATTEMPTS + 1):
        base = fetch_branch(target, COMMONLIB_BRANCH)
        existing = read_gitlink(target, base)
        relation = classify(existing, candidate, source)

        if relation == "same":
            log(f"No-op: commonlibf4 already points to API {candidate}.")
            return "no-op"
        if relation == "stale":
            log(
                f"Stale skip: commonlibf4 points to newer API {existing}; "
                f"candidate {candidate} will not replace it."
            )
            return "stale"

        update_commit = create_update_commit(target, base, candidate)
        if before_push is not None:
            before_push(attempt)
        result = run_git(
            target,
            "push",
            "--porcelain",
            "origin",
            f"{update_commit}:refs/heads/{COMMONLIB_BRANCH}",
            check=False,
        )
        if result.returncode == 0:
            log(
                f"Success: advanced commonlibf4 from API {existing} to {candidate} "
                f"on push attempt {attempt}."
            )
            return "updated"

        details = push_failure_details(result)
        if not is_concurrent_update_rejection(result):
            raise SyncError(f"commonlibf4 push failed without a retryable race:\n{details}")

        refreshed_base = fetch_branch(target, COMMONLIB_BRANCH)
        if refreshed_base == base:
            raise SyncError(
                "commonlibf4 push reported a concurrent-update rejection, but "
                f"{COMMONLIB_BRANCH} did not change:\n{details}"
            )
        if attempt == MAX_PUSH_ATTEMPTS:
            raise SyncError(
                f"commonlibf4 changed during all {MAX_PUSH_ATTEMPTS} push attempts; "
                f"giving up after the bounded retry limit:\n{details}"
            )

        log(
            f"Retry: commonlibf4 {COMMONLIB_BRANCH} changed from {base} to "
            f"{refreshed_base} during push attempt {attempt}."
        )

    raise AssertionError("unreachable")


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Advance commonlibf4 to an exact, successfully built API commit."
    )
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--target", type=Path, required=True)
    parser.add_argument("--candidate", required=True)
    return parser.parse_args()


def main() -> int:
    arguments = parse_arguments()
    try:
        sync_commonlibf4(arguments.source, arguments.target, arguments.candidate)
    except SyncError as error:
        print(f"error: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
