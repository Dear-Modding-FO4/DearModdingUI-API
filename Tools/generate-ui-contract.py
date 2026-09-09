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
        raise GenerationError("operation IDs must be contiguous and immutable")
    names = [operation[1] for operation in operations]
    if len(names) != len(set(names)) or set(names) != set(schema["signatures"]):
        raise GenerationError("operation names and signatures must match exactly")
    optional_seen = False
    for operation in operations:
        optional_seen = optional_seen or not operation[4]
        if optional_seen and operation[4]:
            raise GenerationError("required operations must precede optional operations")
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
    if not c_name.startswith("DMUI_UI"):
        raise GenerationError(f"UI C type does not use the DMUI_UI prefix: {c_name}")
    return "DMUI_UI_" + upper_snake(c_name[len("DMUI_UI") :])


def declaration(type_name: str, name: str) -> str:
    return f"{type_name} {name}"


def render_c_header(schema: dict) -> str:
    contract = schema["contract"]
    operations = schema["operations"]
    signatures = schema["signatures"]
    lines = [
        "#pragma once",
        "",
        "// Generated from schema/ui-contract.json; do not edit.",
        "",
        "#include <DearModdingUI/API.h>",
        "",
        f"#define DMUI_UI_ABI_{contract['abi']} {contract['abi']}u",
        f"#define DMUI_UI_ABI_CURRENT DMUI_UI_ABI_{contract['abi']}",
        f"#define DMUI_UI_REVISION_{contract['revision']} {contract['revision']}u",
        f"#define DMUI_UI_REVISION_CURRENT DMUI_UI_REVISION_{contract['revision']}",
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

    for _, name, _, _, _ in operations:
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
            "\tuint32_t structSize;",
            "\tuint32_t abiVersion;",
            "\tuint32_t revision;",
            "\tuint32_t reserved;",
        ]
    )
    for _, name, field, _, _ in operations:
        lines.append(f"\tDMUI_UI{name}Fn {field};")
    lines.extend(["} DMUI_UIAPI;", ""])
    for _, name, field, _, _ in operations:
        lines.extend(
            [
                f"#define DMUI_UI_API_{upper_snake(name)}_SIZE \\",
                f"\t((uint32_t)(offsetof(DMUI_UIAPI, {field}) + "
                f"sizeof(DMUI_UI{name}Fn)))",
            ]
        )
    required = [operation for operation in operations if operation[4]][-1]
    lines.extend(
        [
            f"#define DMUI_UI_API_REQUIRED_SIZE "
            f"DMUI_UI_API_{upper_snake(required[1])}_SIZE",
            f"#define DMUI_UI_API_CURRENT_SIZE "
            f"DMUI_UI_API_{upper_snake(operations[-1][1])}_SIZE",
            "",
            "typedef struct DMUI_UIAPIInfo",
            "{",
            "\tuint32_t structSize;",
            "\tuint32_t abiVersion;",
            "\tuint32_t revision;",
            "\tuint32_t tableSize;",
            "\tconst DMUI_UIAPI* api;",
            "} DMUI_UIAPIInfo;",
            "",
            "#define DMUI_UI_API_INFO_PREFIX_SIZE \\",
            "\t((uint32_t)(offsetof(DMUI_UIAPIInfo, tableSize) + sizeof(uint32_t)))",
            "#define DMUI_UI_API_INFO_1_SIZE \\",
            "\t((uint32_t)(offsetof(DMUI_UIAPIInfo, api) + sizeof(const DMUI_UIAPI*)))",
            "",
        ]
    )
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
    required_fields = [
        field for _, _, field, _, required in schema["operations"] if required
    ]
    optional_operations = [
        (name, field)
        for _, name, field, _, required in schema["operations"]
        if not required
    ]
    lines.extend(
        [
            "\tnamespace detail",
            "\t{",
            "\t\t[[nodiscard]] inline bool HasOperationsThroughSize(",
            "\t\t\tconst DMUI_UIAPI* a_api,",
            "\t\t\tuint32_t a_minimumSize) noexcept",
            "\t\t{",
            "\t\t\treturn a_api &&",
            "\t\t\t\ta_minimumSize >= DMUI_UI_API_REQUIRED_SIZE &&",
            "\t\t\t\ta_minimumSize <= DMUI_UI_API_CURRENT_SIZE &&",
            "\t\t\t\ta_api->structSize >= a_minimumSize &&",
        ]
    )
    operation_checks = [
        f"a_api->{field}" for field in required_fields
    ] + [
        f"(a_minimumSize < DMUI_UI_API_{upper_snake(name)}_SIZE || "
        f"a_api->{field})"
        for name, field in optional_operations
    ]
    for index, check in enumerate(operation_checks):
        suffix = ";" if index + 1 == len(operation_checks) else " &&"
        lines.append(f"\t\t\t\t{check}{suffix}")
    lines.extend(["\t\t}", "\t}", "", "\tnamespace checked", "\t{"])
    for _, name, field, _, _ in schema["operations"]:
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
                f"\t\t\t\tDMUI_UI_API_{upper_snake(name)}_SIZE,",
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
        "Color": "ImGuiCol",
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

    for _, name, _, _, _ in schema["operations"]:
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
            "\t\t\tDMUI_UI_API_CURRENT_SIZE,",
            "\t\t\tDMUI_UI_ABI_CURRENT,",
            "\t\t\tDMUI_UI_REVISION_CURRENT,",
            "\t\t\t0u,",
        ]
    )
    for index, (_, name, _, _, _) in enumerate(schema["operations"]):
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
                "required": required,
                "signature": schema["signatures"][name],
            }
            for operation_id, name, field, kind, required in schema["operations"]
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
    baseline_contract = baseline["contract"]
    current_contract = current["contract"]
    for field in ("name", "abi", "namespace", "packedColor"):
        if current_contract.get(field) != baseline_contract.get(field):
            raise GenerationError(f"published contract field changed: {field}")

    baseline_revision = baseline_contract.get("revision")
    current_revision = current_contract.get("revision")
    if not isinstance(baseline_revision, int) or not isinstance(current_revision, int):
        raise GenerationError("contract revisions must be integers")
    if current_revision < baseline_revision:
        raise GenerationError("contract revision moved backwards")

    additions = False
    baseline_enums = baseline["enums"]
    current_enums = current["enums"]
    if len(current_enums) < len(baseline_enums):
        raise GenerationError("published enum family was removed")
    for index, published in enumerate(baseline_enums):
        candidate = current_enums[index]
        for field in ("name", "cName", "kind", "rejectedMask"):
            if candidate.get(field) != published.get(field):
                raise GenerationError(
                    f"published enum definition changed: {published.get('name', index)}"
                )
        for field in ("values", "aliases"):
            published_values = published.get(field, [])
            candidate_values = candidate.get(field, [])
            if candidate_values[: len(published_values)] != published_values:
                raise GenerationError(
                    f"published enum {field} changed or reordered: "
                    f"{published.get('name', index)}"
                )
            additions = additions or len(candidate_values) > len(published_values)
    additions = additions or len(current_enums) > len(baseline_enums)

    baseline_operations = baseline["operations"]
    current_operations = current["operations"]
    if len(current_operations) < len(baseline_operations):
        raise GenerationError("published UI operation was removed")
    if current_operations[: len(baseline_operations)] != baseline_operations:
        raise GenerationError(
            "published UI operation IDs, slots, signatures, or requirements changed"
        )
    additions = additions or len(current_operations) > len(baseline_operations)

    if additions and current_revision <= baseline_revision:
        raise GenerationError(
            "additive enum values or UI slots require a newer contract revision"
        )
    if not additions and current_revision != baseline_revision:
        raise GenerationError(
            "contract revision changed without an additive enum value or UI slot"
        )


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate the stable DearModdingUI UI ABI."
    )
    parser.add_argument("--schema", required=True, type=Path)
    parser.add_argument("--c-header", required=True, type=Path)
    parser.add_argument("--checked-header", required=True, type=Path)
    parser.add_argument("--host-bindings", required=True, type=Path)
    parser.add_argument("--baseline-manifest", required=True, type=Path)
    return parser.parse_args()


def main() -> int:
    arguments = parse_arguments()
    schema = load_schema(arguments.schema.resolve())
    baseline = load_baseline_manifest(arguments.baseline_manifest.resolve())
    validate_compatibility(schema, baseline)
    outputs = {
        arguments.c_header.resolve(): render_c_header(schema),
        arguments.checked_header.resolve(): render_checked_header(schema),
        arguments.host_bindings.resolve(): render_host_bindings(schema),
    }
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
