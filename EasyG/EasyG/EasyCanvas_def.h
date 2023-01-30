#pragma once

#include "EasyImage.h"

_EASYG_BEGIN

class EasyCanvas : public EasyImage2
{
public:
	bool IsFlush = true;

	template<class ...Args>
	EasyCanvas(Args&&... args) noexcept;
	EasyCanvas(const EasyCanvas&) noexcept;
	EasyCanvas(EasyCanvas&&) noexcept;

	template<class ...Args>
	EasyCanvas& WINAPI operator=(Args&&... args) noexcept;
	EasyCanvas& WINAPI operator=(const EasyCanvas&) noexcept;
	EasyCanvas& WINAPI operator=(EasyCanvas&&) noexcept;

	EasyCanvas WINAPI Clone() const noexcept;

	void WINAPI Update() noexcept;
	void WINAPI Update(const PTSZ& ptsz) noexcept;
	void WINAPI BeginBatchDraw() noexcept;
	void WINAPI FlushBatchDraw() noexcept;
	void WINAPI FlushBatchDraw(const PTSZ& ptsz) noexcept;
	void WINAPI EndBatchDraw() noexcept;
	bool WINAPI IsFlushing() const noexcept;

	template<class... Args>
	void WINAPI MoveToEx(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI MoveTo(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI LineTo(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI Line(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI LineRect(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearLineRect(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI SolidRect(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearSolidRect(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI FillRect(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearFillRect(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI Rectangle(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearRect(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI SolidRgn(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearSolidRgn(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI FillRgn(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearRgn(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI LineEllipse(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearLineEllipse(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI SolidEllipse(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearSolidEllipse(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI FillEllipse(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearFillEllipse(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI Ellipse(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearEllipse(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI LineCircle(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearLineCircle(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI SolidCircle(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearSolidCircle(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI FillCircle(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearFillCircle(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI Circle(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearCircle(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI LineRoundRect(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearLineRoundRect(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI SolidRoundRect(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearSolidRoundRect(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI FillRoundRect(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearFillRoundRect(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI RoundRect(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearRoundRect(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI Arc(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ArcTo(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI LinePie(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearLinePie(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI SolidPie(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearSolidPie(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI FillPie(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearFillPie(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI Pie(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearPie(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI LinePolygon(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearLinePolygon(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI SolidPolygon(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearSolidPolygon(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI FillPolygon(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearFillPolygon(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI Polygon(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI ClearPolygon(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI PolyLine(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI PolyLineTo(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI PolyBezier(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI PolyBezierTo(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI FloodFill(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI Fill(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI Solid(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI Clear(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI OutTextExXY(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI OutTextXY(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI DrawCTextEx(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI DrawCText(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI DrawTextEx(Args&&... args) noexcept;
	template<class... Args>
	void WINAPI DrawText(Args&&... args) noexcept;
};

_EASYG_END