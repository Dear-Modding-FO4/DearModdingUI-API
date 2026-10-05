#!/usr/bin/env python3

import importlib.util
from pathlib import Path
import subprocess
import tempfile
import unittest


REPOSITORY_ROOT = Path(__file__).resolve().parents[1]
SCRIPT_PATH = REPOSITORY_ROOT / "Tools" / "sync-commonlibf4.py"
SPEC = importlib.util.spec_from_file_location("sync_commonlibf4", SCRIPT_PATH)
SYNC = importlib.util.module_from_spec(SPEC)
assert SPEC.loader is not None
SPEC.loader.exec_module(SYNC)

GITLINK_PATH = "lib/dearmoddingui-api"


def git(
    repository: Path,
    *arguments: str,
    check: bool = True,
) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(
        ["git", "-C", str(repository), *arguments],
        check=False,
        capture_output=True,
        text=True,
    )
    if check and result.returncode != 0:
        raise AssertionError(
            f"git {' '.join(arguments)} failed\n{result.stdout}\n{result.stderr}"
        )
    return result


def configure_author(repository: Path) -> None:
    git(repository, "config", "user.name", "Sync Test")
    git(repository, "config", "user.email", "sync-test@example.invalid")


def git_bare(
    repository: Path,
    *arguments: str,
) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(
        ["git", "--git-dir", str(repository), *arguments],
        check=False,
        capture_output=True,
        text=True,
    )
    if result.returncode != 0:
        raise AssertionError(
            f"git {' '.join(arguments)} failed\n{result.stdout}\n{result.stderr}"
        )
    return result


