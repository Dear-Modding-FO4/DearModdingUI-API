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
        self.api_text = generator.read_api_header(ROOT / "schema/ui-contract.json")
        self.baseline = generator.load_baseline_manifest(
            ROOT / "schema/ui-contract.manifest.json"
        )

    def changed(self, change=lambda schema: None, api_text=None):
        schema = copy.deepcopy(self.schema)
        change(schema)
        return generator.build_manifest(schema, api_text or self.api_text)

    def changed_header(self, old, new):
        self.assertIn(old, self.api_text)
        return self.changed(api_text=self.api_text.replace(old, new, 1))

    def bump(self, manifest, major=0, minor=0):
        contract = manifest["contract"]
        if major:
            contract["abiMajor"] += major
            contract["abiMinor"] = 0
        contract["abiMinor"] += minor
        return manifest

    def test_additions_require_minor_change(self):
        def append_operation(schema):
            schema["signatures"]["Probe"] = [["DMUI_ClientHandle", "client"]]
            schema["operations"].append(
                [len(schema["operations"]) + 1, "Probe", "probe", "void"]
            )

        additions = {
            "append_operation": self.changed(append_operation),
            "add_struct": self.changed_header(
                "typedef struct DMUI_HostAPI\n",
                "typedef struct DMUI_Probe\n{\n\tuint32_t value;\n} DMUI_Probe;\n\n"
                "typedef struct DMUI_HostAPI\n",
            ),
        }
        for name, manifest in additions.items():
            with self.subTest(change=name):
                with self.assertRaisesRegex(generator.GenerationError, "DMUI_ABI_MINOR"):
                    generator.validate_compatibility(manifest, self.baseline)
                if name == "append_operation":
                    with self.assertRaisesRegex(generator.GenerationError, "introducing ABI minor"):
                        generator.validate_compatibility(
                            self.bump(copy.deepcopy(manifest), minor=1), self.baseline
                        )
                    manifest["operations"][-1]["sinceMinor"] = self.baseline["contract"]["abiMinor"] + 1
                generator.validate_compatibility(self.bump(manifest, minor=1), self.baseline)

    def test_breaking_changes_require_major_change(self):
        def reorder_operations(schema):
            operations = schema["operations"]
            operations[0][1:], operations[1][1:] = operations[1][1:], operations[0][1:]

        breaking = {
            "reorder_operations": self.changed(reorder_operations),
            "change_slot_minor": self.changed(
                lambda schema: schema["operationMinors"].update({"1": 97})
            ),
        }
        for name, old, new in (
            (
                "grow_struct",
                "\tconst char* bridgeSourceLabel;\n",
                "\tconst char* bridgeSourceLabel;\n\tuint32_t probe;\n",
            ),
            (
                "change_define",
                "#define DMUI_RESULT_INVALID_ARGUMENT 2u",
                "#define DMUI_RESULT_INVALID_ARGUMENT 3u",
            ),
            ("remove_host_slot", "\tDMUI_RegisterPageFn registerPage;\n", ""),
        ):
            breaking[name] = self.changed_header(old, new)
        for name, manifest in breaking.items():
            with self.subTest(change=name):
                manifest = self.bump(manifest, minor=1)
                with self.assertRaisesRegex(generator.GenerationError, "DMUI_ABI_MAJOR"):
                    generator.validate_compatibility(manifest, self.baseline)
                generator.validate_compatibility(self.bump(manifest, major=1), self.baseline)


if __name__ == "__main__":
    unittest.main()
