#pragma once

// Include Dear ImGui first for lockstep mode; forwarding mode needs no ImGui sources.

#if defined(IMGUI_VERSION) && defined(IMGUI_VERSION_NUM)
#include <DearModdingUI/ImGuiFingerprint.h>
#else
#include <DearModdingUI/ImGuiForward.h>
#endif

#include <DearModdingUI/SettingsActions.h>
#include <DearModdingUI/VisualDecisions.h>
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
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace dmui
{
	struct Version
	{
		uint16_t major{};
		uint16_t minor{};

		[[nodiscard]] constexpr uint32_t Pack() const noexcept
		{
			return DMUI_MAKE_VERSION(major, minor);
		}
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

	struct ForwardingClientTag
	{};

	inline constexpr ForwardingClientTag kForwardingClient{};

	using SettingValue = std::variant<bool, double, int64_t, uint64_t, std::string>;

	template <class T>
	concept SettingValueAlternative =
		std::same_as<std::remove_cvref_t<T>, bool> ||
		std::same_as<std::remove_cvref_t<T>, double> ||
		std::same_as<std::remove_cvref_t<T>, int64_t> ||
		std::same_as<std::remove_cvref_t<T>, uint64_t> ||
		std::same_as<std::remove_cvref_t<T>, std::string>;

	template <class T>
	concept NumericSettingValue =
		std::same_as<std::remove_cvref_t<T>, double> ||
		std::same_as<std::remove_cvref_t<T>, int64_t> ||
		std::same_as<std::remove_cvref_t<T>, uint64_t>;

	enum class SettingApplyTiming : uint8_t
	{
		kImmediate,
		kNextLaunch
	};

	template <NumericSettingValue T>
	struct NumericSettingRange
	{
		std::optional<T> minimum;
		std::optional<T> maximum;
	};

	template <NumericSettingValue T>
	struct NumericQuantization
	{
		T interval;
		T origin;
	};

	struct CheckboxSettingControl
	{};

	template <NumericSettingValue T>
	struct NumericSettingControl
	{
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
		size_t bufferCapacity{ 512 };
	};

	struct ChoiceSettingOption
	{
		std::string value;
		std::string label;
	};

	[[nodiscard]] inline const std::string& ResolveChoiceOptionLabel(
		const ChoiceSettingOption& a_option) noexcept
	{
		return a_option.label.empty() ? a_option.value : a_option.label;
	}

	struct ChoiceSettingControl
	{
		std::vector<ChoiceSettingOption> options;
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
		switch (a_control.index())
		{
		case 0:
			return { SettingControlKind::kCheckbox, true, true, true };
		case 1:
			return { SettingControlKind::kDouble, true, true, true };
		case 2:
			return { SettingControlKind::kSigned, true, true, true };
		case 3:
			return { SettingControlKind::kUnsigned, true, true, true };
		case 4:
			return { SettingControlKind::kText, true, true, true };
		case 5:
			return { SettingControlKind::kChoice, true, true, true };
		case 6:
			return { SettingControlKind::kReadOnly, true, false, false };
		default:
			return {};
		}
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
			return std::holds_alternative<uint64_t>(a_value);
		case SettingControlKind::kText:
		case SettingControlKind::kChoice:
			return std::holds_alternative<std::string>(a_value);
		default:
			return false;
		}
	}

	enum class NumericSettingWidget : uint8_t
	{
		kInput,
		kDrag,
		kSlider
	};

	template <NumericSettingValue T>
	[[nodiscard]] constexpr NumericSettingWidget ResolveNumericSettingWidget(
		const NumericSettingControl<T>& a_control) noexcept
	{
		if (!a_control.range)
			return NumericSettingWidget::kInput;
		if (a_control.range->minimum && a_control.range->maximum)
			return NumericSettingWidget::kSlider;
		return NumericSettingWidget::kDrag;
	}

	template <NumericSettingValue T>
	[[nodiscard]] inline T ClampSettingNumber(
		T a_value,
		T a_default,
		const std::optional<NumericSettingRange<T>>& a_range) noexcept
	{
		if constexpr (std::same_as<T, double>)
		{
			if (!std::isfinite(a_value))
				a_value = std::isfinite(a_default) ? a_default : 0.0;
		}
		if (!a_range)
			return a_value;

		auto minimum = a_range->minimum;
		auto maximum = a_range->maximum;
		if constexpr (std::same_as<T, double>)
		{
			if (minimum && !std::isfinite(*minimum))
				minimum.reset();
			if (maximum && !std::isfinite(*maximum))
				maximum.reset();
		}
		if (minimum && maximum && *maximum < *minimum)
			std::swap(minimum, maximum);
		if (minimum)
			a_value = (std::max)(a_value, *minimum);
		if (maximum)
			a_value = (std::min)(a_value, *maximum);
		return a_value;
	}

	template <NumericSettingValue T>
	[[nodiscard]] inline T QuantizeSettingNumber(
		T a_value,
		const std::optional<NumericQuantization<T>>& a_quantization) noexcept
	{
		if (!a_quantization)
			return a_value;
		const auto interval = a_quantization->interval;
		const auto origin = a_quantization->origin;
		if constexpr (std::same_as<T, double>)
		{
			if (!std::isfinite(a_value) ||
				!std::isfinite(interval) ||
				!std::isfinite(origin) ||
				interval <= 0.0)
				return a_value;
			const auto quantized =
				origin + std::round((a_value - origin) / interval) * interval;
			return std::isfinite(quantized) ? quantized : a_value;
		}
		else
		{
			if (interval <= T{})
				return a_value;
			using Unsigned = std::make_unsigned_t<T>;
			const auto unsignedInterval = static_cast<Unsigned>(interval);
			const auto above = a_value >= origin;
			const auto distance = above ?
				static_cast<Unsigned>(a_value) - static_cast<Unsigned>(origin) :
				static_cast<Unsigned>(origin) - static_cast<Unsigned>(a_value);
			auto steps = distance / unsignedInterval;
			const auto remainder = distance % unsignedInterval;
			if (remainder >= unsignedInterval - remainder)
				++steps;
			const auto room = above ?
				static_cast<Unsigned>((std::numeric_limits<T>::max)()) -
					static_cast<Unsigned>(origin) :
				static_cast<Unsigned>(origin) -
					static_cast<Unsigned>((std::numeric_limits<T>::min)());
			if (steps > room / unsignedInterval)
			{
				return above ?
					(std::numeric_limits<T>::max)() :
					(std::numeric_limits<T>::min)();
			}
			const auto offset = steps * unsignedInterval;
			return above ?
				static_cast<T>(static_cast<Unsigned>(origin) + offset) :
				static_cast<T>(static_cast<Unsigned>(origin) - offset);
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

	inline void DrawDivider() noexcept
	{
		ImGui::Separator();
	}

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
		bool showReset{ true };
		RowPresentation presentation;
		std::function<std::string()> resolveDescription;
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
		std::string searchHint{ "Search settings..." };
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
		bool muted{};
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
		description += "Applies now.";
		return description;
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
		return a_value;
	}

	[[nodiscard]] inline std::optional<SettingValue> ResetSettingToDefault(
		const SettingDescriptor& a_setting)
	{
		const auto presentation =
			ResolveSettingControlPresentation(a_setting.control);
		if (!presentation.editable ||
			!a_setting.binding.set ||
			!SettingValueMatchesControl(
				a_setting.control,
				a_setting.defaultValue))
			return std::nullopt;

		auto effective = a_setting.binding.set(
			NormalizeSettingValue(a_setting, a_setting.defaultValue));
		if (!SettingValueMatchesControl(a_setting.control, effective))
			throw std::bad_variant_access{};
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

	// IMPORTANT: Client instances must outlive the game session because DMUI v1 cannot unregister callbacks.
	class Client
	{
	public:
#if defined(IMGUI_VERSION) && defined(IMGUI_VERSION_NUM)
		Client(
			std::string_view a_id,
			std::string_view a_displayName,
			Version a_version,
			std::string_view a_iconName = {}) :
			id_(a_id),
			displayName_(a_displayName),
			iconName_(a_iconName),
			version_(a_version),
			fingerprint_(DMUI_MakeImGuiFingerprint())
		{}
#endif

		Client(
			std::string_view a_id,
			std::string_view a_displayName,
			Version a_version,
			ForwardingClientTag,
			std::string_view a_iconName = {}) :
			id_(a_id),
			displayName_(a_displayName),
			iconName_(a_iconName),
			version_(a_version)
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

			const auto getHostAPI = FindHostAPI();
			hostPresent_ = getHostAPI != nullptr;
			if (!getHostAPI)
			{
				api_ = nullptr;
				lastResult_ = DMUI_RESULT_OK;
				return false;
			}

			api_ = getHostAPI(DMUI_API_VERSION_CURRENT);
			if (!api_)
			{
				lastResult_ = DMUI_RESULT_UNSUPPORTED_ABI;
				return false;
			}
			if (api_->structSize < kRegisterClientSize || !api_->registerClient)
			{
				lastResult_ = DMUI_RESULT_STRUCT_TOO_SMALL;
				return false;
			}

			DMUI_ClientDescriptor descriptor{};
			descriptor.structSize = sizeof(descriptor);
			descriptor.apiVersion = DMUI_API_VERSION_CURRENT;
			descriptor.id = id_.c_str();
			descriptor.displayName = displayName_.c_str();
			descriptor.version = version_.Pack();
			descriptor.expectedImGui = fingerprint_ ? &*fingerprint_ : nullptr;
			descriptor.onHostReady = &OnHostReady;
			descriptor.onHostUnavailable = &OnHostUnavailable;
			descriptor.userData = this;
			descriptor.capabilities = DMUI_CLIENT_CAPABILITY_NONE;
			descriptor.iconName =
				iconName_.empty() ? nullptr : iconName_.c_str();

			DMUI_ClientHandle handle{ DMUI_INVALID_CLIENT_HANDLE };
			lastResult_ = api_->registerClient(&descriptor, &handle);
			if (lastResult_ != DMUI_RESULT_OK)
				return false;

			clientHandle_ = handle;
			return true;
		}

		template <class Callable>
			requires std::invocable<std::decay_t<Callable>&>
		[[nodiscard]] std::optional<DMUI_PageHandle> AddPage(
			const char* a_id,
			const char* a_displayName,
			const char* a_category,
			Callable&& a_draw,
			const char* a_summary = nullptr,
			int32_t a_sortKey = 0,
			DMUI_PageKind a_kind = DMUI_PAGE_KIND_SETTINGS) noexcept
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

				pages_.push_back({ DMUI_INVALID_PAGE_HANDLE, std::move(callback) });
				auto& registration = pages_.back();

				DMUI_PageDescriptor descriptor{};
				descriptor.structSize = sizeof(descriptor);
				descriptor.id = a_id;
				descriptor.displayName = a_displayName;
				descriptor.category = a_category;
				descriptor.summary = a_summary;
				descriptor.sortKey = a_sortKey;
				descriptor.kind = a_kind;
				descriptor.draw = &Invoke;
				descriptor.userData = &registration.callback;

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

		template <class Page>
			requires std::same_as<std::remove_cvref_t<Page>, SettingsPage>
		[[nodiscard]] std::optional<DMUI_PageHandle> AddSettingsPage(
			const char* a_id,
			const char* a_displayName,
			const char* a_category,
			Page&& a_page,
			const char* a_summary = nullptr,
			int32_t a_sortKey = 0) noexcept
		{
			try
			{
				SettingsPage page{ std::forward<Page>(a_page) };
				return AddPage(
					a_id,
					a_displayName,
					a_category,
					[this, page = std::move(page)]() mutable {
						page.Draw(*this);
					},
					a_summary,
					a_sortKey,
					DMUI_PAGE_KIND_SETTINGS);
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
			Callable&& a_callback) noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}
			if (api_->structSize < DMUI_HOST_API_REGISTER_HOTKEY_ACTION_SIZE ||
				!api_->registerHotkeyAction)
			{
				Fail(DMUI_RESULT_UNSUPPORTED_ABI);
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
				descriptor.structSize = sizeof(descriptor);
				descriptor.id = a_id;
				descriptor.displayName = a_displayName;
				descriptor.suggestedDefaultChord = a_suggestedDefaultChord;
				descriptor.callback = &InvokeHotkey;
				descriptor.userData = registration.callback.get();

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
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}
			if (api_->structSize < DMUI_HOST_API_QUERY_HOTKEY_BINDING_SIZE ||
				!api_->queryHotkeyBinding)
			{
				Fail(DMUI_RESULT_UNSUPPORTED_ABI);
				return std::nullopt;
			}
			DMUI_HotkeyBindingInfo binding{};
			binding.structSize = sizeof(binding);
			lastResult_ = api_->queryHotkeyBinding(clientHandle_, a_action, &binding);
			if (lastResult_ != DMUI_RESULT_OK)
				return std::nullopt;
			return binding;
		}

		bool UnregisterHotkeyAction(DMUI_HotkeyActionHandle a_action) noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			if (api_->structSize < DMUI_HOST_API_UNREGISTER_HOTKEY_ACTION_SIZE ||
				!api_->unregisterHotkeyAction)
				return Fail(DMUI_RESULT_UNSUPPORTED_ABI);
			const auto registration = std::ranges::find(
				hotkeyActions_, a_action, &HotkeyActionRegistration::handle);
			if (registration == hotkeyActions_.end())
				return Fail(DMUI_RESULT_ACTION_NOT_FOUND);
			lastResult_ = api_->unregisterHotkeyAction(clientHandle_, a_action);
			if (lastResult_ != DMUI_RESULT_OK)
				return false;
			auto* callback = registration->callback.release();
			hotkeyActions_.erase(registration);
			ReleaseHotkeyCallback(callback);
			return true;
		}

		template <class Callable>
			requires std::invocable<std::decay_t<Callable>&>
		bool AddAction(
			const char* a_id,
			const char* a_label,
			const char* a_iconName,
			const char* a_tooltip,
			Callable&& a_callback,
			int32_t a_sortKey = 0) noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			if (api_->structSize < DMUI_HOST_API_REGISTER_ACTION_SIZE || !api_->registerAction)
				return Fail(DMUI_RESULT_UNSUPPORTED_ABI);

			try
			{
				std::function<void()> callback{ std::forward<Callable>(a_callback) };
				if (!callback)
					return Fail(DMUI_RESULT_INVALID_ARGUMENT);

				actions_.push_back({ DMUI_INVALID_ACTION_HANDLE, std::move(callback) });
				auto& registration = actions_.back();

				DMUI_ActionDescriptor descriptor{};
				descriptor.structSize = sizeof(descriptor);
				descriptor.id = a_id;
				descriptor.displayLabel = a_label;
				descriptor.iconName = a_iconName;
				descriptor.tooltip = a_tooltip;
				descriptor.sortKey = a_sortKey;
				descriptor.callback = &Invoke;
				descriptor.userData = &registration.callback;

				DMUI_ActionHandle handle{ DMUI_INVALID_ACTION_HANDLE };
				lastResult_ = api_->registerAction(clientHandle_, &descriptor, &handle);
				if (lastResult_ != DMUI_RESULT_OK)
				{
					actions_.pop_back();
					return false;
				}

				registration.handle = handle;
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
			if (api_->structSize < DMUI_HOST_API_REGISTER_FRAME_OBSERVER_SIZE ||
				!api_->registerFrameObserver)
			{
				Fail(DMUI_RESULT_UNSUPPORTED_ABI);
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
				descriptor.structSize = sizeof(descriptor);
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
			if (api_->structSize <
					DMUI_HOST_API_REGISTER_PAGE_ACTIVITY_OBSERVER_SIZE ||
				!api_->registerPageActivityObserver)
			{
				Fail(DMUI_RESULT_UNSUPPORTED_ABI);
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
				descriptor.structSize = sizeof(descriptor);
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
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}
			if (api_->structSize < DMUI_HOST_API_QUERY_VIDEO_MEMORY_SIZE ||
				!api_->queryVideoMemory)
			{
				Fail(DMUI_RESULT_UNSUPPORTED_ABI);
				return std::nullopt;
			}

			VideoMemoryInfo info{};
			lastResult_ =
				api_->queryVideoMemory(clientHandle_, &info.used, &info.budget);
			if (lastResult_ != DMUI_RESULT_OK)
				return std::nullopt;
			return info;
		}

		bool SetStatus(DMUI_StatusSeverity a_severity, const char* a_message) noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			if (api_->structSize < DMUI_HOST_API_SET_STATUS_SIZE || !api_->setStatus)
				return Fail(DMUI_RESULT_UNSUPPORTED_ABI);

			lastResult_ = api_->setStatus(clientHandle_, a_severity, a_message);
			return lastResult_ == DMUI_RESULT_OK;
		}

		[[nodiscard]] std::optional<DMUI_ThemeColors> GetThemeColors() noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}
			if (api_->structSize < DMUI_HOST_API_GET_THEME_COLORS_SIZE || !api_->getThemeColors)
			{
				Fail(DMUI_RESULT_UNSUPPORTED_ABI);
				return std::nullopt;
			}

			DMUI_ThemeColors colors{};
			colors.structSize = sizeof(colors);
			lastResult_ = api_->getThemeColors(clientHandle_, &colors);
			if (lastResult_ != DMUI_RESULT_OK)
				return std::nullopt;
			return colors;
		}

		[[nodiscard]] bool PushFont(DMUI_FontRole a_role) noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			if (api_->structSize < DMUI_HOST_API_PUSH_FONT_SIZE || !api_->pushFont)
				return Fail(DMUI_RESULT_UNSUPPORTED_ABI);

			lastResult_ = api_->pushFont(clientHandle_, a_role);
			return lastResult_ == DMUI_RESULT_OK;
		}

		[[nodiscard]] bool PopFont() noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			if (api_->structSize < DMUI_HOST_API_POP_FONT_SIZE || !api_->popFont)
				return Fail(DMUI_RESULT_UNSUPPORTED_ABI);

			lastResult_ = api_->popFont(clientHandle_);
			return lastResult_ == DMUI_RESULT_OK;
		}

		[[nodiscard]] bool DrawSectionHeader(const char* a_text, char32_t a_glyph = 0) noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			if (api_->structSize < DMUI_HOST_API_DRAW_SECTION_HEADER_SIZE ||
				!api_->drawSectionHeader)
				return Fail(DMUI_RESULT_UNSUPPORTED_ABI);

			lastResult_ = api_->drawSectionHeader(
				clientHandle_,
				a_text,
				static_cast<uint32_t>(a_glyph));
			return lastResult_ == DMUI_RESULT_OK;
		}

		[[nodiscard]] bool DrawBulletText(const char* a_text) noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			if (api_->structSize < DMUI_HOST_API_DRAW_BULLET_TEXT_SIZE ||
				!api_->drawBulletText)
				return Fail(DMUI_RESULT_UNSUPPORTED_ABI);

			lastResult_ = api_->drawBulletText(clientHandle_, a_text);
			return lastResult_ == DMUI_RESULT_OK;
		}

		[[nodiscard]] std::optional<bool> DrawSearchInput(
			const char* a_id,
			const char* a_hint,
			std::string& a_search) noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}
			if (api_->structSize < DMUI_HOST_API_DRAW_SEARCH_INPUT_SIZE ||
				!api_->drawSearchInput)
			{
				Fail(DMUI_RESULT_UNSUPPORTED_ABI);
				return std::nullopt;
			}

			try
			{
				const auto capacity = a_search.size() < 255 ? 256 : a_search.size() + 1;
				std::vector<char> buffer(capacity);
				std::copy(a_search.begin(), a_search.end(), buffer.begin());
				uint32_t changed{};
				lastResult_ = api_->drawSearchInput(
					clientHandle_,
					a_id,
					a_hint,
					buffer.data(),
					buffer.size(),
					&changed);
				if (lastResult_ != DMUI_RESULT_OK)
					return std::nullopt;
				if (changed)
					a_search.assign(buffer.data());
				return changed != 0;
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

		[[nodiscard]] bool DrawCollapsingSectionHeader(
			const char* a_key,
			const char* a_text,
			char32_t a_glyph,
			bool& a_expanded,
			size_t a_count) noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			if (api_->structSize < DMUI_HOST_API_DRAW_COLLAPSING_SECTION_HEADER_SIZE ||
				!api_->drawCollapsingSectionHeader)
				return Fail(DMUI_RESULT_UNSUPPORTED_ABI);

			uint32_t expanded = a_expanded ? 1u : 0u;
			lastResult_ = api_->drawCollapsingSectionHeader(
				clientHandle_,
				a_key,
				a_text,
				static_cast<uint32_t>(a_glyph),
				&expanded,
				a_count);
			if (lastResult_ != DMUI_RESULT_OK)
				return false;
			a_expanded = expanded != 0;
			return true;
		}

		[[nodiscard]] std::optional<bool> DrawSettingsActionButton(
			const char* a_id,
			const ImVec2& a_origin,
			const ImVec2& a_size,
			DMUI_SettingsAction a_action,
			const char* a_fallbackLabel,
			const char* a_tooltip,
			bool a_enabled) noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}
			if (api_->structSize < DMUI_HOST_API_DRAW_SETTINGS_ACTION_BUTTON_SIZE ||
				!api_->drawSettingsActionButton)
			{
				Fail(DMUI_RESULT_UNSUPPORTED_ABI);
				return std::nullopt;
			}

			uint32_t pressed{};
			lastResult_ = api_->drawSettingsActionButton(
				clientHandle_,
				a_id,
				{ a_origin.x, a_origin.y },
				{ a_size.x, a_size.y },
				a_action,
				a_fallbackLabel,
				a_tooltip,
				a_enabled ? 1u : 0u,
				&pressed);
			if (lastResult_ != DMUI_RESULT_OK)
				return std::nullopt;
			return pressed != 0;
		}

		[[nodiscard]] std::optional<float> SettingsActionButtonWidth(
			DMUI_SettingsAction a_action,
			const char* a_fallbackLabel,
			float a_buttonExtent) noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}
			if (api_->structSize < DMUI_HOST_API_SETTINGS_ACTION_BUTTON_WIDTH_SIZE ||
				!api_->settingsActionButtonWidth)
			{
				Fail(DMUI_RESULT_UNSUPPORTED_ABI);
				return std::nullopt;
			}

			float width{};
			lastResult_ = api_->settingsActionButtonWidth(
				clientHandle_,
				a_action,
				a_fallbackLabel,
				a_buttonExtent,
				&width);
			if (lastResult_ != DMUI_RESULT_OK)
				return std::nullopt;
			return width;
		}

		[[nodiscard]] std::optional<float> SettingsActionButtonExtent() noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}
			if (api_->structSize < DMUI_HOST_API_SETTINGS_ACTION_BUTTON_EXTENT_SIZE ||
				!api_->settingsActionButtonExtent)
			{
				Fail(DMUI_RESULT_UNSUPPORTED_ABI);
				return std::nullopt;
			}

			float extent{};
			lastResult_ = api_->settingsActionButtonExtent(clientHandle_, &extent);
			if (lastResult_ != DMUI_RESULT_OK)
				return std::nullopt;
			return extent;
		}

		[[nodiscard]] std::optional<bool> BeginSettingsTable(const char* a_id) noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}
			if (api_->structSize < DMUI_HOST_API_END_SETTINGS_TABLE_SIZE ||
				!api_->beginSettingsTable ||
				!api_->endSettingsTable)
			{
				Fail(DMUI_RESULT_UNSUPPORTED_ABI);
				return std::nullopt;
			}

			uint32_t visible{};
			lastResult_ = api_->beginSettingsTable(clientHandle_, a_id, &visible);
			if (lastResult_ != DMUI_RESULT_OK)
				return std::nullopt;
			return visible != 0;
		}

		[[nodiscard]] std::optional<bool> BeginSettingsRow(
			const char* a_id,
			const char* a_label,
			const char* a_description,
			RowPresentation::Layout a_layout =
				RowPresentation::Layout::kLabelValue) noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}
			if (api_->structSize < DMUI_HOST_API_END_SETTINGS_ROW_SIZE ||
				!api_->endSettingsRow)
			{
				Fail(DMUI_RESULT_UNSUPPORTED_ABI);
				return std::nullopt;
			}

			uint32_t visible{};
			if (api_->structSize >= DMUI_HOST_API_BEGIN_SETTINGS_ROW_EX_SIZE &&
				api_->beginSettingsRowEx)
			{
				const DMUI_SettingsRowBeginOptions options{
					sizeof(DMUI_SettingsRowBeginOptions),
					a_layout == RowPresentation::Layout::kFullSpan ?
						DMUI_SETTINGS_ROW_LAYOUT_FULL_SPAN :
						DMUI_SETTINGS_ROW_LAYOUT_LABEL_VALUE
				};
				lastResult_ = api_->beginSettingsRowEx(
					clientHandle_,
					a_id,
					a_label,
					a_description,
					&options,
					&visible);
			}
			else if (api_->beginSettingsRow)
			{
				lastResult_ = api_->beginSettingsRow(
					clientHandle_,
					a_id,
					a_label,
					a_description,
					&visible);
			}
			else
			{
				Fail(DMUI_RESULT_UNSUPPORTED_ABI);
				return std::nullopt;
			}
			if (lastResult_ != DMUI_RESULT_OK)
				return std::nullopt;
			return visible != 0;
		}

		[[nodiscard]] std::optional<bool> EndSettingsRow(
			bool a_resetVisible,
			bool a_resetEnabled) noexcept
		{
			if (!IsConnected())
			{
				Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
				return std::nullopt;
			}
			if (api_->structSize < DMUI_HOST_API_END_SETTINGS_ROW_SIZE ||
				!api_->endSettingsRow)
			{
				Fail(DMUI_RESULT_UNSUPPORTED_ABI);
				return std::nullopt;
			}

			const DMUI_SettingsRowOptions options{
				sizeof(DMUI_SettingsRowOptions),
				a_resetVisible ? 1u : 0u,
				a_resetEnabled ? 1u : 0u
			};
			uint32_t resetPressed{};
			lastResult_ = api_->endSettingsRow(
				clientHandle_,
				&options,
				&resetPressed);
			if (lastResult_ != DMUI_RESULT_OK)
				return std::nullopt;
			return resetPressed != 0;
		}

		[[nodiscard]] bool EndSettingsTable() noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			if (api_->structSize < DMUI_HOST_API_END_SETTINGS_TABLE_SIZE ||
				!api_->endSettingsTable)
				return Fail(DMUI_RESULT_UNSUPPORTED_ABI);

			lastResult_ = api_->endSettingsTable(clientHandle_);
			return lastResult_ == DMUI_RESULT_OK;
		}

		[[nodiscard]] bool SelectPage(DMUI_PageHandle a_page) noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			if (api_->structSize < DMUI_HOST_API_SELECT_PAGE_SIZE || !api_->selectPage)
				return Fail(DMUI_RESULT_UNSUPPORTED_ABI);

			lastResult_ = api_->selectPage(clientHandle_, a_page);
			return lastResult_ == DMUI_RESULT_OK;
		}

		[[nodiscard]] std::optional<bool> IsMenuVisible() noexcept
		{
			if (!api_)
			{
				Fail(DMUI_RESULT_HOST_NOT_INITIALIZED);
				return std::nullopt;
			}
			if (api_->structSize < kIsMenuVisibleSize || !api_->isMenuVisible)
			{
				Fail(DMUI_RESULT_STRUCT_TOO_SMALL);
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
			if (api_->structSize < kQueryStateSize || !api_->queryState)
			{
				Fail(DMUI_RESULT_STRUCT_TOO_SMALL);
				return std::nullopt;
			}

			DMUI_HostStateInfo state{};
			state.structSize = sizeof(state);
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
		using GetHostAPIFn = const DMUI_HostAPI* (DMUI_CALL*)(uint32_t) noexcept;

		struct PageRegistration
		{
			DMUI_PageHandle handle;
			std::function<void()> callback;
		};

		struct ActionRegistration
		{
			DMUI_ActionHandle handle;
			std::function<void()> callback;
		};

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

		struct HotkeyCallbackState
		{
			std::atomic<uint32_t> references{ 1 };
			std::function<void(bool)> callback;
		};

		struct HotkeyActionRegistration
		{
			DMUI_HotkeyActionHandle handle;
			std::unique_ptr<HotkeyCallbackState> callback;
		};

		static constexpr uint32_t kRegisterClientSize =
			static_cast<uint32_t>(offsetof(DMUI_HostAPI, registerClient) + sizeof(DMUI_RegisterClientFn));
		static constexpr uint32_t kRegisterPageSize =
			static_cast<uint32_t>(offsetof(DMUI_HostAPI, registerPage) + sizeof(DMUI_RegisterPageFn));
		static constexpr uint32_t kQueryStateSize =
			static_cast<uint32_t>(offsetof(DMUI_HostAPI, queryState) + sizeof(DMUI_QueryStateFn));
		static constexpr uint32_t kIsMenuVisibleSize =
			static_cast<uint32_t>(offsetof(DMUI_HostAPI, isMenuVisible) + sizeof(DMUI_IsMenuVisibleFn));

		[[nodiscard]] static GetHostAPIFn FindHostAPI() noexcept
		{
			return detail::ResolveHostSymbol<GetHostAPIFn>("DMUI_GetHostAPI");
		}

		[[nodiscard]] bool CanRegisterPage() noexcept
		{
			if (!IsConnected())
				return Fail(DMUI_RESULT_CLIENT_NOT_FOUND);
			if (api_->structSize < kRegisterPageSize || !api_->registerPage)
				return Fail(DMUI_RESULT_STRUCT_TOO_SMALL);
			return true;
		}

		bool Fail(DMUI_Result a_result) noexcept
		{
			lastResult_ = a_result;
			return false;
		}

		static void DMUI_CALL OnHostReady(
			const DMUI_HostReadyInfo* a_info,
			void* a_userData) noexcept
		{
			if (!a_info || a_info->structSize < sizeof(DMUI_HostReadyInfo))
				return;
#if defined(IMGUI_VERSION) && defined(IMGUI_VERSION_NUM)
			const auto* self = static_cast<const Client*>(a_userData);
			if (!self || !self->fingerprint_)
				return;
			ImGui::SetCurrentContext(static_cast<ImGuiContext*>(a_info->imguiContext));
			ImGui::SetAllocatorFunctions(
				a_info->imguiAlloc,
				a_info->imguiFree,
				a_info->imguiAllocatorUserData);
#else
			(void)a_userData;
#endif
		}

		static void DMUI_CALL OnHostUnavailable(
			DMUI_UnavailableReason a_reason,
			void* a_userData) noexcept
		{
			if (auto* const self = static_cast<Client*>(a_userData))
				self->unavailableReason_.store(a_reason, std::memory_order_relaxed);
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
				a_info->structSize < DMUI_PAGE_ACTIVITY_INFO_1_0_SIZE ||
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
			ReleaseHotkeyCallback(state);
		}

		static void ReleaseHotkeyCallback(HotkeyCallbackState* a_state) noexcept
		{
			if (a_state &&
				a_state->references.fetch_sub(1, std::memory_order_acq_rel) == 1)
				delete a_state;
		}

		std::string id_;
		std::string displayName_;
		std::string iconName_;
		Version version_;
		std::optional<DMUI_ImGuiFingerprint> fingerprint_;
		const DMUI_HostAPI* api_{};
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

	namespace setting_detail
	{
		template <NumericSettingValue T>
		[[nodiscard]] constexpr ImGuiDataType NumericDataType() noexcept
		{
			if constexpr (std::same_as<T, double>)
				return ImGuiDataType_Double;
			else if constexpr (std::same_as<T, int64_t>)
				return ImGuiDataType_S64;
			else
				return ImGuiDataType_U64;
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

		template <NumericSettingValue T>
		[[nodiscard]] constexpr float DefaultDragSpeed() noexcept
		{
			return std::same_as<T, double> ? 0.01f : 1.0f;
		}

		template <NumericSettingValue T>
		[[nodiscard]] T DrawNumericSetting(
			const NumericSettingControl<T>& a_control,
			T a_value,
			T a_default,
			bool& a_changed) noexcept
		{
			auto edited = a_value;
			const auto type = NumericDataType<T>();
			const auto* format = a_control.format.empty() ?
				DefaultNumericFormat<T>() :
				a_control.format.c_str();
			switch (ResolveNumericSettingWidget(a_control))
			{
			case NumericSettingWidget::kInput:
				a_changed = ImGui::InputScalar(
					"##Value",
					type,
					&edited,
					nullptr,
					nullptr,
					format);
				break;
			case NumericSettingWidget::kDrag:
			{
				const auto* minimum =
					a_control.range && a_control.range->minimum ?
					&*a_control.range->minimum :
					nullptr;
				const auto* maximum =
					a_control.range && a_control.range->maximum ?
					&*a_control.range->maximum :
					nullptr;
				a_changed = ImGui::DragScalar(
					"##Value",
					type,
					&edited,
					a_control.dragSpeed > 0.0f ?
						a_control.dragSpeed :
						DefaultDragSpeed<T>(),
					minimum,
					maximum,
					format,
					ImGuiSliderFlags_AlwaysClamp);
				break;
			}
			case NumericSettingWidget::kSlider:
			{
				auto minimum = *a_control.range->minimum;
				auto maximum = *a_control.range->maximum;
				if (maximum < minimum)
					std::swap(minimum, maximum);
				a_changed = ImGui::SliderScalar(
					"##Value",
					type,
					&edited,
					&minimum,
					&maximum,
					format,
					ImGuiSliderFlags_AlwaysClamp);
				break;
			}
			}
			if (!a_changed)
				return a_value;
			edited = QuantizeSettingNumber(edited, a_control.quantization);
			return ClampSettingNumber(edited, a_default, a_control.range);
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

		[[nodiscard]] inline SettingValue DrawBoundSetting(
			const SettingDescriptor& a_setting,
			SettingValue a_value)
		{
			auto changed = false;
			SettingValue edited = a_value;
			switch (ResolveSettingControlPresentation(a_setting.control).kind)
			{
			case SettingControlKind::kCheckbox:
			{
				auto value = std::get<bool>(a_value);
				changed = ImGui::Checkbox("##Value", &value);
				edited = value;
				break;
			}
			case SettingControlKind::kDouble:
				edited = DrawNumericSetting(
					std::get<DoubleSettingControl>(a_setting.control),
					std::get<double>(a_value),
					std::get<double>(a_setting.defaultValue),
					changed);
				break;
			case SettingControlKind::kSigned:
				edited = DrawNumericSetting(
					std::get<SignedSettingControl>(a_setting.control),
					std::get<int64_t>(a_value),
					std::get<int64_t>(a_setting.defaultValue),
					changed);
				break;
			case SettingControlKind::kUnsigned:
				edited = DrawNumericSetting(
					std::get<UnsignedSettingControl>(a_setting.control),
					std::get<uint64_t>(a_value),
					std::get<uint64_t>(a_setting.defaultValue),
					changed);
				break;
			case SettingControlKind::kText:
			{
				const auto& control =
					std::get<TextSettingControl>(a_setting.control);
				const auto& value = std::get<std::string>(a_value);
				const auto capacity = (std::max)(
					(std::max)(control.bufferCapacity, size_t{ 2 }),
					value.size() + 1);
				std::vector<char> buffer(capacity);
				std::copy(value.begin(), value.end(), buffer.begin());
				changed = ImGui::InputText(
					"##Value",
					buffer.data(),
					buffer.size());
				if (changed)
					edited = std::string{ buffer.data() };
				break;
			}
			case SettingControlKind::kChoice:
			{
				const auto& control =
					std::get<ChoiceSettingControl>(a_setting.control);
				const auto& value = std::get<std::string>(a_value);
				const auto selectedOption = std::ranges::find(
					control.options,
					value,
					&ChoiceSettingOption::value);
				const auto& previewLabel = selectedOption == control.options.end() ?
					value :
					ResolveChoiceOptionLabel(*selectedOption);
				if (ImGui::BeginCombo("##Value", previewLabel.c_str()))
				{
					for (const auto& option : control.options)
					{
						const auto selected = option.value == value;
						const auto& visibleLabel =
							ResolveChoiceOptionLabel(option);
						const auto itemLabel =
							visibleLabel + "###" + option.value;
						if (ImGui::Selectable(itemLabel.c_str(), selected))
						{
							edited = option.value;
							changed = option.value != value;
						}
						if (selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				break;
			}
			default:
				break;
			}
			return changed ?
				AcceptSettingValue(a_setting, std::move(edited)) :
				a_value;
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

		class SettingsRowBracket
		{
		public:
			explicit SettingsRowBracket(Client& a_client) noexcept :
				m_client(a_client)
			{}

			~SettingsRowBracket() noexcept
			{
				if (m_active)
					(void)m_client.EndSettingsRow(false, false);
			}

			[[nodiscard]] std::optional<bool> End(
				bool a_resetVisible,
				bool a_resetEnabled) noexcept
			{
				m_active = false;
				return m_client.EndSettingsRow(
					a_resetVisible,
					a_resetEnabled);
			}

			SettingsRowBracket(const SettingsRowBracket&) = delete;
			SettingsRowBracket(SettingsRowBracket&&) = delete;

		private:
			Client& m_client;
			bool m_active{ true };
		};

		class SettingsTableBracket
		{
		public:
			explicit SettingsTableBracket(Client& a_client) noexcept :
				m_client(a_client)
			{}

			~SettingsTableBracket() noexcept
			{
				if (m_active)
					(void)m_client.EndSettingsTable();
			}

			[[nodiscard]] bool End() noexcept
			{
				m_active = false;
				return m_client.EndSettingsTable();
			}

			SettingsTableBracket(const SettingsTableBracket&) = delete;
			SettingsTableBracket(SettingsTableBracket&&) = delete;

		private:
			Client& m_client;
			bool m_active{ true };
		};

		class DisabledScope
		{
		public:
			explicit DisabledScope(bool a_disabled = true) noexcept
			{
				ImGui::BeginDisabled(a_disabled);
			}

			~DisabledScope() noexcept
			{
				ImGui::EndDisabled();
			}

			DisabledScope(const DisabledScope&) = delete;
			DisabledScope(DisabledScope&&) = delete;
		};

		[[nodiscard]] inline bool EndFallbackRow(
			SettingsRowBracket& a_row)
		{
			{
				const DisabledScope disabled;
				ImGui::TextUnformatted("Unsupported setting control.");
			}
			return a_row.End(false, false).has_value();
		}

		[[nodiscard]] inline bool DrawSettingRow(
			Client& a_client,
			const EvaluatedSetting& a_evaluated)
		{
			const auto& setting = *a_evaluated.setting;
			const auto description = ResolveSettingDescription(setting);
			const auto row = a_client.BeginSettingsRow(
				setting.id.c_str(),
				a_evaluated.label.c_str(),
				description.c_str(),
				setting.presentation.layout);
			if (!row)
				return false;
			if (!*row)
				return true;
			SettingsRowBracket rowBracket{ a_client };

			const auto presentation =
				ResolveSettingControlPresentation(setting.control);
			if (!presentation.supported)
				return EndFallbackRow(rowBracket);

			const auto enabled =
				!setting.isEnabled || setting.isEnabled();
			if (presentation.kind == SettingControlKind::kReadOnly)
			{
				const auto& control =
					std::get<ReadOnlySettingControl>(setting.control);
				if (!control.draw)
					return EndFallbackRow(rowBracket);
				{
					const DisabledScope disabled{ !enabled };
					control.draw();
				}
				return rowBracket.End(false, false).has_value();
			}

			if (!setting.binding.get ||
				!setting.binding.set ||
				!SettingValueMatchesControl(
					setting.control,
					setting.defaultValue))
				return EndFallbackRow(rowBracket);

			auto value = setting.binding.get();
			if (!SettingValueMatchesControl(setting.control, value))
				throw std::bad_variant_access{};
			{
				const DisabledScope disabled{ !enabled };
				value = DrawBoundSetting(setting, std::move(value));
			}

			const auto resetVisible =
				setting.showReset && presentation.resetVisible;
			const auto modified = setting.isModified ?
				setting.isModified() :
				!IsSettingDefault(setting, value);
			const auto resetEnabled =
				resetVisible && enabled && modified;
			const auto reset =
				rowBracket.End(resetVisible, resetEnabled);
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
			const auto row = a_client.BeginSettingsRow(
				action.id.c_str(),
				a_evaluated.label.c_str(),
				action.description.c_str(),
				action.presentation.layout);
			if (!row)
				return false;
			if (!*row)
				return true;
			SettingsRowBracket rowBracket{ a_client };
			const auto enabled =
				action.activate && (!action.isEnabled || action.isEnabled());
			{
				const DisabledScope disabled{ !enabled };
				const auto& label =
					action.buttonLabel.empty() ? action.label : action.buttonLabel;
				const auto itemLabel = label + "###" + action.id;
				if (ImGui::Button(itemLabel.c_str()) && enabled)
				{
					try
					{
						action.activate();
					}
					catch (...)
					{}
				}
			}
			return rowBracket.End(false, false).has_value();
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
			if (a_group.headingMode == SettingGroup::HeadingMode::kDivider)
			{
				DrawDivider();
			}
			else
			{
				const auto label = a_group.label.empty() ? key : a_group.label;
				const auto glyph = a_group.glyph ?
					a_group.glyph :
					DearModdingUI::ResolveIconGlyph(
						DearModdingUI::IconKind::kCategory,
						label);
				if (!a_client.DrawCollapsingSectionHeader(
						key.c_str(),
						label.c_str(),
						glyph,
						a_group.expanded,
						MatchingContentCount(matches)))
					return false;
				if (!a_group.expanded)
					return true;
			}

			const auto tableId = "##dmui.settings.table." + key;
			const auto table = a_client.BeginSettingsTable(tableId.c_str());
			if (!table)
				return false;
			if (!*table)
				return true;
			SettingsTableBracket tableBracket{ a_client };
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
							ImGui::TableNextRow();
							(void)ImGui::TableSetColumnIndex(0);
							DrawDivider();
							return true;
						}
					},
					row);
				if (!drawn)
					return false;
			}
			if (!tableBracket.End())
				return false;
			ImGui::Spacing();
			return true;
		}

		[[nodiscard]] inline bool DrawPageHeader(
			Client& a_client,
			SettingsPage& a_page)
		{
			struct Action
			{
				DearModdingUI::SettingsAction action;
				const char* id;
				std::string fallbackLabel;
				const char* widthLabel;
				std::string tooltip;
			};

			const auto pending = a_page.PendingCount();
			const auto dirty = pending != 0;
			std::array<Action, 3> actions{};
			size_t actionCount{};
			if (a_page.actions.showReset || a_page.actions.reset)
			{
				actions[actionCount++] = {
					DearModdingUI::SettingsAction::kReset,
					"##dmui.settings.reset",
					"Reset all",
					"Reset all",
					a_page.actionTooltips.reset.empty() ?
						"Reset loads every setting's default into the draft. "
						"Use Apply to save them." :
						a_page.actionTooltips.reset
				};
			}
			if (a_page.actions.revert)
			{
				actions[actionCount++] = {
					DearModdingUI::SettingsAction::kRevert,
					"##dmui.settings.revert",
					"Revert",
					"Revert",
					a_page.actionTooltips.revert.empty() ?
						"Revert discards pending edits and restores committed settings." :
						a_page.actionTooltips.revert
				};
			}
			if (a_page.actions.apply)
			{
				actions[actionCount++] = {
					DearModdingUI::SettingsAction::kApply,
					"##dmui.settings.apply",
					"Apply (" + std::to_string(pending) + ")",
					"Apply (112)",
					a_page.actionTooltips.apply ?
						a_page.actionTooltips.apply(pending) :
						"Apply saves " + std::to_string(pending) + " pending " +
							(pending == 1 ? "change." : "changes.")
				};
			}

			const auto drawFilter =
				a_page.filterOptions.showSearch ||
				a_page.filterOptions.showModifiedOnly;
			if (actionCount == 0 && !drawFilter)
				return true;

			const auto start = ImGui::GetCursorScreenPos();
			const auto available =
				(std::max)(ImGui::GetContentRegionAvail().x, 0.0f);
			float spacing{};
			float itemSpacingY{};
			float itemInnerSpacingX{};
#if defined(IMGUI_VERSION) && defined(IMGUI_VERSION_NUM)
			const auto& style = ImGui::GetStyle();
			spacing = style.ItemSpacing.x;
			itemSpacingY = style.ItemSpacing.y;
			itemInnerSpacingX = style.ItemInnerSpacing.x;
#else
			DMUI_StyleMetrics style{};
			if (ImGui::GetStyleMetrics(style) != DMUI_RESULT_OK)
				return false;
			spacing = style.itemSpacing.x;
			itemSpacingY = style.itemSpacing.y;
			itemInnerSpacingX = style.itemInnerSpacing.x;
#endif
			const auto frameHeight = ImGui::GetFrameHeight();
			auto buttonExtent = frameHeight;
			std::array<float, actions.size()> widths{};
			if (actionCount != 0)
			{
				const auto extent = a_client.SettingsActionButtonExtent();
				if (!extent)
					return false;
				buttonExtent = *extent;
				for (size_t index = 0; index < actionCount; ++index)
				{
					const auto width = a_client.SettingsActionButtonWidth(
						static_cast<DMUI_SettingsAction>(actions[index].action),
						actions[index].widthLabel,
						buttonExtent);
					if (!width)
						return false;
					widths[index] = *width;
				}
			}
			const auto widthSum =
				DearModdingUI::ResolveSettingsActionButtonWidthSum(
					widths,
					dirty,
					pending);
			const auto layout = DearModdingUI::ResolvePageActionRowLayout(
				start.x,
				start.x + available,
				widthSum,
				actionCount,
				spacing);
			const auto rowHeight = (std::max)(frameHeight, buttonExtent);
			const auto filterWidth =
				(std::max)(layout.titleMaxX - start.x, 0.0f);

			auto modifiedInline = true;
			if (a_page.filterOptions.showSearch)
			{
				std::array<char, 256> search{};
				const auto length = (std::min)(
					a_page.filter.search.size(),
					search.size() - 1);
				std::copy_n(
					a_page.filter.search.data(),
					length,
					search.data());
				const auto modifiedWidth =
					a_page.filterOptions.showModifiedOnly ?
					frameHeight +
						itemInnerSpacingX +
						ImGui::CalcTextSize("Modified only").x :
					0.0f;
				const auto minimumSearchWidth = ImGui::GetFontSize() * 10.0f;
				const auto searchWidth = (std::max)(
					minimumSearchWidth,
					(std::min)(
						ImGui::GetFontSize() * 24.0f,
						filterWidth - modifiedWidth - spacing));
				ImGui::SetNextItemWidth((std::min)(searchWidth, filterWidth));
				if (ImGui::InputTextWithHint(
						"##dmui.settings.search",
						a_page.filterOptions.searchHint.c_str(),
						search.data(),
						search.size()))
					a_page.filter.search = search.data();
				if (a_page.filterOptions.showModifiedOnly)
				{
					modifiedInline =
						searchWidth + modifiedWidth + spacing <= filterWidth;
					if (modifiedInline)
						ImGui::SameLine();
					else
					{
						ImGui::SetCursorScreenPos({
							start.x,
							start.y + rowHeight + itemSpacingY
						});
					}
				}
			}
			if (a_page.filterOptions.showModifiedOnly)
			{
				(void)ImGui::Checkbox(
					"Modified only###dmui.settings.modified",
					&a_page.filter.modifiedOnly);
			}

			std::optional<DearModdingUI::SettingsAction> pressed;
			auto actionX = layout.actionsMinX;
			for (size_t index = 0; index < actionCount; ++index)
			{
				const auto& action = actions[index];
				const auto selected = a_client.DrawSettingsActionButton(
					action.id,
					{
						actionX,
						start.y + DearModdingUI::RowContentOffsetY(
							rowHeight,
							{ buttonExtent },
							DearModdingUI::RowContentMetric::kBox)
					},
					{ widths[index], buttonExtent },
					static_cast<DMUI_SettingsAction>(action.action),
					action.fallbackLabel.c_str(),
					action.tooltip.c_str(),
					DearModdingUI::SettingsActionEnabled(
						action.action,
						dirty));
				if (!selected)
					return false;
				if (*selected)
					pressed = action.action;
				actionX += widths[index] + spacing;
			}

			auto consumedHeight = rowHeight;
			if (a_page.filterOptions.showSearch &&
				a_page.filterOptions.showModifiedOnly &&
				!modifiedInline)
				consumedHeight += itemSpacingY + frameHeight;
			ImGui::SetCursorScreenPos(start);
			ImGui::Dummy({ available, consumedHeight });
			ImGui::Spacing();

			if (!pressed)
				return true;
			switch (*pressed)
			{
			case DearModdingUI::SettingsAction::kReset:
				if (a_page.actions.reset)
					a_page.actions.reset();
				else
					a_page.ResetToDefaults();
				break;
			case DearModdingUI::SettingsAction::kRevert:
				a_page.actions.revert();
				break;
			case DearModdingUI::SettingsAction::kApply:
				a_page.actions.apply();
				break;
			}
			return true;
		}

		inline void DrawPageNotes(const SettingsPage& a_page) noexcept
		{
			for (const auto& note : a_page.notes)
			{
				if (note.muted)
					ImGui::TextDisabled("%s", note.text.c_str());
				else
					ImGui::TextWrapped("%s", note.text.c_str());
			}
			if (!a_page.notes.empty())
				ImGui::Spacing();
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
		setting_detail::DrawPageNotes(*this);
		for (auto& group : groups)
		{
			if (!setting_detail::DrawSettingGroup(
					a_client,
					group,
					filter))
				return;
		}
	}

	class FontGuard
	{
	public:
		FontGuard(Client& a_client, DMUI_FontRole a_role) noexcept :
			m_client(&a_client),
			m_pushed(a_client.PushFont(a_role))
		{}

		~FontGuard() noexcept
		{
			if (m_pushed)
				(void)m_client->PopFont();
		}

		FontGuard(const FontGuard&) = delete;
		FontGuard(FontGuard&&) = delete;
		FontGuard& operator=(const FontGuard&) = delete;
		FontGuard& operator=(FontGuard&&) = delete;

	private:
		Client* m_client;
		bool m_pushed;
	};

	[[nodiscard]] constexpr ImVec4 ToImVec4(DMUI_Vec4 a_color) noexcept
	{
		return { a_color.x, a_color.y, a_color.z, a_color.w };
	}
}
