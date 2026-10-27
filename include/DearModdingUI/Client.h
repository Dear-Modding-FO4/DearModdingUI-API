#pragma once

#include <DearModdingUI/IconGlyphs.h>
#include <DearModdingUI/Presentation/Layout.h>
#include <DearModdingUI/Presentation.h>
#include <DearModdingUI/TextInput.h>
#include <DearModdingUI/TextView.h>
#include <DearModdingUI/UI.h>
#include <DearModdingUI/Win32Discovery.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <limits>
#include <memory>
#include <new>
#include <numeric>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace dmui
{
#if defined(DMUI_UI_TESTING)
	namespace detail
	{
		struct ClientTestAccess;
	}
#endif
	struct Version
	{
		uint16_t major{};
		uint16_t minor{};

		[[nodiscard]] constexpr uint32_t Pack() const noexcept
		{
			return DMUI_MAKE_VERSION(major, minor);
		}
	};

	enum class ClientOriginKind : uint32_t
	{
		kNative = DMUI_CLIENT_ORIGIN_NATIVE,
		kBridged = DMUI_CLIENT_ORIGIN_BRIDGED
	};

	struct ClientOrigin
	{
		ClientOriginKind kind{ ClientOriginKind::kNative };
		std::string_view sourceLabel;
	};

	struct ClientOptions
	{
		DMUI_ClientCapabilities capabilities{ DMUI_CLIENT_CAPABILITY_NONE };
	};

	class ImageResource
	{
	public:
		ImageResource() = default;
		~ImageResource() noexcept
		{
			(void)Release();
		}

		ImageResource(const ImageResource&) = delete;
		ImageResource& operator=(const ImageResource&) = delete;

		ImageResource(ImageResource&& a_other) noexcept :
			release_(std::exchange(a_other.release_, nullptr)),
			client_(std::exchange(
				a_other.client_,
				DMUI_INVALID_CLIENT_HANDLE)),
			handle_(std::exchange(
				a_other.handle_,
				DMUI_INVALID_IMAGE_HANDLE))
		{}

		ImageResource& operator=(ImageResource&& a_other) noexcept
		{
			if (this == std::addressof(a_other))
				return *this;
			(void)Release();
			release_ = std::exchange(a_other.release_, nullptr);
			client_ = std::exchange(
				a_other.client_,
				DMUI_INVALID_CLIENT_HANDLE);
			handle_ = std::exchange(
				a_other.handle_,
				DMUI_INVALID_IMAGE_HANDLE);
			return *this;
		}

		[[nodiscard]] ImageHandle Handle() const noexcept
		{
			return { handle_ };
		}

		[[nodiscard]] DMUI_Result Release() noexcept
		{
			if (!release_ || handle_ == DMUI_INVALID_IMAGE_HANDLE)
				return DMUI_RESULT_OK;
			const auto result = release_(client_, handle_);
			if (result == DMUI_RESULT_OK ||
				result == DMUI_RESULT_STALE_HANDLE ||
				result == DMUI_RESULT_CLIENT_NOT_FOUND)
			{
				release_ = nullptr;
				client_ = DMUI_INVALID_CLIENT_HANDLE;
				handle_ = DMUI_INVALID_IMAGE_HANDLE;
			}
			return result;
		}

	private:
		friend class Client;

		ImageResource(
			DMUI_ReleaseImageFn a_release,
			DMUI_ClientHandle a_client,
			DMUI_ImageHandle a_handle) noexcept :
			release_(a_release),
			client_(a_client),
			handle_(a_handle)
		{}

		DMUI_ReleaseImageFn release_{};
		DMUI_ClientHandle client_{ DMUI_INVALID_CLIENT_HANDLE };
		DMUI_ImageHandle handle_{ DMUI_INVALID_IMAGE_HANDLE };
	};

	struct PageDescriptor
	{
		const char* id{};
		const char* displayName{};
		const char* categoryId{};
		const char* summary{};
		int32_t sortKey{};
		DMUI_PageKind kind{ DMUI_PAGE_KIND_SETTINGS };
		const char* iconName{};
	};

	struct CategoryDescriptor
	{
		const char* id{};
		const char* displayName{};
		int32_t sortKey{};
		const char* iconName{};
	};

	struct ExternalOpen
	{
		DMUI_ExternalTargetKind targetKind{ DMUI_EXTERNAL_TARGET_NONE };
		const char* target{};
		const char* application{};
		std::span<const char* const> arguments;
		const char* workingDirectory{};
	};

	enum class LinkAction : uint32_t
	{
		kCopyTarget = DMUI_LINK_ACTION_COPY_TARGET,
		kOpenExternal = DMUI_LINK_ACTION_OPEN_EXTERNAL
	};

	struct Link
	{
		const char* label{};
		ExternalOpen external;
		const char* note{};
		char32_t glyph{};
		bool enabled{ true };
		LinkAction action{ LinkAction::kCopyTarget };
	};

	struct FaqEntry
	{
		const char* question{};
		const char* answer{};
		// Caller-owned state the host toggles; required.
		bool* expanded{};
	};

	enum class Severity : uint32_t
	{
		kInfo = DMUI_SEVERITY_INFO,
		kSuccess = DMUI_SEVERITY_SUCCESS,
		kWarning = DMUI_SEVERITY_WARNING,
		kError = DMUI_SEVERITY_ERROR
	};

	struct Diagnostic
	{
		Severity severity{ Severity::kInfo };
		const char* scope{};
		const char* summary{};
		const char* detail{};
	};

	enum class PageActivityKind : uint32_t
	{
		kActivated = DMUI_PAGE_ACTIVITY_ACTIVATED,
		kChanged = DMUI_PAGE_ACTIVITY_CHANGED,
		kDeactivated = DMUI_PAGE_ACTIVITY_DEACTIVATED
	};

	struct PageActivity
	{
		PageActivityKind kind{ PageActivityKind::kActivated };
		DMUI_PageHandle previousPage{ DMUI_INVALID_PAGE_HANDLE };
		DMUI_PageHandle activePage{ DMUI_INVALID_PAGE_HANDLE };
	};

	struct VideoMemoryInfo
	{
		uint64_t used{};
		uint64_t budget{};
	};

	using SettingValue = std::variant<bool, double, int64_t, uint64_t, std::string>;

	struct SettingEditEvent
	{
		SettingValue value;
		bool changed{};
		bool completed{};
	};

	template <class T>
	concept SettingValueAlternative =
		std::same_as<std::remove_cvref_t<T>, bool> ||
		std::same_as<std::remove_cvref_t<T>, double> ||
		std::same_as<std::remove_cvref_t<T>, int64_t> ||
		std::same_as<std::remove_cvref_t<T>, uint64_t> ||
		std::same_as<std::remove_cvref_t<T>, std::string>;

	enum class SettingApplyTiming : uint8_t
	{
		kImmediate,
		kNextLaunch
	};

	struct CheckboxSettingControl
	{};

	template <NumericSettingValue T>
	struct NumericSettingControl
	{
		// A slider without two usable bounds is drawn as a drag.
		NumericPresentation presentation{ NumericPresentation::kInput };
		std::optional<NumericSettingRange<T>> range;
		std::optional<NumericQuantization<T>> quantization;
		std::string format;
		float dragSpeed{};
	};

	using DoubleSettingControl = NumericSettingControl<double>;
	using SignedSettingControl = NumericSettingControl<int64_t>;
	using UnsignedSettingControl = NumericSettingControl<uint64_t>;

	struct TextSettingControl
	{
		// Zero is unlimited.
		uint32_t maxBytes{};
		bool multiline{};
	};

	using ChoiceSettingOption = ChoiceOption<std::string>;

	[[nodiscard]] inline const std::string& ResolveChoiceOptionLabel(
		const ChoiceSettingOption& a_option) noexcept
	{
		return a_option.label.empty() ? a_option.value : a_option.label;
	}

	struct ChoiceSettingControl
	{
		std::vector<ChoiceSettingOption> options;
		// Empty uses the localized built-in "Unavailable".
		std::string unmatchedLabel;
	};

	struct ColorSettingPreset
	{
		std::string name;
		std::string description;
		// Packed 0xRRGGBBAA.
		uint32_t rgba{};
	};

	// The value is a uint64_t holding the packed 0xRRGGBBAA color; without alpha
	// the alpha byte is always 0xFF.
	struct ColorSettingControl
	{
		bool alpha{};
		std::string presetsLabel;
		std::vector<ColorSettingPreset> presets;
	};

	// The value is a std::string holding a chord such as "Ctrl+F5"; "none" is unbound.
	// Editing it does not register or move a hotkey action.
	struct KeybindSettingControl
	{
		KeyCaptureSlot slot{ KeyCaptureSlot::kKeyboardMouse };
		bool allowClear{ true };
		// Returns the conflict or invalid-binding message, empty for none. Runs on
		// the render thread for visible rows and must be cheap.
		std::function<std::string()> resolveWarning;
	};

	struct ReadOnlySettingControl
	{
		// Custom drawing is restricted to read-only visualization and must not mutate setting state.
		std::function<void()> draw;
	};

	struct UnsupportedSettingControl
	{
		uint32_t kind{};
	};

	using SettingControl = std::variant<
		CheckboxSettingControl,
		DoubleSettingControl,
		SignedSettingControl,
		UnsignedSettingControl,
		TextSettingControl,
		ChoiceSettingControl,
		ColorSettingControl,
		KeybindSettingControl,
		ReadOnlySettingControl,
		UnsupportedSettingControl>;

	enum class SettingControlKind : uint8_t
	{
		kCheckbox,
		kDouble,
		kSigned,
		kUnsigned,
		kText,
		kChoice,
		kColor,
		kKeybind,
		kReadOnly,
		kUnsupported
	};

	struct SettingControlPresentation
	{
		SettingControlKind kind{ SettingControlKind::kUnsupported };
		bool supported{};
		bool editable{};
		bool resetVisible{};

		constexpr bool operator==(
			const SettingControlPresentation&) const noexcept = default;
	};

	[[nodiscard]] inline SettingControlPresentation ResolveSettingControlPresentation(
		const SettingControl& a_control) noexcept
	{
		if (a_control.valueless_by_exception())
			return {};
		return std::visit(
			[](const auto& a_alternative) noexcept -> SettingControlPresentation {
				using T = std::remove_cvref_t<decltype(a_alternative)>;
				constexpr auto editable = [](SettingControlKind a_kind) noexcept {
					return SettingControlPresentation{ a_kind, true, true, true };
				};
				if constexpr (std::same_as<T, CheckboxSettingControl>)
					return editable(SettingControlKind::kCheckbox);
				else if constexpr (std::same_as<T, DoubleSettingControl>)
					return editable(SettingControlKind::kDouble);
				else if constexpr (std::same_as<T, SignedSettingControl>)
					return editable(SettingControlKind::kSigned);
				else if constexpr (std::same_as<T, UnsignedSettingControl>)
					return editable(SettingControlKind::kUnsigned);
				else if constexpr (std::same_as<T, TextSettingControl>)
					return editable(SettingControlKind::kText);
				else if constexpr (std::same_as<T, ChoiceSettingControl>)
					return editable(SettingControlKind::kChoice);
				else if constexpr (std::same_as<T, ColorSettingControl>)
					return editable(SettingControlKind::kColor);
				else if constexpr (std::same_as<T, KeybindSettingControl>)
					return editable(SettingControlKind::kKeybind);
				else if constexpr (std::same_as<T, ReadOnlySettingControl>)
					return { SettingControlKind::kReadOnly, true, false, false };
				else
					return {};
			},
			a_control);
	}

	[[nodiscard]] inline bool SettingValueMatchesControl(
		const SettingControl& a_control,
		const SettingValue& a_value) noexcept
	{
		switch (ResolveSettingControlPresentation(a_control).kind)
		{
		case SettingControlKind::kCheckbox:
			return std::holds_alternative<bool>(a_value);
		case SettingControlKind::kDouble:
			return std::holds_alternative<double>(a_value);
		case SettingControlKind::kSigned:
			return std::holds_alternative<int64_t>(a_value);
		case SettingControlKind::kUnsigned:
		case SettingControlKind::kColor:
			return std::holds_alternative<uint64_t>(a_value);
		case SettingControlKind::kText:
		case SettingControlKind::kChoice:
		case SettingControlKind::kKeybind:
			return std::holds_alternative<std::string>(a_value);
		default:
			return false;
		}
	}

	struct SettingBinding
	{
		// get runs on the render thread only for visible rows and must be cheap, pure, and non-blocking.
		std::function<SettingValue()> get;
		// set runs there too and returns the effective value; callbacks may throw, but must not reenter drawing, register, select pages, or perform blocking I/O.
		std::function<SettingValue(SettingValue)> set;
	};

	template <class Get>
	using SettingGetterValue =
		std::remove_cvref_t<std::invoke_result_t<std::decay_t<Get>&>>;

	template <class Get, class Set>
	concept SettingBindingCallbacks =
		std::invocable<std::decay_t<Get>&> &&
		SettingValueAlternative<SettingGetterValue<Get>> &&
		std::invocable<std::decay_t<Set>&, SettingGetterValue<Get>> &&
		std::same_as<
			std::remove_cvref_t<
				std::invoke_result_t<
					std::decay_t<Set>&,
					SettingGetterValue<Get>>>,
			SettingGetterValue<Get>>;

	template <class Get, class Set>
		requires SettingBindingCallbacks<Get, Set>
	[[nodiscard]] inline SettingBinding BindSetting(
		Get&& a_get,
		Set&& a_set)
	{
		using T = SettingGetterValue<Get>;
		return {
			[get = std::decay_t<Get>{ std::forward<Get>(a_get) }]() mutable {
				return SettingValue{ std::invoke(get) };
			},
			[set = std::decay_t<Set>{ std::forward<Set>(a_set) }](
				SettingValue a_value) mutable {
				return SettingValue{
					std::invoke(set, std::get<T>(std::move(a_value)))
				};
			}
		};
	}

	struct RowPresentation
	{
		enum class LabelMode : uint32_t
		{
			kAutomatic,
			kHidden
		};

		enum class Layout : uint32_t
		{
			kLabelValue,
			kFullSpan
		};

		LabelMode labelMode{ LabelMode::kAutomatic };
		Layout layout{ Layout::kLabelValue };
	};

	struct FieldFeedback
	{
		Severity severity{ Severity::kInfo };
		std::string message;
	};

	struct SettingDescriptor
	{
		using LabelMode = RowPresentation::LabelMode;

		std::string id;
		std::string label;
		std::string description;
		SettingControl control{ UnsupportedSettingControl{} };
		SettingValue defaultValue{ false };
		SettingBinding binding;
		SettingApplyTiming applyTiming{ SettingApplyTiming::kNextLaunch };
		std::function<bool()> isVisible;
		std::function<bool()> isEnabled;
		std::function<std::string()> resolveLabel;
		std::function<bool()> isDirty;
		std::function<bool()> isModified;
		// Runs immediately after the value widget. binding.set remains live;
		// completed is a separate persistence signal, not a deferred write.
		std::function<void(const SettingEditEvent&)> onEdit;
		bool showReset{ true };
		RowPresentation presentation;
		std::function<std::string()> resolveDescription;
		std::function<std::optional<FieldFeedback>()> resolveFeedback;
	};

	struct SettingsActionRow
	{
		std::string id;
		std::string label;
		std::string buttonLabel;
		std::string description;
		std::function<void()> activate;
		std::function<bool()> isVisible;
		std::function<bool()> isEnabled;
		RowPresentation presentation;
	};

	struct SettingFilter
	{
		std::string search;
		bool modifiedOnly{};
	};

	struct SettingFilterOptions
	{
		bool showSearch{ true };
		bool showModifiedOnly{ true };
		// Empty uses the localized built-in hint.
		std::string searchHint;
	};

	struct SettingGroup
	{
		enum class HeadingMode : uint32_t
		{
			kAutomatic,
			kDivider
		};

		std::string id;
		std::string label;
		char32_t glyph{};
		std::vector<SettingDescriptor> settings;
		struct SettingIndex
		{
			size_t value{};
		};
		struct ActionIndex
		{
			size_t value{};
		};
		struct DividerRow
		{};
		using Row = std::variant<SettingIndex, ActionIndex, DividerRow>;
		bool expanded{ true };
		HeadingMode headingMode{ HeadingMode::kAutomatic };
		std::vector<SettingsActionRow> actionRows;
		std::vector<Row> rows;
	};

	struct SettingsPageActionCallbacks
	{
		// Discrete action callbacks run only on button presses, so persistence I/O is allowed here.
		bool showReset{};
		std::function<void()> reset;
		std::function<void()> revert;
		std::function<void()> apply;
	};

	struct SettingsPageActionTooltips
	{
		std::string reset;
		std::string revert;
		std::function<std::string(size_t)> apply;
	};

	struct SettingsPageNote
	{
		std::string text;
		TextTone tone{ TextTone::kInherit };
		std::string noteId;
	};

	[[nodiscard]] inline bool ContainsFolded(
		std::string_view a_text,
		std::string_view a_search) noexcept
	{
		if (a_search.size() > a_text.size())
			return false;
		const auto fold = [](unsigned char a_character) noexcept {
			return a_character >= 'A' && a_character <= 'Z' ?
				static_cast<unsigned char>(a_character - 'A' + 'a') :
				a_character;
		};
		for (size_t offset = 0;
			 offset + a_search.size() <= a_text.size();
			 ++offset)
		{
			auto matches = true;
			for (size_t index = 0; index < a_search.size(); ++index)
			{
				if (fold(static_cast<unsigned char>(a_text[offset + index])) !=
					fold(static_cast<unsigned char>(a_search[index])))
				{
					matches = false;
					break;
				}
			}
			if (matches)
				return true;
		}
		return false;
	}

	[[nodiscard]] inline bool MatchesSettingFilter(
		const SettingDescriptor& a_setting,
		std::string_view a_label,
		bool a_modified,
		const SettingFilter& a_filter) noexcept
	{
		if (a_filter.modifiedOnly && !a_modified)
			return false;
		if (a_filter.search.empty())
			return true;

		return ContainsFolded(a_setting.id, a_filter.search) ||
			ContainsFolded(a_label, a_filter.search) ||
			ContainsFolded(a_setting.description, a_filter.search);
	}

	[[nodiscard]] inline bool MatchesActionFilter(
		const SettingsActionRow& a_action,
		std::string_view a_label,
		const SettingFilter& a_filter) noexcept
	{
		if (a_filter.modifiedOnly)
			return false;
		if (a_filter.search.empty())
			return true;

		return ContainsFolded(a_action.id, a_filter.search) ||
			ContainsFolded(a_label, a_filter.search) ||
			ContainsFolded(a_action.buttonLabel, a_filter.search) ||
			ContainsFolded(a_action.description, a_filter.search);
	}

	[[nodiscard]] inline bool IsSettingDefault(
		const SettingDescriptor& a_setting,
		const SettingValue& a_value) noexcept
	{
		return SettingValueMatchesControl(a_setting.control, a_setting.defaultValue) &&
			SettingValueMatchesControl(a_setting.control, a_value) &&
			a_value == a_setting.defaultValue;
	}

	[[nodiscard]] inline SettingValue NormalizeSettingValue(
		const SettingDescriptor& a_setting,
		SettingValue a_value)
	{
		if (const auto* control =
				std::get_if<DoubleSettingControl>(&a_setting.control))
		{
			auto value = QuantizeSettingNumber(
				std::get<double>(a_value),
				control->quantization);
			return ClampSettingNumber(
				value,
				std::get<double>(a_setting.defaultValue),
				control->range);
		}
		if (const auto* control =
				std::get_if<SignedSettingControl>(&a_setting.control))
		{
			auto value = QuantizeSettingNumber(
				std::get<int64_t>(a_value),
				control->quantization);
			return ClampSettingNumber(
				value,
				std::get<int64_t>(a_setting.defaultValue),
				control->range);
		}
		if (const auto* control =
				std::get_if<UnsignedSettingControl>(&a_setting.control))
		{
			auto value = QuantizeSettingNumber(
				std::get<uint64_t>(a_value),
				control->quantization);
			return ClampSettingNumber(
				value,
				std::get<uint64_t>(a_setting.defaultValue),
				control->range);
		}
		if (const auto* control =
				std::get_if<ColorSettingControl>(&a_setting.control))
		{
			auto color = std::get<uint64_t>(a_value) & UINT64_C(0xFFFFFFFF);
			return control->alpha ? color : color | UINT64_C(0xFF);
		}
		return a_value;
	}

	[[nodiscard]] inline std::optional<SettingValue> ResetSettingToDefault(
		const SettingDescriptor& a_setting)
	{
		const auto presentation =
			ResolveSettingControlPresentation(a_setting.control);
		if (!presentation.editable ||
			(a_setting.isEnabled && !a_setting.isEnabled()) ||
			!a_setting.binding.set ||
			!SettingValueMatchesControl(
				a_setting.control,
				a_setting.defaultValue))
			return std::nullopt;

		std::optional<SettingValue> previous;
		if (a_setting.binding.get)
		{
			previous = a_setting.binding.get();
			if (!SettingValueMatchesControl(a_setting.control, *previous))
				throw std::bad_variant_access{};
			if (IsSettingDefault(a_setting, *previous))
				return previous;
		}
		else if (a_setting.isModified && !a_setting.isModified())
			return std::nullopt;

		auto effective = a_setting.binding.set(
			NormalizeSettingValue(a_setting, a_setting.defaultValue));
		if (!SettingValueMatchesControl(a_setting.control, effective))
			throw std::bad_variant_access{};
		const auto changed = !previous || *previous != effective;
		if (changed && a_setting.onEdit)
			a_setting.onEdit({ effective, true, true });
		return effective;
	}

	[[nodiscard]] inline size_t SettingsPendingCount(
		const std::vector<SettingGroup>& a_groups)
	{
		size_t pending{};
		for (const auto& group : a_groups)
		{
			for (const auto& setting : group.settings)
			{
				if (setting.isDirty && setting.isDirty())
					++pending;
			}
		}
		return pending;
	}

	class Client;

	struct SettingsPage
	{
		std::vector<SettingGroup> groups;
		SettingsPageActionCallbacks actions;
		SettingsPageActionTooltips actionTooltips;
		SettingFilterOptions filterOptions;
		std::vector<SettingsPageNote> notes;
		std::function<void()> prepare;
		std::function<void(SettingsPage&)> prepareView;
		SettingFilter filter;

		[[nodiscard]] size_t PendingCount() const
		{
			return SettingsPendingCount(groups);
		}

		[[nodiscard]] bool IsDirty() const
		{
			return PendingCount() != 0;
		}

		void ResetToDefaults() const
		{
			for (const auto& group : groups)
			{
				for (const auto& setting : group.settings)
					(void)ResetSettingToDefault(setting);
			}
		}

		void ResetView() noexcept
		{
			filter = {};
			for (auto& group : groups)
				group.expanded = true;
		}

		void Draw(Client& a_client);
	};

	class FontGuard;
	class FieldScope;
	class SettingsTableScope;

	// IMPORTANT: Client instances must outlive the game session because callbacks cannot be unregistered.
	class Client
	{
	public:
		Client(
			std::string_view a_id,
			std::string_view a_displayName,
			Version a_version,
			std::string_view a_iconName = {},
			ClientOrigin a_origin = {},
			ClientOptions a_options = {}) :
			id_(a_id),
			displayName_(a_displayName),
			iconName_(a_iconName),
			origin_(a_origin.kind),
			bridgeSourceLabel_(a_origin.sourceLabel),
			version_(a_version),
			options_(a_options)
		{}

		~Client() = default;

		Client(const Client&) = delete;
		Client(Client&&) = delete;
		Client& operator=(const Client&) = delete;
		Client& operator=(Client&&) = delete;

		[[nodiscard]] bool Connect() noexcept
		{
			if (clientHandle_ != DMUI_INVALID_CLIENT_HANDLE)
			{
				lastResult_ = DMUI_RESULT_OK;
				return true;
			}

			hostPresent_ = detail::HostModulePresent();
			const auto getAPI = FindAPI();
			if (!getAPI)
			{
				api_ = nullptr;
				lastResult_ = hostPresent_ ?
					DMUI_RESULT_UNSUPPORTED_ABI :
					DMUI_RESULT_OK;
				return false;
			}

			api_ = detail::NegotiateAPI(getAPI, abiMinor_);
			if (!api_ || api_->abiMajor != DMUI_ABI_MAJOR)
			{
				api_ = nullptr;
				uiAPI_ = nullptr;
				lastResult_ = DMUI_RESULT_UNSUPPORTED_ABI;
				return false;
			}
			uiAPI_ = api_->ui;

			DMUI_ClientDescriptor descriptor{};
			descriptor.id = id_.c_str();
			descriptor.displayName = displayName_.c_str();
			descriptor.version = version_.Pack();
			descriptor.onHostReady = &OnHostReady;
			descriptor.onHostUnavailable = &OnHostUnavailable;
			descriptor.userData = this;
			descriptor.capabilities = options_.capabilities;
			descriptor.iconName =
				iconName_.empty() ? nullptr : iconName_.c_str();
			descriptor.origin = static_cast<DMUI_ClientOrigin>(origin_);
			descriptor.bridgeSourceLabel =
				bridgeSourceLabel_.empty() ? nullptr : bridgeSourceLabel_.c_str();

			DMUI_ClientHandle handle{ DMUI_INVALID_CLIENT_HANDLE };
			lastResult_ = api_->registerClient(&descriptor, &handle);
			if (lastResult_ != DMUI_RESULT_OK)
				return false;

			clientHandle_ = handle;
			return true;
		}

		[[nodiscard]] uint32_t AbiMinor() const noexcept
		{
			return abiMinor_;
		}

		// a_owner names its <owner>_<language>.txt file, usually the client ID.
		[[nodiscard]] static std::optional<std::string> ResolveText(
			std::string_view a_owner,
			std::string_view a_key,
			DMUI_Result* a_result = nullptr) noexcept
		{
			auto result = DMUI_RESULT_HOST_NOT_INITIALIZED;
			std::optional<std::string> text;
			try
			{
				uint32_t minor{ 0 };
				const auto getAPI = FindAPI();
				const auto* api = getAPI ? detail::NegotiateAPI(getAPI, minor) : nullptr;
				if (getAPI &&
					(!api || api->abiMajor != DMUI_ABI_MAJOR ||
						minor < detail::SlotAbiMinor<&DMUI_HostAPI::resolveText>))
					result = DMUI_RESULT_UNSUPPORTED_ABI;
				else if (api)
				{
					const std::string owner{ a_owner };
					const std::string key{ a_key };
					std::string buffer(64, '\0');
					uint32_t required{ 0 };
					result = api->resolveText(owner.c_str(), key.c_str(), buffer.data(),
						static_cast<uint32_t>(buffer.size()), &required);
					if (result == DMUI_RESULT_BUFFER_TOO_SMALL)
					{
						buffer.resize(required);
						result = api->resolveText(
							owner.c_str(), key.c_str(), buffer.data(), required, &required);
					}
					if (result == DMUI_RESULT_OK)
					{
						buffer.resize(required - 1);
						text = std::move(buffer);
					}
				}
			}
			catch (...)
			{
				result = DMUI_RESULT_RESOURCE_EXHAUSTED;
			}
			if (a_result)
				*a_result = result;
			return text;
		}

		[[nodiscard]] static std::string Localize(
			std::string_view a_owner,
			std::string_view a_key,
			std::string_view a_fallback)
		{
			auto text = ResolveText(a_owner, a_key);
			return text ? std::move(*text) : std::string{ a_fallback };
		}

		template <class Callable>
			requires std::invocable<std::decay_t<Callable>&>
		[[nodiscard]] std::optional<DMUI_PageHandle> AddPage(
			const PageDescriptor& a_page,
			Callable&& a_draw) noexcept
		{
			if (!CanRegisterPage())
				return std::nullopt;

			try
			{
				std::function<void()> callback{ std::forward<Callable>(a_draw) };
				if (!callback)
				{
					Fail(DMUI_RESULT_INVALID_ARGUMENT);
					return std::nullopt;
				}

				pages_.push_back({
					DMUI_INVALID_PAGE_HANDLE,
					this,
					std::move(callback)
				});
				auto& registration = pages_.back();

				DMUI_PageDescriptor descriptor{};
				descriptor.id = a_page.id;
				descriptor.displayName = a_page.displayName;
				descriptor.categoryId = a_page.categoryId;
				descriptor.summary = a_page.summary;
				descriptor.sortKey = a_page.sortKey;
				descriptor.kind = a_page.kind;
				descriptor.draw = &InvokeDrawingCallback<PageRegistration>;
				descriptor.userData = &registration;
				descriptor.iconName = a_page.iconName;

				DMUI_PageHandle handle{ DMUI_INVALID_PAGE_HANDLE };
				lastResult_ = api_->registerPage(clientHandle_, &descriptor, &handle);
				if (lastResult_ != DMUI_RESULT_OK)
				{
					pages_.pop_back();
					return std::nullopt;
				}

				registration.handle = handle;
				return handle;
			}
			catch (const std::bad_alloc&)
			{
				Fail(DMUI_RESULT_RESOURCE_EXHAUSTED);
				return std::nullopt;
			}
			catch (...)
			{
				Fail(DMUI_RESULT_CALLBACK_FAILED);
				return std::nullopt;
			}
		}

		[[nodiscard]] bool AddCategory(
			const CategoryDescriptor& a_category) noexcept
		{
			DMUI_CategoryDescriptor descriptor{};
			descriptor.id = a_category.id;
			descriptor.displayName = a_category.displayName;
			descriptor.sortKey = a_category.sortKey;
			descriptor.iconName = a_category.iconName;
			return Call<&DMUI_HostAPI::registerCategory>(&descriptor);
		}

		[[nodiscard]] bool OpenExternal(
			const ExternalOpen& a_external,
			uint32_t* a_nativeError = nullptr) noexcept
		{
			if (a_nativeError)
				*a_nativeError = 0;
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			if (a_external.arguments.size() >
				(static_cast<size_t>((std::numeric_limits<uint32_t>::max)())))
				return Fail(DMUI_RESULT_INVALID_ARGUMENT);

			DMUI_ExternalOpenDescriptor descriptor{};
			descriptor.targetKind = a_external.targetKind;
			descriptor.target = a_external.target;
			descriptor.application = a_external.application;
			descriptor.arguments = a_external.arguments.data();
			descriptor.argumentCount =
				static_cast<uint32_t>(a_external.arguments.size());
			descriptor.workingDirectory = a_external.workingDirectory;
			lastResult_ =
				api_->openExternal(clientHandle_, &descriptor, a_nativeError);
			return lastResult_ == DMUI_RESULT_OK;
		}

		template <class Page>
			requires std::same_as<std::remove_cvref_t<Page>, SettingsPage>
		[[nodiscard]] std::optional<DMUI_PageHandle> AddSettingsPage(
			PageDescriptor a_descriptor,
			Page&& a_page) noexcept
		{
			try
			{
				SettingsPage page{ std::forward<Page>(a_page) };
				a_descriptor.kind = DMUI_PAGE_KIND_SETTINGS;
				return AddPage(
					a_descriptor,
					[this, page = std::move(page)]() mutable {
						page.Draw(*this);
					});
			}
			catch (const std::bad_alloc&)
			{
				Fail(DMUI_RESULT_RESOURCE_EXHAUSTED);
				return std::nullopt;
			}
			catch (...)
			{
				Fail(DMUI_RESULT_CALLBACK_FAILED);
				return std::nullopt;
			}
		}

		template <class Callable>
			requires std::invocable<std::decay_t<Callable>&, bool>
		[[nodiscard]] std::optional<DMUI_HotkeyActionHandle> AddHotkeyAction(
			const char* a_id,
			const char* a_displayName,
			const char* a_suggestedDefaultChord,
			Callable&& a_callback,
			DMUI_HotkeyContextPolicy a_contextPolicy =
				DMUI_HOTKEY_CONTEXT_ALWAYS) noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}

			try
			{
				std::function<void(bool)> callback{ std::forward<Callable>(a_callback) };
				if (!callback)
				{
					Fail(DMUI_RESULT_INVALID_ARGUMENT);
					return std::nullopt;
				}

				auto callbackState = std::make_unique<HotkeyCallbackState>();
				callbackState->callback = std::move(callback);
				hotkeyActions_.push_back(
					{ DMUI_INVALID_HOTKEY_ACTION_HANDLE, std::move(callbackState) });
				auto& registration = hotkeyActions_.back();
				DMUI_HotkeyActionDescriptor descriptor{};
				descriptor.id = a_id;
				descriptor.displayName = a_displayName;
				descriptor.suggestedDefaultChord = a_suggestedDefaultChord;
				descriptor.callback = &InvokeHotkey;
				descriptor.userData = registration.callback.get();
				descriptor.contextPolicy = a_contextPolicy;

				DMUI_HotkeyActionHandle handle{ DMUI_INVALID_HOTKEY_ACTION_HANDLE };
				lastResult_ = api_->registerHotkeyAction(
					clientHandle_, &descriptor, &handle);
				if (lastResult_ != DMUI_RESULT_OK)
				{
					hotkeyActions_.pop_back();
					return std::nullopt;
				}
				registration.handle = handle;
				return handle;
			}
			catch (const std::bad_alloc&)
			{
				Fail(DMUI_RESULT_RESOURCE_EXHAUSTED);
				return std::nullopt;
			}
			catch (...)
			{
				Fail(DMUI_RESULT_CALLBACK_FAILED);
				return std::nullopt;
			}
		}

		[[nodiscard]] std::optional<DMUI_HotkeyBindingInfo> QueryHotkeyBinding(
			DMUI_HotkeyActionHandle a_action) noexcept
		{
			return Query<DMUI_HotkeyBindingInfo, &DMUI_HostAPI::queryHotkeyBinding>(a_action);
		}

		bool UnregisterHotkeyAction(DMUI_HotkeyActionHandle a_action) noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			const auto registration = std::ranges::find(
				hotkeyActions_, a_action, &HotkeyActionRegistration::handle);
			if (registration == hotkeyActions_.end())
				return Fail(DMUI_RESULT_ACTION_NOT_FOUND);
			lastResult_ = api_->unregisterHotkeyAction(clientHandle_, a_action);
			if (lastResult_ != DMUI_RESULT_OK)
				return false;
			auto* callback = registration->callback.release();
			hotkeyActions_.erase(registration);
			ReleaseCallback(callback);
			return true;
		}

		template <class Callable>
			requires std::invocable<std::decay_t<Callable>&>
		[[nodiscard]] std::optional<DMUI_ActionHandle> AddAction(
			const char* a_id,
			const char* a_label,
			const char* a_iconName,
			const char* a_tooltip,
			Callable&& a_callback,
			int32_t a_sortKey = 0) noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}

			try
			{
				std::function<void()> callback{ std::forward<Callable>(a_callback) };
				if (!callback)
				{
					Fail(DMUI_RESULT_INVALID_ARGUMENT);
					return std::nullopt;
				}

				auto callbackState = std::make_unique<ActionCallbackState>();
				callbackState->owner = this;
				callbackState->callback = std::move(callback);
				actions_.push_back(
					{ DMUI_INVALID_ACTION_HANDLE, std::move(callbackState) });
				auto& registration = actions_.back();

				DMUI_ActionDescriptor descriptor{};
				descriptor.id = a_id;
				descriptor.displayLabel = a_label;
				descriptor.iconName = a_iconName;
				descriptor.tooltip = a_tooltip;
				descriptor.sortKey = a_sortKey;
				descriptor.callback = &InvokeAction;
				descriptor.userData = registration.callback.get();

				DMUI_ActionHandle handle{ DMUI_INVALID_ACTION_HANDLE };
				lastResult_ = api_->registerAction(clientHandle_, &descriptor, &handle);
				if (lastResult_ != DMUI_RESULT_OK)
				{
					actions_.pop_back();
					return std::nullopt;
				}

				registration.handle = handle;
				return handle;
			}
			catch (const std::bad_alloc&)
			{
				Fail(DMUI_RESULT_RESOURCE_EXHAUSTED);
				return std::nullopt;
			}
			catch (...)
			{
				Fail(DMUI_RESULT_CALLBACK_FAILED);
				return std::nullopt;
			}
		}

		// Render thread only; see DMUI_UnregisterActionFn.
		bool UnregisterAction(DMUI_ActionHandle a_action) noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			const auto registration = std::ranges::find(
				actions_, a_action, &ActionRegistration::handle);
			if (registration == actions_.end())
				return Fail(DMUI_RESULT_ACTION_NOT_FOUND);
			lastResult_ = api_->unregisterAction(clientHandle_, a_action);
			if (lastResult_ != DMUI_RESULT_OK)
				return false;
			auto* callback = registration->callback.release();
			actions_.erase(registration);
			ReleaseCallback(callback);
			return true;
		}

		template <class Callable>
			requires std::invocable<std::decay_t<Callable>&>
		[[nodiscard]] std::optional<DMUI_FrameObserverHandle> AddFrameObserver(
			Callable&& a_callback) noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}

			try
			{
				std::function<void()> callback{ std::forward<Callable>(a_callback) };
				if (!callback)
				{
					Fail(DMUI_RESULT_INVALID_ARGUMENT);
					return std::nullopt;
				}

				frameObservers_.push_back(
					{ DMUI_INVALID_FRAME_OBSERVER_HANDLE, std::move(callback) });
				auto& registration = frameObservers_.back();

				DMUI_FrameObserverDescriptor descriptor{};
				descriptor.callback = &Invoke;
				descriptor.userData = &registration.callback;

				DMUI_FrameObserverHandle handle{ DMUI_INVALID_FRAME_OBSERVER_HANDLE };
				lastResult_ =
					api_->registerFrameObserver(clientHandle_, &descriptor, &handle);
				if (lastResult_ != DMUI_RESULT_OK)
				{
					frameObservers_.pop_back();
					return std::nullopt;
				}

				registration.handle = handle;
				return handle;
			}
			catch (const std::bad_alloc&)
			{
				Fail(DMUI_RESULT_RESOURCE_EXHAUSTED);
				return std::nullopt;
			}
			catch (...)
			{
				Fail(DMUI_RESULT_CALLBACK_FAILED);
				return std::nullopt;
			}
		}

		template <class Callable>
			requires std::invocable<std::decay_t<Callable>&, const PageActivity&>
		[[nodiscard]] std::optional<DMUI_PageActivityObserverHandle>
			AddPageActivityObserver(Callable&& a_callback) noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}

			try
			{
				std::function<void(const PageActivity&)> callback{
					std::forward<Callable>(a_callback)
				};
				if (!callback)
				{
					Fail(DMUI_RESULT_INVALID_ARGUMENT);
					return std::nullopt;
				}

				pageActivityObservers_.push_back({
					DMUI_INVALID_PAGE_ACTIVITY_OBSERVER_HANDLE,
					std::move(callback)
				});
				auto& registration = pageActivityObservers_.back();
				DMUI_PageActivityObserverDescriptor descriptor{};
				descriptor.callback = &InvokePageActivity;
				descriptor.userData = &registration.callback;

				DMUI_PageActivityObserverHandle handle{
					DMUI_INVALID_PAGE_ACTIVITY_OBSERVER_HANDLE
				};
				lastResult_ = api_->registerPageActivityObserver(
					clientHandle_,
					&descriptor,
					&handle);
				if (lastResult_ != DMUI_RESULT_OK)
				{
					pageActivityObservers_.pop_back();
					return std::nullopt;
				}

				registration.handle = handle;
				return handle;
			}
			catch (const std::bad_alloc&)
			{
				Fail(DMUI_RESULT_RESOURCE_EXHAUSTED);
				return std::nullopt;
			}
			catch (...)
			{
				Fail(DMUI_RESULT_CALLBACK_FAILED);
				return std::nullopt;
			}
		}

		[[nodiscard]] std::optional<VideoMemoryInfo> QueryVideoMemory() noexcept
		{
			VideoMemoryInfo info{};
			if (!Call<&DMUI_HostAPI::queryVideoMemory>(&info.used, &info.budget))
				return std::nullopt;
			return info;
		}

		[[nodiscard]] bool RequestFrame(DMUI_PageHandle a_page) noexcept
		{
			return Call<&DMUI_HostAPI::requestFrame>(a_page);
		}

		[[nodiscard]] bool ReleaseFrame(DMUI_PageHandle a_page) noexcept
		{
			return Call<&DMUI_HostAPI::releaseFrame>(a_page);
		}

		[[nodiscard]] bool AttachSwapChain(void* a_nativeSwapChain) noexcept
		{
			return Call<&DMUI_HostAPI::attachSwapChain>(a_nativeSwapChain);
		}

		[[nodiscard]] bool SetHotkeyActionEnabled(
			DMUI_HotkeyActionHandle a_action,
			bool a_enabled) noexcept
		{
			return Call<&DMUI_HostAPI::setHotkeyActionEnabled>(a_action, a_enabled ? 1u : 0u);
		}

		[[nodiscard]] bool SetActionEnabled(
			DMUI_ActionHandle a_action,
			bool a_enabled) noexcept
		{
			return Call<&DMUI_HostAPI::setActionEnabled>(a_action, a_enabled ? 1u : 0u);
		}

		[[nodiscard]] std::optional<ImageResource> ImportD3D11Image(
			void* a_shaderResourceView,
			uint32_t a_contentWidth = 0,
			uint32_t a_contentHeight = 0) noexcept
		{
			const DMUI_D3D11ImageDescriptor descriptor{
				a_shaderResourceView,
				a_contentWidth,
				a_contentHeight
			};
			const auto handle =
				Query<DMUI_ImageHandle, &DMUI_HostAPI::importD3D11Image>(&descriptor);
			if (!handle)
				return std::nullopt;
			return ImageResource{ api_->releaseImage, clientHandle_, *handle };
		}

		[[nodiscard]] std::optional<ImageResource> CreateImage(
			const DMUI_ImageDescriptor& a_descriptor) noexcept
		{
			const auto handle =
				Query<DMUI_ImageHandle, &DMUI_HostAPI::createImage>(&a_descriptor);
			if (!handle)
				return std::nullopt;
			return ImageResource{ api_->releaseImage, clientHandle_, *handle };
		}

		[[nodiscard]] std::optional<ImageResource> LoadImageFile(const char* a_utf8Path) noexcept
		{
			const auto handle =
				Query<DMUI_ImageHandle, &DMUI_HostAPI::loadImageFile>(a_utf8Path);
			if (!handle)
				return std::nullopt;
			return ImageResource{ api_->releaseImage, clientHandle_, *handle };
		}

		// Same load as LoadImageFile from encoded bytes; they are copied before return.
		[[nodiscard]] std::optional<ImageResource> LoadImageMemory(
			std::span<const std::byte> a_bytes) noexcept
		{
			const auto handle = Query<DMUI_ImageHandle, &DMUI_HostAPI::loadImageMemory>(
				static_cast<const void*>(a_bytes.data()), a_bytes.size());
			if (!handle)
				return std::nullopt;
			return ImageResource{ api_->releaseImage, clientHandle_, *handle };
		}

		[[nodiscard]] bool UpdateImage(
			ImageHandle a_image,
			const DMUI_ImageDescriptor& a_descriptor) noexcept
		{
			return Call<&DMUI_HostAPI::updateImage>(a_image.value, &a_descriptor);
		}

		[[nodiscard]] std::optional<DMUI_ImageInfo> QueryImage(
			ImageHandle a_image) noexcept
		{
			return Query<DMUI_ImageInfo, &DMUI_HostAPI::queryImage>(a_image.value);
		}

		[[nodiscard]] bool ConfigureOverlay(
			DMUI_PageHandle a_page,
			const DMUI_ManagedOverlayOptions& a_options) noexcept
		{
			return Call<&DMUI_HostAPI::configureOverlay>(a_page, &a_options);
		}

		[[nodiscard]] std::optional<DMUI_ManagedOverlayPlacement>
			QueryOverlay(DMUI_PageHandle a_page) noexcept
		{
			return Query<DMUI_ManagedOverlayPlacement, &DMUI_HostAPI::queryOverlay>(a_page);
		}

		[[nodiscard]] bool ResetOverlay(DMUI_PageHandle a_page) noexcept
		{
			return Call<&DMUI_HostAPI::resetOverlay>(a_page);
		}

		[[nodiscard]] bool RequestOverlayFocus(DMUI_PageHandle a_page) noexcept
		{
			return Call<&DMUI_HostAPI::requestOverlayFocus>(a_page);
		}

		[[nodiscard]] bool ReleaseOverlayFocus(DMUI_PageHandle a_page) noexcept
		{
			return Call<&DMUI_HostAPI::releaseOverlayFocus>(a_page);
		}

		[[nodiscard]] std::optional<DMUI_OverlayFocusInfo>
			QueryOverlayFocus(DMUI_PageHandle a_page) noexcept
		{
			return Query<DMUI_OverlayFocusInfo, &DMUI_HostAPI::queryOverlayFocus>(a_page);
		}

		[[nodiscard]] bool PostNotification(
			Severity a_severity,
			const char* a_message,
			uint32_t a_durationMilliseconds = 0,
			const char* a_title = nullptr) noexcept
		{
			const DMUI_NotificationDescriptor descriptor{
				static_cast<DMUI_Severity>(a_severity),
				a_message,
				a_durationMilliseconds,
				a_title
			};
			return Call<&DMUI_HostAPI::postNotification>(&descriptor);
		}

		[[nodiscard]] std::optional<DMUI_DialogHandle> RequestDialog(
			const DMUI_DialogDescriptor& a_descriptor) noexcept
		{
			return Query<DMUI_DialogHandle, &DMUI_HostAPI::requestDialog>(&a_descriptor);
		}

		[[nodiscard]] std::optional<DMUI_DialogEvent> PollDialogEvent(
			DMUI_DialogHandle a_dialog,
			std::string& a_text) noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}
			try
			{
				DMUI_DialogEvent event{};
				if (a_text.size() >=
					static_cast<size_t>((std::numeric_limits<uint32_t>::max)()))
				{
					Fail(DMUI_RESULT_RESOURCE_EXHAUSTED);
					return std::nullopt;
				}
				std::vector<char> text((std::max)(a_text.size() + 1, size_t{ 65 }));
				lastResult_ = api_->pollDialogEvent(
					clientHandle_,
					a_dialog,
					&event,
					text.data(),
					static_cast<uint32_t>(text.size()));
				if (lastResult_ == DMUI_RESULT_BUFFER_TOO_SMALL)
				{
					text.resize(event.requiredTextCapacity);
					lastResult_ = api_->pollDialogEvent(
						clientHandle_,
						a_dialog,
						&event,
						text.data(),
						static_cast<uint32_t>(text.size()));
				}
				if (lastResult_ != DMUI_RESULT_OK)
					return std::nullopt;
				a_text.assign(text.data());
				return event;
			}
			catch (...)
			{
				Fail(DMUI_RESULT_RESOURCE_EXHAUSTED);
				return std::nullopt;
			}
		}

		[[nodiscard]] bool ResolveDialogSubmission(
			DMUI_DialogHandle a_dialog,
			uint64_t a_submissionId,
			bool a_accepted,
			const char* a_error = nullptr) noexcept
		{
			return Call<&DMUI_HostAPI::resolveDialogSubmission>(
				a_dialog,
				a_submissionId,
				a_accepted ? 1u : 0u,
				a_error);
		}

		[[nodiscard]] bool CancelDialog(
			DMUI_DialogHandle a_dialog) noexcept
		{
			return Call<&DMUI_HostAPI::cancelDialog>(a_dialog);
		}

		bool SetStatus(Severity a_severity, const char* a_message) noexcept
		{
			return Call<&DMUI_HostAPI::setStatus>(
				static_cast<DMUI_Severity>(a_severity), a_message);
		}

		bool ReportDiagnostic(const Diagnostic& a_diagnostic) noexcept
		{
			const DMUI_DiagnosticDescriptor descriptor{
				static_cast<DMUI_Severity>(a_diagnostic.severity),
				a_diagnostic.scope,
				a_diagnostic.summary,
				a_diagnostic.detail
			};
			return Call<&DMUI_HostAPI::reportDiagnostic>(&descriptor);
		}

		[[nodiscard]] std::optional<DMUI_StyleMetrics> GetStyleMetrics() noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}
			DMUI_StyleMetrics metrics{};
			lastResult_ = ui::GetStyleMetrics(metrics);
			if (lastResult_ != DMUI_RESULT_OK)
				return std::nullopt;
			return metrics;
		}

		[[nodiscard]] bool PushFont(DMUI_FontRole a_role) noexcept
		{
			return Call<&DMUI_HostAPI::pushFont>(a_role);
		}

		[[nodiscard]] bool PopFont() noexcept
		{
			return Call<&DMUI_HostAPI::popFont>();
		}

		[[nodiscard]] std::optional<char32_t> ResolveIconGlyph(
			const char* a_primaryMetadata,
			const char* a_explicitName = nullptr,
			const char* a_secondaryMetadata = nullptr) noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}

			const DMUI_IconResolutionRequest request{
				a_explicitName,
				a_primaryMetadata,
				a_secondaryMetadata
			};
			uint32_t glyph{};
			lastResult_ = api_->resolveIconGlyph(&request, &glyph);
			if (lastResult_ != DMUI_RESULT_OK)
				return std::nullopt;
			return static_cast<char32_t>(glyph);
		}

		[[nodiscard]] bool DrawTextView(
			const char* a_id,
			const TextViewRequest& a_request,
			TextViewState& a_state) noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);

			auto presentation = a_state;
			SynchronizeTextViewState(a_request, presentation);
			DMUI_TextViewDescriptor descriptor{
				a_id,
				a_request.text.data(),
				a_request.text.size(),
				a_request.lineOffsets.data(),
				a_request.lineOffsets.size(),
				a_request.matchByteOffsets.data(),
				a_request.matchByteOffsets.size(),
				a_request.matchByteLength,
				a_request.spans.data(),
				a_request.spans.size(),
				a_request.contentRevision,
				a_request.matchRevision,
				a_request.spanRevision,
				a_request.firstLineNumber,
				(a_request.followTail ? DMUI_TEXT_VIEW_FOLLOW_TAIL : 0u) |
					(a_request.selectable ? DMUI_TEXT_VIEW_SELECTABLE : 0u),
				a_request.viewport
			};
			DMUI_TextViewState state{
				presentation.contentRevision,
				presentation.matchRevision,
				presentation.activeMatch,
				presentation.revealByteOffset,
				kNoTextOffset,
				0u,
				static_cast<DMUI_TextViewScrollCommand>(presentation.scroll),
				0u,
				0u
			};
			lastResult_ = api_->drawTextView(
				clientHandle_,
				&descriptor,
				&state);
			if (lastResult_ != DMUI_RESULT_OK)
				return false;
			a_state = {
				state.contentRevision,
				state.matchRevision,
				state.activeMatch,
				state.revealByteOffset,
				TextViewScroll::kNone,
				state.atBottom != 0,
				(state.events & DMUI_TEXT_VIEW_EVENT_SPAN_HOVERED) != 0,
				(state.events & DMUI_TEXT_VIEW_EVENT_SPAN_CLICKED) != 0,
				state.spanIndex,
				state.userId
			};
			return true;
		}

		[[nodiscard]] bool DrawLinkRow(
			const char* a_id,
			std::span<const Link> a_links) noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);

			try
			{
				std::vector<DMUI_ExternalOpenDescriptor> external;
				std::vector<DMUI_LinkDescriptor> descriptors;
				external.reserve(a_links.size());
				descriptors.reserve(a_links.size());
				for (const auto& link : a_links)
				{
					if (link.external.arguments.size() >
						(static_cast<size_t>((std::numeric_limits<uint32_t>::max)())))
						return Fail(DMUI_RESULT_INVALID_ARGUMENT);
					external.push_back({
						link.external.targetKind,
						link.external.target,
						link.external.application,
						link.external.arguments.data(),
						static_cast<uint32_t>(link.external.arguments.size()),
						0,
						link.external.workingDirectory
					});
					descriptors.push_back({
						link.label,
						link.note,
						static_cast<uint32_t>(link.glyph),
						link.enabled ? 1u : 0u,
						static_cast<DMUI_LinkAction>(link.action),
						0,
						&external.back()
					});
				}
				lastResult_ = api_->drawLinkRow(
					clientHandle_,
					a_id,
					descriptors.empty() ? nullptr : descriptors.data(),
					descriptors.size());
				return lastResult_ == DMUI_RESULT_OK;
			}
			catch (const std::bad_alloc&)
			{
				return Fail(DMUI_RESULT_RESOURCE_EXHAUSTED);
			}
			catch (...)
			{
				return Fail(DMUI_RESULT_CALLBACK_FAILED);
			}
		}

		[[nodiscard]] bool DrawFaq(
			const char* a_id,
			std::span<const FaqEntry> a_entries) noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);

			try
			{
				std::vector<uint32_t> expanded;
				std::vector<DMUI_FaqEntry> entries;
				expanded.reserve(a_entries.size());
				entries.reserve(a_entries.size());
				for (const auto& entry : a_entries)
				{
					if (!entry.expanded)
						return Fail(DMUI_RESULT_INVALID_ARGUMENT);
					expanded.push_back(*entry.expanded ? 1u : 0u);
				}
				for (size_t index = 0; index < a_entries.size(); ++index)
				{
					entries.push_back({
						a_entries[index].question,
						a_entries[index].answer,
						&expanded[index]
					});
				}
				lastResult_ = api_->drawFaq(
					clientHandle_,
					a_id,
					entries.empty() ? nullptr : entries.data(),
					entries.size());
				if (lastResult_ != DMUI_RESULT_OK)
					return false;
				for (size_t index = 0; index < a_entries.size(); ++index)
					*a_entries[index].expanded = expanded[index] != 0;
				return true;
			}
			catch (const std::bad_alloc&)
			{
				return Fail(DMUI_RESULT_RESOURCE_EXHAUSTED);
			}
			catch (...)
			{
				return Fail(DMUI_RESULT_CALLBACK_FAILED);
			}
		}

		[[nodiscard]] std::optional<bool> BeginSettingsTable(const char* a_id) noexcept
		{
			return QueryFlag<&DMUI_HostAPI::beginSettingsTable>(a_id);
		}

		[[nodiscard]] std::optional<bool> BeginField(
			const char* a_id,
			const char* a_label,
			const char* a_description = nullptr,
			RowPresentation::Layout a_layout =
				RowPresentation::Layout::kLabelValue) noexcept
		{
			const DMUI_FieldBeginOptions options{
				a_layout == RowPresentation::Layout::kFullSpan ?
					DMUI_FIELD_LAYOUT_FULL_SPAN :
					DMUI_FIELD_LAYOUT_LABEL_VALUE
			};
			return QueryFlag<&DMUI_HostAPI::beginField>(
				a_id, a_label, a_description, &options);
		}

		[[nodiscard]] bool SetFieldFeedback(
			Severity a_severity,
			const char* a_message) noexcept
		{
			const DMUI_FieldFeedback feedback{
				static_cast<DMUI_Severity>(a_severity),
				a_message
			};
			return Call<&DMUI_HostAPI::setFieldFeedback>(&feedback);
		}

		[[nodiscard]] bool ClearFieldFeedback() noexcept
		{
			return SetFieldFeedback(
				Severity::kInfo,
				nullptr);
		}

		[[nodiscard]] std::optional<bool> EndField(
			bool a_resetVisible = false,
			bool a_resetEnabled = false) noexcept
		{
			const DMUI_FieldEndOptions options{
				a_resetVisible ? 1u : 0u,
				a_resetEnabled ? 1u : 0u
			};
			return QueryFlag<&DMUI_HostAPI::endField>(&options);
		}

		[[nodiscard]] bool EndSettingsTable() noexcept
		{
			return Call<&DMUI_HostAPI::endSettingsTable>();
		}

		[[nodiscard]] bool SelectPage(DMUI_PageHandle a_page) noexcept
		{
			return Call<&DMUI_HostAPI::selectPage>(a_page);
		}

		[[nodiscard]] std::optional<bool> IsMenuVisible() noexcept
		{
			if (!api_)
			{
				Fail(DMUI_RESULT_HOST_NOT_INITIALIZED);
				return std::nullopt;
			}

			uint32_t visible{};
			lastResult_ = api_->isMenuVisible(&visible);
			if (lastResult_ != DMUI_RESULT_OK)
				return std::nullopt;
			return visible != 0;
		}

		[[nodiscard]] std::optional<DMUI_HostStateInfo> QueryState() noexcept
		{
			if (!api_)
			{
				Fail(DMUI_RESULT_HOST_NOT_INITIALIZED);
				return std::nullopt;
			}

			DMUI_HostStateInfo state{};
			lastResult_ = api_->queryState(&state);
			if (lastResult_ != DMUI_RESULT_OK)
				return std::nullopt;
			return state;
		}

		[[nodiscard]] bool HostPresent() const noexcept
		{
			return hostPresent_;
		}

		[[nodiscard]] bool IsConnected() const noexcept
		{
			return clientHandle_ != DMUI_INVALID_CLIENT_HANDLE;
		}

		[[nodiscard]] DMUI_Result LastResult() const noexcept
		{
			return lastResult_.load(std::memory_order_relaxed);
		}

		[[nodiscard]] DMUI_UnavailableReason UnavailableReason() const noexcept
		{
			return unavailableReason_.load(std::memory_order_relaxed);
		}

	private:
		friend class FontGuard;
