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
        rendered, excluded = generator.render(
            self.catalog, self.metadata, self.domains
        )
        self.assertEqual([], excluded)
        self.assertEqual(OUTPUT.read_text(encoding="utf-8"), rendered)

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
