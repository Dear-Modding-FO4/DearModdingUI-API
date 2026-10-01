import copy
import importlib.util
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "generator", ROOT / "Tools/generate-ui-contract.py"
)
generator = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(generator)


class ABIContractTests(unittest.TestCase):
    def setUp(self):
        self.schema = generator.load_schema(ROOT / "schema/ui-contract.json")
        self.baseline = generator.load_baseline_manifest(
            ROOT / "schema/ui-contract.manifest.json"
        )

    def test_published_contract_matches(self):
        generator.validate_compatibility(self.schema, self.baseline)

    def test_layout_changes_require_abi_change(self):
        signature = copy.deepcopy(self.schema)
        signature["signatures"]["Button"][1][0] = "uint64_t"
        reordered = copy.deepcopy(self.schema)
        reordered["operations"][0], reordered["operations"][1] = (
            reordered["operations"][1], reordered["operations"][0]
        )
        appended = copy.deepcopy(self.schema)
        appended["signatures"]["Probe"] = [["DMUI_ClientHandle", "client"]]
        appended["operations"].append(
            [len(appended["operations"]) + 1, "Probe", "probe", "void"]
        )
        removed = copy.deepcopy(self.schema)
        removed["operations"].pop()
        for changed in (signature, reordered, appended, removed):
            with self.subTest(operations=len(changed["operations"])):
                with self.assertRaisesRegex(generator.GenerationError, "DMUI_ABI_VERSION"):
                    generator.validate_compatibility(changed, self.baseline)
                changed["contract"]["abi"] += 1
                generator.validate_compatibility(changed, self.baseline)

    def test_enum_changes_require_abi_change(self):
        self.schema["enums"][0]["values"][0][1] += 1
        with self.assertRaisesRegex(generator.GenerationError, "DMUI_ABI_VERSION"):
            generator.validate_compatibility(self.schema, self.baseline)


if __name__ == "__main__":
    unittest.main()
