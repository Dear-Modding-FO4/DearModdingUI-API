import argparse
import json
import os
import tempfile
from pathlib import Path


class GenerationError(RuntimeError):
    pass


def load_schema(path: Path) -> dict:
    try:
        schema = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as error:
        raise GenerationError(f"cannot read {path}: {error}") from error
    required = {"contract", "enums", "signatures", "operations"}
    if not isinstance(schema, dict) or set(schema) != required:
        raise GenerationError(f"schema must contain exactly {sorted(required)}")
    operations = schema["operations"]
    if [operation[0] for operation in operations] != list(
        range(1, len(operations) + 1)
    ):
        raise GenerationError("operation IDs must be contiguous")
    names = [operation[1] for operation in operations]
    if len(names) != len(set(names)) or set(names) != set(schema["signatures"]):
        raise GenerationError("operation names and signatures must match exactly")
    return schema


def atomic_write(path: Path, text: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    descriptor, temporary_name = tempfile.mkstemp(
        prefix=f".{path.name}.", suffix=".tmp", dir=path.parent, text=True
    )
    temporary = Path(temporary_name)
    try:
        with os.fdopen(descriptor, "w", encoding="utf-8", newline="\n") as output:
            output.write(text)
            output.flush()
            os.fsync(output.fileno())
        os.replace(temporary, path)
    finally:
        if temporary.exists():
            temporary.unlink()


def upper_snake(name: str) -> str:
    result = []
    for index, character in enumerate(name):
        if character.isupper() and index and (
            not name[index - 1].isupper()
            or (index + 1 < len(name) and name[index + 1].islower())
        ):
            result.append("_")
        result.append(character.upper())
    return "".join(result)


def c_macro(c_name: str) -> str:
    if c_name.startswith("DMUI_UI"):
        return "DMUI_UI_" + upper_snake(c_name[len("DMUI_UI") :])
    if c_name.startswith("DMUI_"):
        return "DMUI_" + upper_snake(c_name[len("DMUI_") :])
    raise GenerationError(f"UI C type does not use the DMUI prefix: {c_name}")


def declaration(type_name: str, name: str) -> str:
    return f"{type_name} {name}"


def render_c_header(schema: dict) -> str:
    operations = schema["operations"]
    signatures = schema["signatures"]
    lines = [
        "#pragma once",
        "",
        "// Generated from schema/ui-contract.json; do not edit.",
        "",
        "#include <DearModdingUI/API.h>",
        "",
    ]
    for enum in schema["enums"]:
        c_name = enum["cName"]
        lines.append(f"typedef uint32_t {c_name};")
        lines.append(f"#define {c_macro(c_name)}_NONE 0u")
        for name, value, _ in enum["values"]:
            lines.append(
                f"#define {c_macro(c_name)}_{upper_snake(name)} {value}u"
            )
        for name, value in enum.get("aliases", []):
            lines.append(
                f"#define {c_macro(c_name)}_{upper_snake(name)} {value}u"
            )
        if enum.get("rejectedMask"):
            lines.append(
                f"#define {c_macro(c_name)}_REJECTED_CALLBACK_MASK "
                f"{enum['rejectedMask']}u"
            )
        lines.append("")

    for _, name, _, _ in operations:
        parameters = ",\n\t".join(
            declaration(type_name, argument_name)
            for type_name, argument_name in signatures[name]
        )
        lines.extend(
            [
                f"typedef DMUI_Result (DMUI_CALL *DMUI_UI{name}Fn)(",
                f"\t{parameters}) DMUI_NOEXCEPT;",
            ]
        )
    lines.extend(
        [
            "",
            "typedef struct DMUI_UIAPI",
            "{",
        ]
    )
    for _, name, field, _ in operations:
        lines.append(f"\tDMUI_UI{name}Fn {field};")
    lines.extend(["} DMUI_UIAPI;", ""])
    return "\n".join(lines)


def render_checked_header(schema: dict) -> str:
    lines = [
        "#pragma once",
        "",
        "// Generated from schema/ui-contract.json; do not edit.",
        "// Included by DearModdingUI/UI.h after its context implementation.",
        "",
        "namespace dmui::ui",
        "{",
        "\tusing Vec2 = DMUI_Vec2;",
        "\tusing Vec4 = DMUI_Vec4;",
        "\tusing Color32 = uint32_t;",
        "\tusing ID = uint32_t;",
        "",
    ]
    for enum in schema["enums"]:
        name = enum["name"]
        c_name = enum["cName"]
        lines.extend(
            [
                f"\tenum class {name} : uint32_t",
                "\t{",
                "\t\tkNone = 0u,",
            ]
        )
        values = [
            (value_name, f"{c_macro(c_name)}_{upper_snake(value_name)}")
            for value_name, _, _ in enum["values"]
        ] + [
            (value_name, f"{c_macro(c_name)}_{upper_snake(value_name)}")
            for value_name, _ in enum.get("aliases", [])
        ]
        for index, (value_name, value) in enumerate(values):
            comma = "," if index + 1 < len(values) else ""
            lines.append(f"\t\tk{value_name} = {value}{comma}")
        lines.extend(["\t};", ""])
        if enum["kind"] == "flags":
            lines.extend(
                [
                    f"\t[[nodiscard]] constexpr {name} operator|(",
                    f"\t\t{name} a_left, {name} a_right) noexcept",
                    "\t{",
                    f"\t\treturn static_cast<{name}>(",
                    "\t\t\tstatic_cast<uint32_t>(a_left) |",
                    "\t\t\tstatic_cast<uint32_t>(a_right));",
                    "\t}",
                    "",
                    f"\t[[nodiscard]] constexpr {name} operator&(",
                    f"\t\t{name} a_left, {name} a_right) noexcept",
                    "\t{",
                    f"\t\treturn static_cast<{name}>(",
                    "\t\t\tstatic_cast<uint32_t>(a_left) &",
                    "\t\t\tstatic_cast<uint32_t>(a_right));",
                    "\t}",
                    "",
                    f"\tconstexpr {name}& operator|=(",
                    f"\t\t{name}& a_left, {name} a_right) noexcept",
                    "\t{",
                    "\t\ta_left = a_left | a_right;",
                    "\t\treturn a_left;",
                    "\t}",
                    "",
                ]
            )
        if enum.get("rejectedMask"):
            lines.append(
                f"\tinline constexpr uint32_t k{name}RejectedCallbackMask{{ "
                f"{c_macro(c_name)}_REJECTED_CALLBACK_MASK }};"
            )
            lines.append("")
    lines.extend(["\tnamespace checked", "\t{"])
    for _, name, field, _ in schema["operations"]:
        parameters = schema["signatures"][name][1:]
        rendered = ",\n\t\t".join(
            declaration(type_name, argument_name)
            for type_name, argument_name in parameters
        )
        arguments = ", ".join(argument_name for _, argument_name in parameters)
        comma = ", " if arguments else ""
        lines.extend(
            [
                f"\t\t[[nodiscard]] inline DMUI_Result {name}(",
                f"\t\t\t{rendered}) noexcept" if rendered else "\t\t\tvoid) noexcept",
                "\t\t{",
                f"\t\t\treturn detail::Invoke(",
                f"\t\t\t\t&DMUI_UIAPI::{field}{comma}{arguments});",
                "\t\t}",
                "",
            ]
        )
    if lines[-1] == "":
        lines.pop()
    lines.extend(["\t}", "}", ""])
    return "\n".join(lines)


def native_type(enum_name: str) -> str:
    return {
        "WindowFlags": "ImGuiWindowFlags",
        "Color": "ImGuiCol",
        "StyleVar": "ImGuiStyleVar",
        "DataType": "ImGuiDataType",
        "ComboFlags": "ImGuiComboFlags",
        "HoveredFlags": "ImGuiHoveredFlags",
        "InputTextFlags": "ImGuiInputTextFlags",
        "SelectableFlags": "ImGuiSelectableFlags",
        "SliderFlags": "ImGuiSliderFlags",
        "TableFlags": "ImGuiTableFlags",
        "TableColumnFlags": "ImGuiTableColumnFlags",
        "TableRowFlags": "ImGuiTableRowFlags",
        "TreeNodeFlags": "ImGuiTreeNodeFlags",
    }[enum_name]


def render_host_bindings(schema: dict) -> str:
    lines = [
        "#pragma once",
        "",
        "// Generated from schema/ui-contract.json; do not edit.",
        "",
        "#include <DearModdingUI/CUIAPI.h>",
        "",
        "#include <imgui/imgui.h>",
        "",
        "namespace DearModdingUI::UI::Bindings",
        "{",
    ]
    for enum in schema["enums"]:
        name = enum["name"]
        c_name = enum["cName"]
        if enum.get("hostTranslated") is False:
            continue
        native = native_type(name)
        if enum["kind"] == "value":
            lines.extend(
                [
                    f"\t[[nodiscard]] inline DMUI_Result Translate{name}(",
                    f"\t\t{c_name} a_value,",
                    f"\t\t{native}& a_native) noexcept",
                    "\t{",
                    "\t\tswitch (a_value)",
                    "\t\t{",
                ]
            )
            for value_name, _, native_name in enum["values"]:
                lines.extend(
                    [
                        f"\t\tcase {c_macro(c_name)}_{upper_snake(value_name)}:",
                        f"\t\t\ta_native = {native_name};",
                        "\t\t\treturn DMUI_RESULT_OK;",
                    ]
                )
            lines.extend(
                [
                    "\t\tdefault:",
                    "\t\t\treturn DMUI_RESULT_INVALID_ARGUMENT;",
                    "\t\t}",
                    "\t}",
                    "",
                ]
            )
        else:
            known = " | ".join(
                f"{c_macro(c_name)}_{upper_snake(value_name)}"
                for value_name, _, _ in enum["values"]
            )
            lines.extend(
                [
                    f"\t[[nodiscard]] inline DMUI_Result Translate{name}(",
                    f"\t\t{c_name} a_value,",
                    f"\t\t{native}& a_native) noexcept",
                    "\t{",
                    f"\t\tconstexpr uint32_t known{{ {known} }};",
                    "\t\tif ((a_value & ~known) != 0)",
                    "\t\t\treturn DMUI_RESULT_INVALID_ARGUMENT;",
                ]
            )
            for group in enum.get("exclusive", []):
                mask = " | ".join(
                    f"{c_macro(c_name)}_{upper_snake(value_name)}"
                    for value_name in group
                )
                lines.extend(
                    [
                        f"\t\tconstexpr uint32_t {group[0][0].lower() + group[0][1:]}Mask{{ {mask} }};",
                        f"\t\tconst auto {group[0][0].lower() + group[0][1:]}Bits = "
                        f"a_value & {group[0][0].lower() + group[0][1:]}Mask;",
                        f"\t\tif ({group[0][0].lower() + group[0][1:]}Bits != 0 && "
                        f"({group[0][0].lower() + group[0][1:]}Bits & "
                        f"({group[0][0].lower() + group[0][1:]}Bits - 1u)) != 0)",
                        "\t\t\treturn DMUI_RESULT_INVALID_ARGUMENT;",
                    ]
                )
            lines.append("\t\ta_native = 0;")
            for value_name, _, native_name in enum["values"]:
                lines.extend(
                    [
                        f"\t\tif ((a_value & {c_macro(c_name)}_{upper_snake(value_name)}) != 0)",
                        f"\t\t\ta_native |= {native_name};",
                    ]
                )
            lines.extend(["\t\treturn DMUI_RESULT_OK;", "\t}", ""])

    for _, name, _, _ in schema["operations"]:
        parameters = ",\n\t\t".join(
            declaration(type_name, f"a_{argument_name}")
            for type_name, argument_name in schema["signatures"][name]
        )
        lines.extend(
            [
                f"\t[[nodiscard]] DMUI_Result DMUI_CALL {name}(",
                f"\t\t{parameters}) noexcept;",
            ]
        )
    lines.extend(
        [
            "",
            "\t[[nodiscard]] inline DMUI_UIAPI MakeAPI() noexcept",
            "\t{",
            "\t\treturn {",
        ]
    )
    for index, (_, name, _, _) in enumerate(schema["operations"]):
        comma = "," if index + 1 < len(schema["operations"]) else ""
        lines.append(f"\t\t\t&{name}{comma}")
    lines.extend(["\t\t};", "\t}", "}", ""])
    return "\n".join(lines)


def build_manifest(schema: dict) -> dict:
    return {
        "contract": schema["contract"],
        "enums": [
            {
                "name": enum["name"],
                "cName": enum["cName"],
                "kind": enum["kind"],
                "values": [
                    {"name": name, "value": value}
                    for name, value, _ in enum["values"]
                ],
                "aliases": [
                    {"name": name, "value": value}
                    for name, value in enum.get("aliases", [])
                ],
                "rejectedMask": enum.get("rejectedMask", 0),
            }
            for enum in schema["enums"]
        ],
        "operations": [
            {
                "id": operation_id,
                "name": name,
                "field": field,
                "kind": kind,
                "signature": schema["signatures"][name],
            }
            for operation_id, name, field, kind in schema["operations"]
        ],
    }


def load_baseline_manifest(path: Path) -> dict:
    try:
        manifest = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as error:
        raise GenerationError(f"cannot read compatibility baseline {path}: {error}") from error
    required = {"contract", "enums", "operations"}
    if not isinstance(manifest, dict) or set(manifest) != required:
        raise GenerationError(
            f"compatibility baseline must contain exactly {sorted(required)}"
        )
    return manifest


def validate_compatibility(schema: dict, baseline: dict) -> None:
    current = build_manifest(schema)
    if current["contract"]["abi"] == baseline["contract"]["abi"] and current != baseline:
        raise GenerationError("UI contract changed without a DMUI_ABI_VERSION change")


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate the stable DearModdingUI UI ABI."
    )
    parser.add_argument("--schema", required=True, type=Path)
    parser.add_argument("--c-header", required=True, type=Path)
    parser.add_argument("--checked-header", required=True, type=Path)
    parser.add_argument("--host-bindings", required=True, type=Path)
    parser.add_argument("--baseline-manifest", required=True, type=Path)
    parser.add_argument(
        "--update-baseline",
        action="store_true",
        help="record the validated schema as the published baseline",
    )
    return parser.parse_args()


def main() -> int:
    arguments = parse_arguments()
    schema = load_schema(arguments.schema.resolve())
    baseline = load_baseline_manifest(arguments.baseline_manifest.resolve())
    if not arguments.update_baseline:
        validate_compatibility(schema, baseline)
    api_header = arguments.schema.resolve().parents[1] / "include/DearModdingUI/API.h"
    if f"#define DMUI_ABI_VERSION {schema['contract']['abi']}u" not in api_header.read_text(encoding="utf-8"):
        raise GenerationError("schema ABI must match DMUI_ABI_VERSION in API.h")
    outputs = {
        arguments.c_header.resolve(): render_c_header(schema),
        arguments.checked_header.resolve(): render_checked_header(schema),
        arguments.host_bindings.resolve(): render_host_bindings(schema),
    }
    if arguments.update_baseline:
        outputs[arguments.baseline_manifest.resolve()] = (
            json.dumps(build_manifest(schema), indent=2) + "\n"
        )
    for path, text in outputs.items():
        atomic_write(path, text)
        print(f"Wrote {path} ({len(text.encode('utf-8'))} bytes).")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except GenerationError as error:
        print(f"error: {error}")
        raise SystemExit(1)