class SyncCommonlibf4Tests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory(prefix="sync-commonlibf4-tests-")
        self.root = Path(self.temporary.name)
        self.source_remote = self.root / "source.git"
        self.source_author = self.root / "source-author"
        self.source_checkout = self.root / "source-checkout"

        git(self.root, "init", "--bare", "--initial-branch=main", str(self.source_remote))
        git(self.root, "clone", str(self.source_remote), str(self.source_author))
        configure_author(self.source_author)

        self.api_commits = []
        for number in range(1, 4):
            (self.source_author / "api.txt").write_text(
                f"api {number}\n", encoding="utf-8"
            )
            git(self.source_author, "add", "api.txt")
            git(self.source_author, "commit", "-m", f"api {number}")
            self.api_commits.append(
                git(self.source_author, "rev-parse", "HEAD").stdout.strip()
            )
        git(self.source_author, "push", "origin", "main")
        git(self.root, "clone", str(self.source_remote), str(self.source_checkout))

    def tearDown(self) -> None:
        self.temporary.cleanup()

    def create_target(
        self,
        existing: str | None,
    ) -> tuple[Path, Path]:
        target_remote = self.root / f"target-{len(list(self.root.glob('target-*.git')))}.git"
        target_author = self.root / f"{target_remote.stem}-author"
        target_checkout = self.root / f"{target_remote.stem}-checkout"

        git(self.root, "init", "--bare", "--initial-branch=main", str(target_remote))
        git(self.root, "clone", str(target_remote), str(target_author))
        configure_author(target_author)
        (target_author / "README.md").write_text("commonlibf4\n", encoding="utf-8")
        git(target_author, "add", "README.md")

        if existing is not None:
            git(
                target_author,
                "update-index",
                "--add",
                "--cacheinfo",
                f"160000,{existing},{GITLINK_PATH}",
            )

        git(target_author, "commit", "-m", "initial commonlib")
        git(target_author, "push", "origin", "main")
        git(self.root, "clone", str(target_remote), str(target_checkout))
        return target_remote, target_checkout

    def target_pointer(self, target_remote: Path) -> str | None:
        result = git_bare(
            target_remote,
            "ls-tree",
            "refs/heads/main",
            "--",
            GITLINK_PATH,
        ).stdout.strip()
        if not result:
            return None
        return result.split()[2]

    def target_file(self, target_remote: Path, path: str) -> str:
        return git_bare(target_remote, "show", f"refs/heads/main:{path}").stdout

    def concurrent_clone(self, target_remote: Path, name: str) -> Path:
        clone = self.root / name
        git(self.root, "clone", str(target_remote), str(clone))
        configure_author(clone)
        return clone

    def advance_concurrently(
        self,
        clone: Path,
        attempt: int,
        *,
        pointer: str | None = None,
    ) -> None:
        git(clone, "fetch", "origin", "main")
        git(clone, "checkout", "-B", "main", "origin/main")
        path = clone / f"concurrent-{attempt}.txt"
        path.write_text(f"change {attempt}\n", encoding="utf-8")
        git(clone, "add", path.name)
        if pointer is not None:
            git(
                clone,
                "update-index",
                "--cacheinfo",
                f"160000,{pointer},{GITLINK_PATH}",
            )
        git(clone, "commit", "-m", f"concurrent {attempt}")
        git(clone, "push", "origin", "main")

    def run_sync(
        self,
        target_checkout: Path,
        candidate: str,
        **kwargs,
    ) -> tuple[str, list[str]]:
        messages = []
        result = SYNC.sync_commonlibf4(
            self.source_checkout,
            target_checkout,
            candidate,
            log=messages.append,
            **kwargs,
        )
        return result, messages

    def test_forward_advance(self) -> None:
        target_remote, target_checkout = self.create_target(self.api_commits[1])

        result, messages = self.run_sync(target_checkout, self.api_commits[2])

        self.assertEqual("updated", result)
        self.assertEqual(self.api_commits[2], self.target_pointer(target_remote))
        self.assertIn("Success:", messages[-1])

    def test_out_of_order_older_build_skips(self) -> None:
        target_remote, target_checkout = self.create_target(self.api_commits[2])

        result, messages = self.run_sync(target_checkout, self.api_commits[1])

        self.assertEqual("stale", result)
        self.assertEqual(self.api_commits[2], self.target_pointer(target_remote))
        self.assertIn("Stale skip:", messages[-1])

    def test_divergent_history_refuses(self) -> None:
        git(self.source_checkout, "checkout", "--detach", self.api_commits[0])
        (self.source_checkout / "divergent.txt").write_text(
            "divergent\n", encoding="utf-8"
        )
        git(self.source_checkout, "add", "divergent.txt")
        configure_author(self.source_checkout)
        git(self.source_checkout, "commit", "-m", "divergent")
        divergent = git(self.source_checkout, "rev-parse", "HEAD").stdout.strip()
        target_remote, target_checkout = self.create_target(divergent)

        with self.assertRaisesRegex(SYNC.SyncError, "have diverged"):
            self.run_sync(target_checkout, self.api_commits[2])

        self.assertEqual(divergent, self.target_pointer(target_remote))

    def test_concurrent_unrelated_change_survives_retry(self) -> None:
        target_remote, target_checkout = self.create_target(self.api_commits[1])
        concurrent = self.concurrent_clone(target_remote, "concurrent-unrelated")

        def race(attempt: int) -> None:
            if attempt == 1:
                self.advance_concurrently(concurrent, attempt)

        result, messages = self.run_sync(
            target_checkout,
            self.api_commits[2],
            before_push=race,
        )

        self.assertEqual("updated", result)
        self.assertEqual(self.api_commits[2], self.target_pointer(target_remote))
        self.assertEqual("change 1\n", self.target_file(target_remote, "concurrent-1.txt"))
        self.assertTrue(any(message.startswith("Retry:") for message in messages))

    def test_concurrent_newer_pointer_prevents_stale_retry_rollback(self) -> None:
        target_remote, target_checkout = self.create_target(self.api_commits[0])
        concurrent = self.concurrent_clone(target_remote, "concurrent-newer")

        def race(attempt: int) -> None:
            if attempt == 1:
                self.advance_concurrently(
                    concurrent,
                    attempt,
                    pointer=self.api_commits[2],
                )

        result, messages = self.run_sync(
            target_checkout,
            self.api_commits[1],
            before_push=race,
        )

        self.assertEqual("stale", result)
        self.assertEqual(self.api_commits[2], self.target_pointer(target_remote))
        self.assertTrue(any(message.startswith("Retry:") for message in messages))
        self.assertIn("Stale skip:", messages[-1])


if __name__ == "__main__":
    unittest.main()
