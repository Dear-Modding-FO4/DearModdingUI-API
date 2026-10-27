#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace dmui
{
	enum class RowContentMetric : uint32_t
	{
		kBox,
		kOptical
	};

	struct RowContentMetrics
	{
		float height{ 0.0f };
		float opticalMinY{ 0.0f };
		float opticalMaxY{ 0.0f };
		float opticalScale{ 0.0f };
	};

	[[nodiscard]] constexpr float RowContentOffsetY(
		float a_rowHeight,
		const RowContentMetrics& a_content,
		RowContentMetric a_metric) noexcept
	{
		const auto rowHeight = (std::max)(a_rowHeight, 0.0f);
		const auto contentHeight = (std::max)(a_content.height, 0.0f);
		const auto boxOffset = (std::max)(
			(rowHeight - contentHeight) * 0.5f,
			0.0f);
		if (a_metric != RowContentMetric::kOptical ||
			a_rowHeight <= 0.0f ||
			a_content.height <= 0.0f ||
			a_content.opticalMaxY <= a_content.opticalMinY ||
			a_content.opticalScale <= 0.0f)
			return boxOffset;
		return (std::max)(
			rowHeight * 0.5f -
				(a_content.opticalMinY + a_content.opticalMaxY) *
					a_content.opticalScale * 0.5f,
			0.0f);
	}

	struct PageActionRowLayout
	{
		float titleMinX{ 0.0f };
		float titleMaxX{ 0.0f };
		float actionsMinX{ 0.0f };
		float actionsMaxX{ 0.0f };
		float reservedWidth{ 0.0f };
	};

	[[nodiscard]] constexpr PageActionRowLayout ResolvePageActionRowLayout(
		float a_contentMinX,
		float a_contentMaxX,
		float a_buttonWidthSum,
		size_t a_actionCount,
		float a_spacing) noexcept
	{
		const auto contentMax = a_contentMaxX > a_contentMinX ?
			a_contentMaxX :
			a_contentMinX;
		if (a_actionCount == 0)
		{
			return {
				a_contentMinX,
				contentMax,
				contentMax,
				contentMax,
				0.0f
			};
		}
		const auto spacing = a_spacing > 0.0f ? a_spacing : 0.0f;
		const auto buttonWidth = a_buttonWidthSum > 0.0f ? a_buttonWidthSum : 0.0f;
		const auto requestedWidth =
			buttonWidth + spacing * static_cast<float>(a_actionCount - 1);
		const auto actionsMin = contentMax - requestedWidth;
		const auto unboundedTitleMax = actionsMin - spacing;
		const auto titleMax = unboundedTitleMax > a_contentMinX ?
			unboundedTitleMax :
			a_contentMinX;
		return {
			a_contentMinX,
			titleMax,
			actionsMin,
			contentMax,
			requestedWidth
		};
	}
}
