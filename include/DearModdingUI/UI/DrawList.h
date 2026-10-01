#pragma once

#include <DearModdingUI/UI.h>
#include <span>

namespace dmui::ui
{
	class DrawList
	{
	public:
		explicit constexpr DrawList(DrawTarget a_target) noexcept :
			m_target(static_cast<DMUI_DrawTarget>(a_target)) {}

		void AddLine(Vec2 a_p1, Vec2 a_p2, Color32 a_color, float a_thickness = 1.0f) const noexcept
		{
			detail::Record(checked::DrawListAddLine(m_target, a_p1, a_p2, a_color, a_thickness));
		}
		void AddRect(Vec2 a_min, Vec2 a_max, Color32 a_color, float a_rounding = 0, float a_thickness = 1) const noexcept
		{
			detail::Record(checked::DrawListAddRect(m_target, a_min, a_max, a_color, a_rounding, a_thickness));
		}
		void AddRectFilled(Vec2 a_min, Vec2 a_max, Color32 a_color, float a_rounding = 0) const noexcept
		{
			detail::Record(checked::DrawListAddRectFilled(m_target, a_min, a_max, a_color, a_rounding));
		}
		void AddCircle(Vec2 a_center, float a_radius, Color32 a_color, uint32_t a_segments = 0, float a_thickness = 1) const noexcept
		{
			detail::Record(checked::DrawListAddCircle(m_target, a_center, a_radius, a_color, a_segments, a_thickness));
		}
		void AddCircleFilled(Vec2 a_center, float a_radius, Color32 a_color, uint32_t a_segments = 0) const noexcept
		{
			detail::Record(checked::DrawListAddCircleFilled(m_target, a_center, a_radius, a_color, a_segments));
		}
		void AddTriangle(Vec2 a_p1, Vec2 a_p2, Vec2 a_p3, Color32 a_color, float a_thickness = 1) const noexcept
		{
			detail::Record(checked::DrawListAddTriangle(m_target, a_p1, a_p2, a_p3, a_color, a_thickness));
		}
		void AddTriangleFilled(Vec2 a_p1, Vec2 a_p2, Vec2 a_p3, Color32 a_color) const noexcept
		{
			detail::Record(checked::DrawListAddTriangleFilled(m_target, a_p1, a_p2, a_p3, a_color));
		}
		void AddBezierCubic(Vec2 a_p1, Vec2 a_p2, Vec2 a_p3, Vec2 a_p4, Color32 a_color,
			float a_thickness = 1, uint32_t a_segments = 0) const noexcept
		{
			detail::Record(checked::DrawListAddBezierCubic(m_target, a_p1, a_p2, a_p3, a_p4, a_color, a_thickness, a_segments));
		}
		void AddPolyline(const Vec2* a_points, uint32_t a_count, Color32 a_color, bool a_closed = false, float a_thickness = 1) const noexcept
		{
			detail::Record(checked::DrawListAddPolyline(m_target, a_points, a_count, a_color, a_closed ? 1u : 0u, a_thickness));
		}
		void AddPolyline(std::span<const Vec2> a_points, Color32 a_color, bool a_closed = false, float a_thickness = 1) const noexcept
		{
			if (ValidCount(a_points.size()))
				AddPolyline(a_points.data(), static_cast<uint32_t>(a_points.size()), a_color, a_closed, a_thickness);
		}
		void AddPolygonFilled(const Vec2* a_points, uint32_t a_count, Color32 a_color) const noexcept
		{
			detail::Record(checked::DrawListAddPolygonFilled(m_target, a_points, a_count, a_color));
		}
		void AddPolygonFilled(std::span<const Vec2> a_points, Color32 a_color) const noexcept
		{
			if (ValidCount(a_points.size()))
				AddPolygonFilled(a_points.data(), static_cast<uint32_t>(a_points.size()), a_color);
		}
		void AddText(Vec2 a_pos, Color32 a_color, std::string_view a_text, float a_fontSize = 0) const noexcept
		{
			detail::Record(checked::DrawListAddText(m_target, a_pos, a_color, detail::TextData(a_text), a_text.size(), a_fontSize));
		}
		void AddImage(ImageHandle a_image, Vec2 a_min, Vec2 a_max, Vec2 a_uv0 = {}, Vec2 a_uv1 = { 1, 1 },
			Color32 a_tint = 0xFFFFFFFFu) const noexcept
		{
			detail::Record(checked::DrawListAddImage(m_target, a_image.value, a_min, a_max, a_uv0, a_uv1, a_tint));
		}
		[[nodiscard]] bool PushClipRect(Vec2 a_min, Vec2 a_max, bool a_intersectWithCurrent = true) const noexcept
		{
			const auto result = checked::DrawListPushClipRect(m_target, a_min, a_max, a_intersectWithCurrent ? 1u : 0u);
			detail::Record(result);
			return result == DMUI_RESULT_OK;
		}
		void PopClipRect() const noexcept
		{
			detail::Record(checked::DrawListPopClipRect(m_target));
		}

	private:
		static bool ValidCount(size_t a_count) noexcept
		{
			if (a_count <= (std::numeric_limits<uint32_t>::max)())
				return true;
			detail::Record(DMUI_RESULT_INVALID_ARGUMENT);
			return false;
		}
		DMUI_DrawTarget m_target;
	};

	[[nodiscard]] inline DrawList WindowDrawList() noexcept { return DrawList{ DrawTarget::kWindow }; }
	[[nodiscard]] inline DrawList ForegroundDrawList() noexcept { return DrawList{ DrawTarget::kForeground }; }
	[[nodiscard]] inline DrawList BackgroundDrawList() noexcept { return DrawList{ DrawTarget::kBackground }; }

	class ClipRectScope
	{
	public:
		ClipRectScope(DrawList a_list, Vec2 a_min, Vec2 a_max, bool a_intersectWithCurrent = true) noexcept :
			m_list(a_list), m_active(a_list.PushClipRect(a_min, a_max, a_intersectWithCurrent)) {}
		~ClipRectScope() noexcept
		{
			if (m_active)
				m_list.PopClipRect();
		}
		ClipRectScope(const ClipRectScope&) = delete;
		ClipRectScope& operator=(const ClipRectScope&) = delete;

	private:
		DrawList m_list;
		bool m_active;
	};
}