#if defined(DMUI_UI_TESTING)
		friend struct detail::ClientTestAccess;
#endif
		friend class FieldScope;
		friend class SettingsTableScope;
		using GetAPIFn = const DMUI_HostAPI* (DMUI_CALL*)(uint32_t) noexcept;

		struct PageRegistration
		{
			DMUI_PageHandle handle;
			Client* owner;
			std::function<void()> callback;
		};

		// Counted so a callback may unregister its own action while it runs.
		template <class Signature>
		struct CallbackState
		{
			std::atomic<uint32_t> references{ 1 };
			Client* owner{};
			std::function<Signature> callback;
		};

		template <class Handle, class State>
		struct CallbackRegistration
		{
			Handle handle;
			std::unique_ptr<State> callback;
		};

		using ActionCallbackState = CallbackState<void()>;
		using ActionRegistration =
			CallbackRegistration<DMUI_ActionHandle, ActionCallbackState>;

		struct FrameObserverRegistration
		{
			DMUI_FrameObserverHandle handle;
			std::function<void()> callback;
		};

		struct PageActivityObserverRegistration
		{
			DMUI_PageActivityObserverHandle handle;
			std::function<void(const PageActivity&)> callback;
		};

		using HotkeyCallbackState = CallbackState<void(bool)>;
		using HotkeyActionRegistration =
			CallbackRegistration<DMUI_HotkeyActionHandle, HotkeyCallbackState>;

		[[nodiscard]] static GetAPIFn FindAPI() noexcept
		{
			return detail::ResolveHostSymbol<GetAPIFn>("DMUI_GetAPI");
		}

		[[nodiscard]] bool CanRegisterPage() noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			return true;
		}

		bool Fail(DMUI_Result a_result) noexcept
		{
			lastResult_ = a_result;
			return false;
		}

		template <auto Slot, class... Args>
		[[nodiscard]] bool Call(Args&&... a_args) noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			if (abiMinor_ < detail::SlotAbiMinor<Slot>)
				return Fail(DMUI_RESULT_UNSUPPORTED_ABI);
			lastResult_ = (api_->*Slot)(clientHandle_, std::forward<Args>(a_args)...);
			return lastResult_ == DMUI_RESULT_OK;
		}

		template <class Value, auto Slot, class... Args>
		[[nodiscard]] std::optional<Value> Query(Args&&... a_args) noexcept
		{
			Value value{};
			if (!Call<Slot>(std::forward<Args>(a_args)..., &value))
				return std::nullopt;
			return value;
		}

		template <auto Slot, class... Args>
		[[nodiscard]] std::optional<bool> QueryFlag(Args&&... a_args) noexcept
		{
			const auto flag = Query<uint32_t, Slot>(std::forward<Args>(a_args)...);
			if (!flag)
				return std::nullopt;
			return *flag != 0;
		}

		void ReportUIFailure(DMUI_Result a_result) noexcept
		{
			if (!api_ ||
				clientHandle_ == DMUI_INVALID_CLIENT_HANDLE)
				return;
			const DMUI_DiagnosticDescriptor diagnostic{
				DMUI_SEVERITY_ERROR,
				"ui-contract",
				"Stable UI operation failed; drawing callback was disabled.",
				DMUI_ResultToString(a_result)
			};
			(void)api_->reportDiagnostic(clientHandle_, &diagnostic);
		}

		void SetPresentationResult(DMUI_Result a_result) noexcept
		{
			lastResult_ = a_result;
		}

		static void DMUI_CALL OnHostReady(
			const DMUI_HostReadyInfo* a_info,
			void* a_userData) noexcept
		{
			if (!a_info)
				return;
			(void)a_userData;
		}

		static void DMUI_CALL OnHostUnavailable(
			DMUI_UnavailableReason a_reason,
			void* a_userData) noexcept
		{
			if (auto* const self = static_cast<Client*>(a_userData))
				self->unavailableReason_.store(a_reason, std::memory_order_relaxed);
		}

		template <class Registration>
		static DMUI_Result DMUI_CALL InvokeDrawingCallback(void* a_userData) noexcept
		{
			auto* const registration =
				static_cast<Registration*>(a_userData);
			if (!registration || !registration->owner)
				return DMUI_RESULT_INVALID_ARGUMENT;
			auto& owner = *registration->owner;
			owner.lastResult_ = DMUI_RESULT_OK;
			ui::detail::ScopedContext uiContext{
				owner.uiAPI_,
				owner.clientHandle_,
				owner.abiMinor_
			};
			try
			{
				registration->callback();
			}
			catch (...)
			{
				owner.lastResult_ = DMUI_RESULT_CALLBACK_FAILED;
				return DMUI_RESULT_CALLBACK_FAILED;
			}
			const auto uiResult = uiContext.Result();
			// Contention and unsupported features allow drawing callbacks to fall back.
			const auto hostResult =
				owner.LastResult() == DMUI_RESULT_BUSY ||
				owner.LastResult() == DMUI_RESULT_UNSUPPORTED_ABI ?
				DMUI_RESULT_OK : owner.LastResult();
			const auto result = uiResult != DMUI_RESULT_OK ?
				uiResult :
				hostResult;
			if (result != DMUI_RESULT_OK)
			{
				owner.lastResult_ = result;
				owner.ReportUIFailure(result);
			}
			return result;
		}

		static void DMUI_CALL Invoke(void* a_userData) noexcept
		{
			if (!a_userData)
				return;
			try
			{
				(*static_cast<std::function<void()>*>(a_userData))();
			}
			catch (...)
			{}
		}

		static void DMUI_CALL InvokePageActivity(
			const DMUI_PageActivityInfo* a_info,
			void* a_userData) noexcept
		{
			if (!a_info ||
				!a_userData)
				return;
			try
			{
				(*static_cast<std::function<void(const PageActivity&)>*>(
					a_userData))({
					static_cast<PageActivityKind>(a_info->kind),
					a_info->previousPage,
					a_info->activePage
				});
			}
			catch (...)
			{}
		}

		static void DMUI_CALL InvokeHotkey(
			DMUI_HotkeyActionHandle,
			uint32_t a_pressed,
			void* a_userData) noexcept
		{
			auto* const state = static_cast<HotkeyCallbackState*>(a_userData);
			if (!state)
				return;
			state->references.fetch_add(1, std::memory_order_relaxed);
			try
			{
				state->callback(a_pressed != 0);
			}
			catch (...)
			{}
			ReleaseCallback(state);
		}

		static DMUI_Result DMUI_CALL InvokeAction(void* a_userData) noexcept
		{
			auto* const state = static_cast<ActionCallbackState*>(a_userData);
			if (!state)
				return DMUI_RESULT_INVALID_ARGUMENT;
			state->references.fetch_add(1, std::memory_order_relaxed);
			const auto result = InvokeDrawingCallback<ActionCallbackState>(state);
			ReleaseCallback(state);
			return result;
		}

		template <class State>
		static void ReleaseCallback(State* a_state) noexcept
		{
			if (a_state &&
				a_state->references.fetch_sub(1, std::memory_order_acq_rel) == 1)
				delete a_state;
		}

		std::string id_;
		std::string displayName_;
		std::string iconName_;
		ClientOriginKind origin_{ ClientOriginKind::kNative };
		std::string bridgeSourceLabel_;
		Version version_;
		ClientOptions options_;
		const DMUI_HostAPI* api_{};
		const DMUI_UIAPI* uiAPI_{};
		uint32_t abiMinor_{};
		DMUI_ClientHandle clientHandle_{ DMUI_INVALID_CLIENT_HANDLE };
		std::atomic<DMUI_Result> lastResult_{ DMUI_RESULT_OK };
		std::atomic<DMUI_UnavailableReason> unavailableReason_{ DMUI_UNAVAILABLE_NONE };
		bool hostPresent_{};
		std::deque<PageRegistration> pages_;
		std::deque<ActionRegistration> actions_;
		std::deque<FrameObserverRegistration> frameObservers_;
		std::deque<PageActivityObserverRegistration> pageActivityObservers_;
		std::deque<HotkeyActionRegistration> hotkeyActions_;
	};

	struct ScopeResult
	{
		DMUI_Result result{ DMUI_RESULT_OK };
		bool visible{};

		[[nodiscard]] constexpr explicit operator bool() const noexcept
		{
			return result == DMUI_RESULT_OK;
		}
	};

	class DialogSession
	{
	public:
		using Submit = std::function<std::optional<std::string>(std::string_view)>;

		DialogSession() = default;
		~DialogSession() noexcept { Cancel(); }
		DialogSession(const DialogSession&) = delete;
		DialogSession& operator=(const DialogSession&) = delete;
		DialogSession(DialogSession&&) = delete;
		DialogSession& operator=(DialogSession&&) = delete;

		[[nodiscard]] bool Open(
			Client& a_client,
			const DMUI_DialogDescriptor& a_descriptor,
			Submit a_submit) noexcept
		{
			if (Active())
			{
				lastResult_ = DMUI_RESULT_BUSY;
				return false;
			}
			if (!a_submit)
			{
				lastResult_ = DMUI_RESULT_INVALID_ARGUMENT;
				return false;
			}
			const auto handle = a_client.RequestDialog(a_descriptor);
			lastResult_ = a_client.LastResult();
			if (!handle)
				return false;
			client_ = &a_client;
			handle_ = *handle;
			submit_ = std::move(a_submit);
			return true;
		}

		void Poll() noexcept
		{
			while (Active())
			{
				const auto event = client_->PollDialogEvent(handle_, text_);
				lastResult_ = client_->LastResult();
				if (!event)
				{
					FailAndCancel(lastResult_);
					return;
				}
				if (event->kind == DMUI_DIALOG_EVENT_PENDING)
					return;
				if (event->kind == DMUI_DIALOG_EVENT_CANCELLED ||
					event->kind == DMUI_DIALOG_EVENT_COMPLETED)
				{
					Clear();
					return;
				}
				try
				{
					const auto error = submit_(text_);
					if (!client_->ResolveDialogSubmission(
							handle_, event->submissionId, !error, error ? error->c_str() : nullptr))
					{
						FailAndCancel(client_->LastResult());
						return;
					}
				}
				catch (const std::bad_alloc&)
				{
					FailAndCancel(DMUI_RESULT_RESOURCE_EXHAUSTED);
					return;
				}
				catch (...)
				{
					FailAndCancel(DMUI_RESULT_CALLBACK_FAILED);
					return;
				}
			}
		}

		[[nodiscard]] bool Active() const noexcept { return client_ != nullptr; }
		[[nodiscard]] DMUI_Result LastResult() const noexcept { return lastResult_; }

		void Cancel() noexcept
		{
			if (!Active())
				return;
			if (!client_->CancelDialog(handle_))
			{
				lastResult_ = client_->LastResult();
				if (lastResult_ == DMUI_RESULT_BUSY)
				{
					const auto event = client_->PollDialogEvent(handle_, text_);
					lastResult_ = client_->LastResult();
					if (event && event->kind == DMUI_DIALOG_EVENT_SUBMITTED)
					{
						if (client_->ResolveDialogSubmission(handle_, event->submissionId, false))
							(void)client_->CancelDialog(handle_);
						lastResult_ = client_->LastResult();
					}
				}
				if (lastResult_ != DMUI_RESULT_OK)
				{
					if (lastResult_ == DMUI_RESULT_STALE_HANDLE ||
						lastResult_ == DMUI_RESULT_CLIENT_NOT_FOUND)
						Clear();
					return;
				}
			}
			// Consume the terminal event so the host can accept the next request.
			const auto event = client_->PollDialogEvent(handle_, text_);
			lastResult_ = client_->LastResult();
			if (event || lastResult_ == DMUI_RESULT_STALE_HANDLE ||
				lastResult_ == DMUI_RESULT_CLIENT_NOT_FOUND)
				Clear();
		}

	private:
		void Clear() noexcept
		{
			client_ = nullptr;
			handle_ = DMUI_INVALID_DIALOG_HANDLE;
			submit_ = {};
			text_.clear();
		}

		void FailAndCancel(DMUI_Result a_result) noexcept
		{
			Cancel();
			lastResult_ = a_result;
		}

		Client* client_{};
		DMUI_DialogHandle handle_{ DMUI_INVALID_DIALOG_HANDLE };
		Submit submit_;
		std::string text_;
		DMUI_Result lastResult_{ DMUI_RESULT_OK };
	};

	class FontGuard
	{
	public:
		FontGuard(Client& a_client, DMUI_FontRole a_role) noexcept :
			m_client(&a_client),
			m_pushed(a_client.PushFont(a_role)),
			m_result(a_client.LastResult())
		{}

		~FontGuard() noexcept
		{
			if (!m_pushed)
				return;
			const auto original = m_client->LastResult();
			(void)End();
			if (original != DMUI_RESULT_OK)
				m_client->SetPresentationResult(original);
		}

		FontGuard(const FontGuard&) = delete;
		FontGuard(FontGuard&&) = delete;
		FontGuard& operator=(const FontGuard&) = delete;
		FontGuard& operator=(FontGuard&&) = delete;

		[[nodiscard]] bool Pushed() const noexcept
		{
			return m_pushed;
		}

		[[nodiscard]] DMUI_Result Result() const noexcept
		{
			return m_result;
		}

		[[nodiscard]] bool End() noexcept
		{
			if (!m_pushed)
				return m_result == DMUI_RESULT_OK;
			m_pushed = false;
			const auto popped = m_client->PopFont();
			m_result = m_client->LastResult();
			return popped;
		}

	private:
		Client* m_client;
		bool m_pushed;
		DMUI_Result m_result;
	};

	class SettingsTableScope
	{
	public:
		SettingsTableScope(Client& a_client, const char* a_id) noexcept :
			m_client(&a_client)
		{
			const auto begun = a_client.BeginSettingsTable(a_id);
			m_state.result = begun ?
				DMUI_RESULT_OK :
				a_client.LastResult();
			m_state.visible = begun.value_or(false);
			m_active = m_state.result == DMUI_RESULT_OK && m_state.visible;
		}

		~SettingsTableScope() noexcept
		{
			if (!m_active)
				return;
			const auto original = m_client->LastResult();
			(void)End();
			if (original != DMUI_RESULT_OK)
				m_client->SetPresentationResult(original);
		}

		SettingsTableScope(const SettingsTableScope&) = delete;
		SettingsTableScope(SettingsTableScope&&) = delete;
		SettingsTableScope& operator=(const SettingsTableScope&) = delete;
		SettingsTableScope& operator=(SettingsTableScope&&) = delete;

		[[nodiscard]] ScopeResult State() const noexcept
		{
			return m_state;
		}

		[[nodiscard]] DMUI_Result Result() const noexcept
		{
			return m_state.result;
		}

		[[nodiscard]] bool Visible() const noexcept
		{
			return m_state.visible;
		}

		[[nodiscard]] bool End() noexcept
		{
			if (!m_active)
				return m_state.result == DMUI_RESULT_OK;
			m_active = false;
			const auto ended = m_client->EndSettingsTable();
			m_state.result = m_client->LastResult();
			return ended;
		}

	private:
		Client* m_client;
		ScopeResult m_state;
		bool m_active{};
	};

	class FieldScope
	{
	public:
		FieldScope(
			Client& a_client,
			const char* a_id,
			const char* a_label,
			const char* a_description = nullptr,
			RowPresentation::Layout a_layout =
				RowPresentation::Layout::kLabelValue) noexcept :
			m_client(&a_client)
		{
			const auto begun =
				a_client.BeginField(a_id, a_label, a_description, a_layout);
			m_state.result = begun ?
				DMUI_RESULT_OK :
				a_client.LastResult();
			m_state.visible = begun.value_or(false);
			m_active = m_state.result == DMUI_RESULT_OK && m_state.visible;
			if (m_state.result == DMUI_RESULT_OK && !m_state.visible)
				m_resetPressed = false;
		}

		~FieldScope() noexcept
		{
			if (!m_active)
				return;
			const auto original = m_client->LastResult();
			(void)End();
			if (original != DMUI_RESULT_OK)
				m_client->SetPresentationResult(original);
		}

		FieldScope(const FieldScope&) = delete;
		FieldScope(FieldScope&&) = delete;
		FieldScope& operator=(const FieldScope&) = delete;
		FieldScope& operator=(FieldScope&&) = delete;

		[[nodiscard]] ScopeResult State() const noexcept
		{
			return m_state;
		}

		[[nodiscard]] DMUI_Result Result() const noexcept
		{
			return m_state.result;
		}

		[[nodiscard]] bool Visible() const noexcept
		{
			return m_state.visible;
		}

		[[nodiscard]] bool SetFeedback(
			Severity a_severity,
			const char* a_message) noexcept
		{
			if (!m_active)
				return false;
			return m_client->SetFieldFeedback(a_severity, a_message);
		}

		[[nodiscard]] bool ClearFeedback() noexcept
		{
			if (!m_active)
				return false;
			return m_client->ClearFieldFeedback();
		}

		[[nodiscard]] std::optional<bool> End(
			bool a_resetVisible = false,
			bool a_resetEnabled = false) noexcept
		{
			if (!m_active)
			{
				if (m_state.result != DMUI_RESULT_OK)
					return std::nullopt;
				return m_resetPressed;
			}
			m_active = false;
			m_resetPressed = m_client->EndField(a_resetVisible, a_resetEnabled);
			m_state.result = m_client->LastResult();
			return m_resetPressed;
		}

	private:
		Client* m_client;
		ScopeResult m_state;
		bool m_active{};
		std::optional<bool> m_resetPressed;
	};

	// Built-in SettingsPage text from the host's own translations, with
	// English fallbacks. Resolved once, on the first draw after the host is ready.
	struct SettingsPageText
	{
		std::string resetAll;
		std::string resetAllTooltip;
		std::string revert;
		std::string revertTooltip;
		std::string apply;
		std::string applyTooltipOne;
		std::string applyTooltipMany;
		std::string modifiedOnly;
		std::string searchHint;
		std::string unsupportedControl;
		std::string appliesNow;
		std::string unavailable;
	};

	namespace settings_text_detail
	{
		struct Entry
		{
			std::string SettingsPageText::* member;
			const char* key;
			const char* english;
		};

		inline constexpr std::array kEntries{
			Entry{ &SettingsPageText::resetAll,
				"$DMUI_SettingsPage_ResetAll", "Reset all" },
			Entry{ &SettingsPageText::resetAllTooltip,
				"$DMUI_SettingsPage_ResetAllTooltip",
				"Reset loads every setting's default into the draft. "
				"Use Apply to save them." },
			Entry{ &SettingsPageText::revert,
				"$DMUI_SettingsPage_Revert", "Revert" },
			Entry{ &SettingsPageText::revertTooltip,
				"$DMUI_SettingsPage_RevertTooltip",
				"Revert discards pending edits and restores committed settings." },
			Entry{ &SettingsPageText::apply,
				"$DMUI_SettingsPage_Apply", "Apply ({})" },
			Entry{ &SettingsPageText::applyTooltipOne,
				"$DMUI_SettingsPage_ApplyTooltipOne",
				"Apply saves {} pending change." },
			Entry{ &SettingsPageText::applyTooltipMany,
				"$DMUI_SettingsPage_ApplyTooltipMany",
				"Apply saves {} pending changes." },
			Entry{ &SettingsPageText::modifiedOnly,
				"$DMUI_SettingsPage_ModifiedOnly", "Modified only" },
			Entry{ &SettingsPageText::searchHint,
				"$DMUI_SettingsPage_SearchHint", "Search settings..." },
			Entry{ &SettingsPageText::unsupportedControl,
				"$DMUI_SettingsPage_UnsupportedControl",
				"Unsupported setting control." },
			Entry{ &SettingsPageText::appliesNow,
				"$DMUI_SettingsPage_AppliesNow", "Applies now." },
			Entry{ &SettingsPageText::unavailable,
				"$DMUI_SettingsPage_Unavailable", "Unavailable" }
		};
	}

	// a_resolve(key, result) returns the translation or nullopt. A false
	// a_final means the host had no language yet, so the text is provisional.
	template <class Resolve>
	[[nodiscard]] SettingsPageText LoadSettingsPageText(
		Resolve&& a_resolve,
		bool& a_final)
	{
		SettingsPageText text;
		a_final = true;
		for (const auto& entry : settings_text_detail::kEntries)
		{
			DMUI_Result result{};
			auto value = a_resolve(entry.key, result);
			if (result == DMUI_RESULT_HOST_NOT_READY)
				a_final = false;
			text.*entry.member = value ? std::move(*value) : std::string{ entry.english };
		}
		return text;
	}

	// Render thread only.
	[[nodiscard]] inline const SettingsPageText& GetSettingsPageText()
	{
		static SettingsPageText text;
		static bool resolved{};
		if (!resolved)
		{
			text = LoadSettingsPageText(
				[](const char* a_key, DMUI_Result& a_result) {
					return Client::ResolveText("DearModdingUI", a_key, &a_result);
				},
				resolved);
		}
		return text;
	}

	[[nodiscard]] inline std::string ResolveSettingDescription(
		const SettingDescriptor& a_setting)
	{
		auto description = a_setting.resolveDescription ?
			a_setting.resolveDescription() :
			a_setting.description;
		if (a_setting.applyTiming != SettingApplyTiming::kImmediate)
			return description;
		if (!description.empty())
			description.push_back('\n');
		description += GetSettingsPageText().appliesNow;
		return description;
	}

	namespace setting_detail
	{
		// Fills the first "{}" of a translated template.
		[[nodiscard]] inline std::string FillPlaceholder(
			std::string a_template,
			std::string_view a_value)
		{
			if (const auto at = a_template.find("{}"); at != std::string::npos)
				a_template.replace(at, 2, a_value);
			return a_template;
		}

		template <NumericSettingValue T>
		[[nodiscard]] constexpr const char* DefaultNumericFormat() noexcept
		{
			if constexpr (std::same_as<T, double>)
				return "%.3f";
			else if constexpr (std::same_as<T, int64_t>)
				return "%lld";
			else
				return "%llu";
		}

		// The host rejects non-finite numbers, so they are left out of the descriptor.
		template <NumericSettingValue T>
		[[nodiscard]] bool IsUsableNumber(T a_value) noexcept
		{
			if constexpr (std::same_as<T, double>)
				return std::isfinite(a_value);
			else
				return true;
		}

		template <NumericSettingValue T>
		[[nodiscard]] T DrawNumericSetting(
			const NumericSettingControl<T>& a_control,
			T a_value,
			bool& a_changed,
			bool& a_completed) noexcept
		{
			NumericOptions<T> options{
				.id = "Value",
				.presentation = a_control.presentation,
				.value = a_value,
				.format = a_control.format.empty() ?
					DefaultNumericFormat<T>() :
					a_control.format.c_str(),
				.clamp = true,
				.dragSpeed = a_control.dragSpeed
			};
			if (a_control.range)
			{
				const auto& range = *a_control.range;
				if (range.minimum && IsUsableNumber(*range.minimum))
					options.minimum = range.minimum;
				if (range.maximum && IsUsableNumber(*range.maximum))
					options.maximum = range.maximum;
			}
			if (const auto& step = a_control.quantization;
				step && step->interval > T{} &&
				IsUsableNumber(step->interval) && IsUsableNumber(step->origin))
			{
				options.step = step->interval;
				options.stepOrigin = step->origin;
			}
			if (options.presentation == NumericPresentation::kSlider &&
				!(options.minimum && options.maximum))
				options.presentation = NumericPresentation::kDrag;
			const auto draw = ui::Numeric<T>(options);
			a_changed = draw.changed;
			a_completed = draw.completed;
			return draw.value;
		}

		[[nodiscard]] inline SettingValue AcceptSettingValue(
			const SettingDescriptor& a_setting,
			SettingValue a_value)
		{
			auto effective = a_setting.binding.set(
				NormalizeSettingValue(a_setting, std::move(a_value)));
			if (!SettingValueMatchesControl(a_setting.control, effective))
				throw std::bad_variant_access{};
			return effective;
		}

		struct BoundSettingDrawResult
		{
			SettingValue value;
			bool changed{};
			bool completed{};
		};

		inline void NotifySettingEdit(
			const SettingDescriptor& a_setting,
			const BoundSettingDrawResult& a_draw)
		{
			if (a_setting.onEdit && (a_draw.changed || a_draw.completed))
			{
				a_setting.onEdit({
					a_draw.value,
					a_draw.changed,
					a_draw.completed
				});
			}
		}

		[[nodiscard]] inline BoundSettingDrawResult DrawBoundSetting(
			const SettingDescriptor& a_setting,
			SettingValue a_value)
		{
			auto changed = false;
			auto completed = false;
			SettingValue edited = a_value;
			switch (ResolveSettingControlPresentation(a_setting.control).kind)
			{
			case SettingControlKind::kCheckbox:
			{
				const auto draw = ui::Toggle({
					.id = "Value",
					.value = std::get<bool>(a_value)
				});
				changed = draw.changed;
				completed = draw.changed;
				edited = draw.value;
				break;
			}
			case SettingControlKind::kDouble:
				edited = DrawNumericSetting(
					std::get<DoubleSettingControl>(a_setting.control),
					std::get<double>(a_value),
					changed,
					completed);
				break;
			case SettingControlKind::kSigned:
				edited = DrawNumericSetting(
					std::get<SignedSettingControl>(a_setting.control),
					std::get<int64_t>(a_value),
					changed,
					completed);
				break;
			case SettingControlKind::kUnsigned:
				edited = DrawNumericSetting(
					std::get<UnsignedSettingControl>(a_setting.control),
					std::get<uint64_t>(a_value),
					changed,
					completed);
				break;
			case SettingControlKind::kText:
			{
				const auto& control =
					std::get<TextSettingControl>(a_setting.control);
				auto text = std::get<std::string>(a_value);
				changed = ui::InputText(
					"##Value",
					text,
					{
						.maxBytes = control.maxBytes,
						.lines = control.multiline ? 3u : 0u
					});
				completed = ui::IsItemDeactivatedAfterEdit();
				if (changed)
					edited = std::move(text);
				break;
			}
			case SettingControlKind::kKeybind:
			{
				const auto& control =
					std::get<KeybindSettingControl>(a_setting.control);
				const auto& binding = std::get<std::string>(a_value);
				const auto warning = control.resolveWarning ?
					control.resolveWarning() :
					std::string{};
				const auto draw = ui::KeyCapture({
					.id = "Value",
					.binding = binding.c_str(),
					.warning = warning.empty() ? nullptr : warning.c_str(),
					.slot = control.slot,
					.allowClear = control.allowClear
				});
				if (draw.changed)
				{
					edited = draw.chord;
					changed = true;
					completed = true;
				}
				break;
			}
			case SettingControlKind::kColor:
			{
				const auto& control =
					std::get<ColorSettingControl>(a_setting.control);
				std::vector<ColorPreset> presets;
				presets.reserve(control.presets.size());
				for (const auto& preset : control.presets)
				{
					presets.emplace_back(
						preset.name.c_str(),
						preset.rgba,
						preset.description.c_str());
				}
				const auto draw = ui::ColorEdit({
					.id = "Value",
					.rgba = static_cast<uint32_t>(std::get<uint64_t>(a_value)),
					.alpha = control.alpha,
					.presets = presets,
					.presetsLabel = control.presetsLabel.c_str()
				});
				changed = draw.changed;
				completed = draw.completed;
				edited = uint64_t{ draw.rgba };
				break;
			}
			case SettingControlKind::kChoice:
			{
				const auto& control =
					std::get<ChoiceSettingControl>(a_setting.control);
				const auto& value = std::get<std::string>(a_value);
				const auto draw = ui::Choice(
					"Value",
					value,
					std::span<const ChoiceSettingOption>{ control.options },
					control.unmatchedLabel.empty() ?
						GetSettingsPageText().unavailable.c_str() :
						control.unmatchedLabel.c_str());
				if (draw.changed && draw.selected)
				{
					edited = *draw.selected;
					changed = true;
					completed = draw.completed;
				}
				break;
			}
			default:
				break;
			}
			auto result = changed ?
				AcceptSettingValue(a_setting, std::move(edited)) :
				std::move(a_value);
			return {
				std::move(result),
				changed,
				completed
			};
		}

		[[nodiscard]] inline std::string ResolveSettingLabel(
			const SettingDescriptor& a_setting)
		{
			if (a_setting.presentation.labelMode ==
				RowPresentation::LabelMode::kHidden)
				return {};
			if (a_setting.resolveLabel)
			{
				auto label = a_setting.resolveLabel();
				if (!label.empty())
					return label;
			}
			return a_setting.label.empty() ? a_setting.id : a_setting.label;
		}

		struct EvaluatedSetting
		{
			const SettingDescriptor* setting;
			std::string label;
		};

		struct EvaluatedAction
		{
			const SettingsActionRow* action;
			std::string label;
		};

		struct EvaluatedDivider
		{};

		using EvaluatedRow =
			std::variant<EvaluatedSetting, EvaluatedAction, EvaluatedDivider>;

		[[nodiscard]] inline std::string ResolveActionLabel(
			const SettingsActionRow& a_action)
		{
			if (a_action.presentation.labelMode ==
				RowPresentation::LabelMode::kHidden)
				return {};
			return a_action.label.empty() ? a_action.id : a_action.label;
		}

		[[nodiscard]] inline std::vector<EvaluatedRow> MatchingRows(
			const SettingGroup& a_group,
			const SettingFilter& a_filter)
		{
			std::vector<EvaluatedRow> matches;
			matches.reserve(a_group.rows.empty() ?
					a_group.settings.size() + a_group.actionRows.size() :
					a_group.rows.size());
			const auto evaluateSetting =
				[&](const SettingDescriptor& a_setting)
				-> std::optional<EvaluatedRow> {
				if (a_setting.isVisible && !a_setting.isVisible())
					return std::nullopt;
				auto label = ResolveSettingLabel(a_setting);
				const auto modified = a_filter.modifiedOnly &&
					a_setting.isModified &&
					a_setting.isModified();
				if (!MatchesSettingFilter(
						a_setting,
						label,
						modified,
						a_filter))
					return std::nullopt;
				return EvaluatedSetting{
					&a_setting,
					std::move(label)
				};
			};
			const auto evaluateAction =
				[&](const SettingsActionRow& a_action)
				-> std::optional<EvaluatedRow> {
				if (a_action.isVisible && !a_action.isVisible())
					return std::nullopt;
				auto label = ResolveActionLabel(a_action);
				if (!MatchesActionFilter(a_action, label, a_filter))
					return std::nullopt;
				return EvaluatedAction{
					&a_action,
					std::move(label)
				};
			};
			if (a_group.rows.empty())
			{
				for (const auto& setting : a_group.settings)
					if (auto row = evaluateSetting(setting))
						matches.push_back(std::move(*row));
				for (const auto& action : a_group.actionRows)
					if (auto row = evaluateAction(action))
						matches.push_back(std::move(*row));
				return matches;
			}
			auto pendingDivider = false;
			for (const auto& row : a_group.rows)
			{
				std::visit(
					[&](const auto& a_index) {
						using T = std::remove_cvref_t<decltype(a_index)>;
						if constexpr (std::same_as<T, SettingGroup::DividerRow>)
						{
							pendingDivider = !matches.empty();
						}
						else
						{
							std::optional<EvaluatedRow> evaluated;
							if constexpr (
								std::same_as<T, SettingGroup::SettingIndex>)
							{
								if (a_index.value < a_group.settings.size())
									evaluated = evaluateSetting(
										a_group.settings[a_index.value]);
							}
							else
							{
								if (a_index.value < a_group.actionRows.size())
									evaluated = evaluateAction(
										a_group.actionRows[a_index.value]);
							}
							if (!evaluated)
								return;
							if (pendingDivider)
								matches.emplace_back(EvaluatedDivider{});
							matches.push_back(std::move(*evaluated));
							pendingDivider = false;
						}
					},
					row);
			}
			return matches;
		}

		[[nodiscard]] inline size_t MatchingContentCount(
			const std::vector<EvaluatedRow>& a_rows) noexcept
		{
			return static_cast<size_t>(std::ranges::count_if(
				a_rows,
				[](const EvaluatedRow& a_row) {
					return !std::holds_alternative<EvaluatedDivider>(a_row);
				}));
		}

		[[nodiscard]] inline std::optional<bool> EndSettingRow(
			const SettingDescriptor& a_setting,
			FieldScope& a_row,
			bool a_resetVisible = false,
			bool a_resetEnabled = false)
		{
			if (a_setting.resolveFeedback)
			{
				auto feedback = a_setting.resolveFeedback();
				if (feedback && !feedback->message.empty() &&
					!a_row.SetFeedback(
						feedback->severity,
						feedback->message.c_str()))
					return std::nullopt;
			}
			return a_row.End(a_resetVisible, a_resetEnabled);
		}

		[[nodiscard]] inline bool EndFallbackRow(
			const SettingDescriptor& a_setting,
			FieldScope& a_row)
		{
			{
				const DisabledScope disabled;
				ui::TextUnformatted(GetSettingsPageText().unsupportedControl.c_str());
			}
			return EndSettingRow(a_setting, a_row).has_value();
		}

		[[nodiscard]] inline bool DrawSettingRow(
			Client& a_client,
			const EvaluatedSetting& a_evaluated)
		{
			const auto& setting = *a_evaluated.setting;
			const auto description = ResolveSettingDescription(setting);
			FieldScope row{
				a_client,
				setting.id.c_str(),
				a_evaluated.label.c_str(),
				description.c_str(),
				setting.presentation.layout
			};
			if (row.Result() != DMUI_RESULT_OK)
				return false;
			if (!row.Visible())
				return true;

			const auto presentation =
				ResolveSettingControlPresentation(setting.control);
			if (!presentation.supported)
				return EndFallbackRow(setting, row);

			const auto enabled =
				!setting.isEnabled || setting.isEnabled();
			if (presentation.kind == SettingControlKind::kReadOnly)
			{
				const auto& control =
					std::get<ReadOnlySettingControl>(setting.control);
				if (!control.draw)
					return EndFallbackRow(setting, row);
				{
					const DisabledScope disabled{ !enabled };
					control.draw();
				}
				return EndSettingRow(setting, row).has_value();
			}

			if (!setting.binding.get ||
				!setting.binding.set ||
				!SettingValueMatchesControl(
					setting.control,
					setting.defaultValue))
				return EndFallbackRow(setting, row);

			auto value = setting.binding.get();
			if (!SettingValueMatchesControl(setting.control, value))
				throw std::bad_variant_access{};
			{
				const DisabledScope disabled{ !enabled };
				auto draw = DrawBoundSetting(setting, std::move(value));
				setting_detail::NotifySettingEdit(setting, draw);
				value = std::move(draw.value);
			}

			const auto resetVisible =
				setting.showReset && presentation.resetVisible;
			const auto modified = setting.isModified ?
				setting.isModified() :
				!IsSettingDefault(setting, value);
			const auto resetEnabled =
				resetVisible && enabled && modified;
			const auto reset = EndSettingRow(
				setting,
				row,
				resetVisible,
				resetEnabled);
			if (!reset)
				return false;
			if (*reset)
				(void)ResetSettingToDefault(setting);
			return true;
		}

		[[nodiscard]] inline bool DrawActionRow(
			Client& a_client,
			const EvaluatedAction& a_evaluated)
		{
			const auto& action = *a_evaluated.action;
			FieldScope row{
				a_client,
				action.id.c_str(),
				a_evaluated.label.c_str(),
				action.description.c_str(),
				action.presentation.layout
			};
			if (row.Result() != DMUI_RESULT_OK)
				return false;
			if (!row.Visible())
				return true;
			const auto enabled =
				action.activate && (!action.isEnabled || action.isEnabled());
			const auto& label =
				action.buttonLabel.empty() ? action.label : action.buttonLabel;
			if (ui::Button({
					.id = action.id.c_str(),
					.label = label.c_str(),
					.enabled = enabled
				}) && enabled)
			{
				try
				{
					action.activate();
				}
				catch (...)
				{}
			}
			return row.End(false, false).has_value();
		}

		// Mirrors the host section rhythm rather than plain item spacing.
		[[nodiscard]] inline bool SectionSpacing(Client& a_client) noexcept
		{
			const auto style = a_client.GetStyleMetrics();
			if (!style)
				return false;
			const auto cursor = ui::GetCursorScreenPos();
			ui::SetCursorScreenPos({
				cursor.x,
				cursor.y + style->sectionGap - 2.0f * style->itemSpacing.y
			});
			ui::Dummy({});
			return true;
		}

		[[nodiscard]] inline bool DrawSettingGroup(
			Client& a_client,
			SettingGroup& a_group,
			const SettingFilter& a_filter)
		{
			auto matches = MatchingRows(a_group, a_filter);
			if (matches.empty())
				return true;

			const auto key = a_group.id.empty() ? a_group.label : a_group.id;
			// A divider heading is a section without title, icon, or collapse state.
			const auto divider =
				a_group.headingMode == SettingGroup::HeadingMode::kDivider;
			const auto label = a_group.label.empty() ? key : a_group.label;
			ui::SectionScope section{ {
				.id = key.c_str(),
				.title = divider ? nullptr : label.c_str(),
				.glyph = divider ? PhosphorGlyph::kNone : a_group.glyph,
				.expanded = divider ? nullptr : &a_group.expanded,
				.count = divider ?
					std::nullopt :
					std::optional<uint32_t>{
						static_cast<uint32_t>(MatchingContentCount(matches))
					}
			} };
			if (!section)
				return true;

			const auto tableId = "##dmui.settings.table." + key;
			SettingsTableScope table{ a_client, tableId.c_str() };
			if (table.Result() != DMUI_RESULT_OK)
				return false;
			if (!table.Visible())
				return true;
			for (const auto& row : matches)
			{
				const auto drawn = std::visit(
					[&](const auto& a_evaluated) {
						using T = std::remove_cvref_t<decltype(a_evaluated)>;
						if constexpr (std::same_as<T, EvaluatedSetting>)
							return DrawSettingRow(a_client, a_evaluated);
						else if constexpr (std::same_as<T, EvaluatedAction>)
							return DrawActionRow(a_client, a_evaluated);
						else
						{
							ui::TableNextRow();
							(void)ui::TableSetColumnIndex(0);
							ui::Separator();
							return true;
						}
					},
					row);
				if (!drawn)
					return false;
			}
			if (!table.End())
				return false;
			section.End();
			return SectionSpacing(a_client);
		}

		[[nodiscard]] inline bool DrawPageHeader(
			Client& a_client,
			SettingsPage& a_page)
		{
			enum class PageAction
			{
				kReset,
				kRevert,
				kApply
			};
			struct Action
			{
				PageAction action;
				const char* id;
				char32_t glyph;
				std::string label;
				const char* widthLabel;
				std::string tooltip;
				ButtonStyle style;
				bool enabled;
			};

			const auto& text = GetSettingsPageText();
			const auto pending = a_page.PendingCount();
			const auto pendingText = std::to_string(pending);
			const auto widestApplyLabel = FillPlaceholder(text.apply, "112");
			const auto dirty = pending != 0;
			std::array<Action, 3> actions{};
			size_t actionCount{};
			if (a_page.actions.showReset || a_page.actions.reset)
			{
				actions[actionCount++] = {
					PageAction::kReset,
					"##dmui.settings.reset",
					PhosphorGlyph::kArrowsClockwise,
					text.resetAll,
					text.resetAll.c_str(),
					a_page.actionTooltips.reset.empty() ?
						text.resetAllTooltip :
						a_page.actionTooltips.reset,
					ButtonStyle::kDefault,
					true
				};
			}
			if (a_page.actions.revert)
			{
				actions[actionCount++] = {
					PageAction::kRevert,
					"##dmui.settings.revert",
					PhosphorGlyph::kArrowCounterClockwise,
					text.revert,
					text.revert.c_str(),
					a_page.actionTooltips.revert.empty() ?
						text.revertTooltip :
						a_page.actionTooltips.revert,
					ButtonStyle::kDefault,
					dirty
				};
			}
			if (a_page.actions.apply)
			{
				actions[actionCount++] = {
					PageAction::kApply,
					"##dmui.settings.apply",
					PhosphorGlyph::kFloppyDisk,
					FillPlaceholder(text.apply, pendingText),
					widestApplyLabel.c_str(),
					a_page.actionTooltips.apply ?
						a_page.actionTooltips.apply(pending) :
						FillPlaceholder(
							pending == 1 ?
								text.applyTooltipOne :
								text.applyTooltipMany,
							pendingText),
					ButtonStyle::kPrimary,
					dirty
				};
			}

			const auto drawFilter =
				a_page.filterOptions.showSearch ||
				a_page.filterOptions.showModifiedOnly;
			if (actionCount == 0 && !drawFilter)
				return true;

			const auto start = ui::GetCursorScreenPos();
			const auto available =
				(std::max)(ui::GetContentRegionAvail().x, 0.0f);
			const auto style = a_client.GetStyleMetrics();
			if (!style)
				return false;
			const auto spacing = style->itemSpacing.x;
			const auto itemSpacingY = style->itemSpacing.y;
			const auto itemInnerSpacingX = style->itemInnerSpacing.x;
			const auto frameHeight = ui::GetFrameHeight();
			auto buttonHeight = frameHeight;
			std::array<float, actions.size()> widths{};
			for (size_t index = 0; index < actionCount; ++index)
			{
				// The widest label reserves the width so Apply never resizes.
				const auto size = ui::MeasureButton({
					.id = actions[index].id,
					.label = actions[index].widthLabel,
					.glyph = actions[index].glyph,
					.style = actions[index].style
				});
				widths[index] = size.x;
				buttonHeight = (std::max)(buttonHeight, size.y);
			}
			const auto widthSum =
				std::accumulate(widths.begin(), widths.end(), 0.0f);
			const auto layout = ResolvePageActionRowLayout(
				start.x,
				start.x + available,
				widthSum,
				actionCount,
				spacing);
			auto rowHeight = buttonHeight;
			const auto filterWidth =
				(std::max)(layout.titleMaxX - start.x, 0.0f);

			auto modifiedInline = true;
			if (a_page.filterOptions.showSearch)
			{
				const auto modifiedWidth =
					a_page.filterOptions.showModifiedOnly ?
					frameHeight +
						itemInnerSpacingX +
						ui::CalcTextSize(text.modifiedOnly.c_str()).x :
					0.0f;
				const auto minimumSearchWidth = ui::GetFontSize() * 10.0f;
				const auto searchWidth = (std::max)(
					minimumSearchWidth,
					(std::min)(
						ui::GetFontSize() * 24.0f,
						filterWidth - modifiedWidth - spacing));
				(void)ui::InputText(
					"##dmui.settings.search",
					a_page.filter.search,
					{
						.hint = a_page.filterOptions.searchHint.empty() ?
							text.searchHint.c_str() :
							a_page.filterOptions.searchHint.c_str(),
						.style = ui::TextFieldStyle::kSearch,
						.size = { (std::min)(searchWidth, filterWidth), 0.0f }
					});
				// The host search frame can be taller than a plain frame.
				rowHeight = (std::max)(
					rowHeight,
					ui::GetCursorScreenPos().y - start.y - itemSpacingY);
				if (a_page.filterOptions.showModifiedOnly)
				{
					modifiedInline =
						searchWidth + modifiedWidth + spacing <= filterWidth;
					if (modifiedInline)
						ui::SameLine();
					else
					{
						ui::SetCursorScreenPos({
							start.x,
							start.y + rowHeight + itemSpacingY
						});
					}
				}
			}
			if (a_page.filterOptions.showModifiedOnly)
			{
				const auto modified = ui::Toggle({
					.id = "dmui.settings.modified",
					.label = text.modifiedOnly.c_str(),
					.value = a_page.filter.modifiedOnly
				});
				if (modified.changed)
					a_page.filter.modifiedOnly = modified.value;
			}

			std::optional<PageAction> pressed;
			auto actionX = layout.actionsMinX;
			for (size_t index = 0; index < actionCount; ++index)
			{
				const auto& action = actions[index];
				ui::SetCursorScreenPos({
					actionX,
					start.y + RowContentOffsetY(
						rowHeight,
						{ buttonHeight },
						RowContentMetric::kBox)
				});
				if (ui::Button({
						.id = action.id,
						.label = action.label.c_str(),
						.glyph = action.glyph,
						.tooltip = action.tooltip.c_str(),
						.enabled = action.enabled,
						.style = action.style,
						.size = { widths[index], buttonHeight }
					}))
					pressed = action.action;
				actionX += widths[index] + spacing;
			}

			auto consumedHeight = rowHeight;
			if (a_page.filterOptions.showSearch &&
				a_page.filterOptions.showModifiedOnly &&
				!modifiedInline)
				consumedHeight += itemSpacingY + frameHeight;
			ui::SetCursorScreenPos(start);
			ui::Dummy({ available, consumedHeight });
			if (!SectionSpacing(a_client))
				return false;

			if (!pressed)
				return true;
			switch (*pressed)
			{
			case PageAction::kReset:
				if (a_page.actions.reset)
					a_page.actions.reset();
				else
					a_page.ResetToDefaults();
				break;
			case PageAction::kRevert:
				a_page.actions.revert();
				break;
			case PageAction::kApply:
				a_page.actions.apply();
				break;
			}
			return true;
		}

		[[nodiscard]] inline bool DrawPageNotes(
			Client& a_client,
			const SettingsPage& a_page) noexcept
		{
			for (const auto& note : a_page.notes)
				ui::Text(note.text, TextStyle{}.Tone(note.tone), { .wrap = true });
			return a_page.notes.empty() || SectionSpacing(a_client);
		}
	}

	inline void SettingsPage::Draw(Client& a_client)
	{
		if (prepare)
			prepare();
		if (prepareView)
			prepareView(*this);
		if (!setting_detail::DrawPageHeader(a_client, *this))
			return;
		if (!setting_detail::DrawPageNotes(a_client, *this))
			return;
		for (auto& group : groups)
		{
			if (!setting_detail::DrawSettingGroup(
					a_client,
					group,
					filter))
				return;
		}
	}

	[[nodiscard]] constexpr ui::Vec4 ToUIVec4(DMUI_Vec4 a_color) noexcept
	{
		return { a_color.x, a_color.y, a_color.z, a_color.w };
	}
}
