import copy
import importlib.util
import json
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
GENERATOR = ROOT / "Tools" / "generate-ui-contract.py"
SCHEMA = ROOT / "schema" / "ui-contract.json"
BASELINE = ROOT / "schema" / "ui-contract.manifest.json"

SPEC = importlib.util.spec_from_file_location("dmui_ui_contract_generator", GENERATOR)
assert SPEC and SPEC.loader
generator = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(generator)


class CompatibilityBaselineTests(unittest.TestCase):
    def setUp(self) -> None:
        self.schema = json.loads(SCHEMA.read_text(encoding="utf-8"))
        self.baseline = json.loads(BASELINE.read_text(encoding="utf-8"))

    def validate(self, schema: dict) -> None:
        generator.validate_compatibility(schema, self.baseline)

    def test_current_schema_matches_published_baseline(self) -> None:
        self.validate(self.schema)

    def test_published_signature_cannot_change(self) -> None:
        changed = copy.deepcopy(self.schema)
        changed["signatures"]["Button"][1][0] = "uint64_t"
        with self.assertRaisesRegex(
            generator.GenerationError, "published UI operation"
        ):
            self.validate(changed)

    def test_published_slot_cannot_be_reordered(self) -> None:
        changed = copy.deepcopy(self.schema)
        changed["operations"][0], changed["operations"][1] = (
            changed["operations"][1],
            changed["operations"][0],
        )
        with self.assertRaisesRegex(
            generator.GenerationError, "published UI operation"
        ):
            self.validate(changed)

    def test_published_enum_value_cannot_change(self) -> None:
        changed = copy.deepcopy(self.schema)
        changed["enums"][0]["values"][0][1] += 1
        with self.assertRaisesRegex(generator.GenerationError, "enum values"):
            self.validate(changed)

    def test_append_requires_new_revision(self) -> None:
        changed = copy.deepcopy(self.schema)
        changed["signatures"]["OptionalProbe"] = [
            ["DMUI_ClientHandle", "client"]
        ]
        changed["operations"].append(
            [64, "OptionalProbe", "optionalProbe", "void", False]
        )
        with self.assertRaisesRegex(generator.GenerationError, "newer contract revision"):
            self.validate(changed)

        changed["contract"]["revision"] += 1
        self.validate(changed)

    def test_appended_optional_slot_is_checked_by_requested_size(self) -> None:
        changed = copy.deepcopy(self.schema)
        changed["contract"]["revision"] += 1
        changed["signatures"]["OptionalProbe"] = [
            ["DMUI_ClientHandle", "client"]
        ]
        changed["operations"].append(
            [64, "OptionalProbe", "optionalProbe", "void", False]
        )

        header = generator.render_checked_header(changed)
        self.assertIn("HasOperationsThroughSize", header)
        self.assertIn(
            "(a_minimumSize < DMUI_UI_API_OPTIONAL_PROBE_SIZE || "
            "a_api->optionalProbe)",
            header,
        )

    def test_revision_cannot_change_without_addition(self) -> None:
        changed = copy.deepcopy(self.schema)
        changed["contract"]["revision"] += 1
        with self.assertRaisesRegex(
            generator.GenerationError, "revision changed without"
        ):
            self.validate(changed)


if __name__ == "__main__":
    unittest.main()
