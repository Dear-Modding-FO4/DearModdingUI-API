import copy
import importlib.util
import json
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
GENERATOR = ROOT / "Tools" / "GeneratePhosphorGlyphs.py"
CATALOG = ROOT / "data/phosphor-icons/PhosphorFillCatalog.json"
METADATA = ROOT / "data/phosphor-icons/PhosphorMetadata.json"
DOMAINS = ROOT / "data/phosphor-icons/DomainIconPreferences.json"
OUTPUT = ROOT / "include/DearModdingUI/PhosphorGlyphs.generated.h"

SPEC = importlib.util.spec_from_file_location("phosphor_generator", GENERATOR)
assert SPEC and SPEC.loader
generator = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(generator)


class PhosphorGeneratorTests(unittest.TestCase):
    def setUp(self) -> None:
        self.catalog = json.loads(CATALOG.read_text(encoding="utf-8"))
        self.metadata = json.loads(METADATA.read_text(encoding="utf-8"))
        self.domains = json.loads(DOMAINS.read_text(encoding="utf-8"))

    def test_checked_in_output_is_current(self) -> None:
        generator.validate_pinned_source(
            self.catalog, generator.EXPECTED_CATALOG_SHA256
        )
        generator.validate_pinned_source(
            self.metadata, generator.EXPECTED_METADATA_SHA256
        )
        rendered, excluded = generator.render(
            self.catalog, self.metadata, self.domains
        )
        self.assertEqual([], excluded)
        self.assertEqual(OUTPUT.read_text(encoding="utf-8"), rendered)

    def test_changed_upstream_inputs_are_rejected(self) -> None:
        changed_catalog = copy.deepcopy(self.catalog)
        changed_catalog["glyphs"][0]["codepoint"] += 1
        changed_metadata = copy.deepcopy(self.metadata)
        changed_metadata["icons"][0]["tags"].append("unreviewed")
        for source, checksum in (
            (changed_catalog, generator.EXPECTED_CATALOG_SHA256),
            (changed_metadata, generator.EXPECTED_METADATA_SHA256),
        ):
            with self.assertRaisesRegex(
                generator.GenerationError, "input checksum mismatch"
            ):
                generator.validate_pinned_source(source, checksum)

    def test_unexpected_codepoint_difference_is_rejected(self) -> None:
        self.metadata["icons"][0]["codepoint"] += 1
        with self.assertRaisesRegex(
            generator.GenerationError, "codepoint mismatch set changed"
        ):
            generator.build_vocabulary(self.catalog, self.metadata, self.domains)

    def test_normalization_matches_runtime_contract(self) -> None:
        for source, expected in generator.NORMALIZATION_EXAMPLES.items():
            self.assertEqual(expected, generator.normalize(source))

    def test_normalized_canonical_collisions_are_rejected(self) -> None:
        changed = copy.deepcopy(self.catalog)
        changed["glyphs"][1]["name"] = changed["glyphs"][0]["name"].replace(
            "-", "_"
        )
        changed["glyphs"][1]["codepoint"] += 1
        with self.assertRaisesRegex(
            generator.GenerationError, "normalized canonical name collision"
        ):
            generator.build_vocabulary(changed, self.metadata, self.domains)


if __name__ == "__main__":
    unittest.main()
