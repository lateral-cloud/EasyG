#pragma once

#include "EasyCanvas_def.h"

_EASYG_BEGIN

template<class ...Args>
EasyCanvas::EasyCanvas(Args&&... args) noexcept : EasyImage2(_STD forward<Args>(args)...) {}

EasyCanvas::EasyCanvas(const EasyCanvas& newCanvas) noexcept : EasyCanvas(newCanvas.Clone()) {}

EasyCanvas::EasyCanvas(EasyCanvas&& newCanvas) noexcept : IsFlush(newCanvas.IsFlush), EasyImage2(_STD move(newCanvas)) {}

template<class ...Args>
EasyCanvas& WINAPI EasyCanvas::operator=(Args&&... args) noexcept
{
	EasyImage2::operator=(_STD forward<Args>(args)...);
	return *this;
}

EasyCanvas& WINAPI EasyCanvas::operator=(const EasyCanvas& newCanvas) noexcept
{
	return *this = newCanvas.Clone();
}

EasyCanvas& WINAPI EasyCanvas::operator=(EasyCanvas&& newCanvas) noexcept
{
	EasyImage2::operator=(_STD move(newCanvas));
	IsFlush = newCanvas.IsFlush;
	return *this;
}

EasyCanvas WINAPI EasyCanvas::Clone() const noexcept
{
	EasyCanvas newCanvas;
	newCanvas.EasyImage2::operator=(*this);
	newCanvas.IsFlush = this->IsFlush;
	return newCanvas;
}

void WINAPI EasyCanvas::Update() noexcept
{
	if (IsFlush)
		EasyImage2::Flush();
}

void WINAPI EasyCanvas::Update(const PTSZ& ptsz) noexcept
{
	if (IsFlush)
		EasyImage2::Flush(ptsz);
}

void WINAPI EasyCanvas::BeginBatchDraw() noexcept
{
	IsFlush = 0;
}

void WINAPI EasyCanvas::FlushBatchDraw() noexcept
{
	EasyImage2::Flush();
}

void WINAPI EasyCanvas::FlushBatchDraw(const PTSZ& ptsz) noexcept
{
	EasyImage2::Flush(ptsz);
}

void WINAPI EasyCanvas::EndBatchDraw() noexcept
{
	IsFlush = 1;
	this->FlushBatchDraw();
}

bool WINAPI EasyCanvas::IsFlushing() const noexcept
{
	return IsFlush;
}

template<class... Args>
void WINAPI EasyCanvas::MoveToEx(Args&&... args) noexcept { EasyImage2::MoveToEx(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::MoveTo(Args&&... args) noexcept { EasyImage2::MoveTo(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::LineTo(Args&&... args) noexcept { EasyImage2::LineTo(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::Line(Args&&... args) noexcept { EasyImage2::Line(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::LineRect(Args&&... args) noexcept { EasyImage2::LineRect(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearLineRect(Args&&... args) noexcept { EasyImage2::ClearLineRect(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::SolidRect(Args&&... args) noexcept { EasyImage2::SolidRect(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearSolidRect(Args&&... args) noexcept { EasyImage2::ClearSolidRect(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::FillRect(Args&&... args) noexcept { EasyImage2::FillRect(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearFillRect(Args&&... args) noexcept { EasyImage2::ClearFillRect(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::Rectangle(Args&&... args) noexcept { EasyImage2::Rectangle(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearRect(Args&&... args) noexcept { EasyImage2::ClearRect(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::SolidRgn(Args&&... args) noexcept { EasyImage2::SolidRgn(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearSolidRgn(Args&&... args) noexcept { EasyImage2::ClearSolidRgn(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::FillRgn(Args&&... args) noexcept { EasyImage2::FillRgn(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearRgn(Args&&... args) noexcept { EasyImage2::ClearRgn(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::LineEllipse(Args&&... args) noexcept { EasyImage2::LineEllipse(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearLineEllipse(Args&&... args) noexcept { EasyImage2::ClearLineEllipse(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::SolidEllipse(Args&&... args) noexcept { EasyImage2::SolidEllipse(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearSolidEllipse(Args&&... args) noexcept { EasyImage2::ClearSolidEllipse(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::FillEllipse(Args&&... args) noexcept { EasyImage2::FillEllipse(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearFillEllipse(Args&&... args) noexcept { EasyImage2::ClearFillEllipse(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::Ellipse(Args&&... args) noexcept { EasyImage2::Ellipse(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearEllipse(Args&&... args) noexcept { EasyImage2::ClearEllipse(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::LineCircle(Args&&... args) noexcept { EasyImage2::LineCircle(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearLineCircle(Args&&... args) noexcept { EasyImage2::ClearLineCircle(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::SolidCircle(Args&&... args) noexcept { EasyImage2::SolidCircle(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearSolidCircle(Args&&... args) noexcept { EasyImage2::ClearSolidCircle(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::FillCircle(Args&&... args) noexcept { EasyImage2::FillCircle(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearFillCircle(Args&&... args) noexcept { EasyImage2::ClearFillCircle(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::Circle(Args&&... args) noexcept { EasyImage2::Circle(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearCircle(Args&&... args) noexcept { EasyImage2::ClearCircle(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::LineRoundRect(Args&&... args) noexcept { EasyImage2::LineRoundRect(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearLineRoundRect(Args&&... args) noexcept { EasyImage2::ClearLineRoundRect(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::SolidRoundRect(Args&&... args) noexcept { EasyImage2::SolidRoundRect(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearSolidRoundRect(Args&&... args) noexcept { EasyImage2::ClearSolidRoundRect(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::FillRoundRect(Args&&... args) noexcept { EasyImage2::FillRoundRect(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearFillRoundRect(Args&&... args) noexcept { EasyImage2::ClearFillRoundRect(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::RoundRect(Args&&... args) noexcept { EasyImage2::RoundRect(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearRoundRect(Args&&... args) noexcept { EasyImage2::ClearRoundRect(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::Arc(Args&&... args) noexcept { EasyImage2::Arc(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ArcTo(Args&&... args) noexcept { EasyImage2::ArcTo(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::LinePie(Args&&... args) noexcept { EasyImage2::LinePie(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearLinePie(Args&&... args) noexcept { EasyImage2::ClearLinePie(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::SolidPie(Args&&... args) noexcept { EasyImage2::SolidPie(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearSolidPie(Args&&... args) noexcept { EasyImage2::ClearSolidPie(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::FillPie(Args&&... args) noexcept { EasyImage2::FillPie(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearFillPie(Args&&... args) noexcept { EasyImage2::ClearFillPie(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::Pie(Args&&... args) noexcept { EasyImage2::Pie(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearPie(Args&&... args) noexcept { EasyImage2::ClearPie(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::LinePolygon(Args&&... args) noexcept { EasyImage2::LinePolygon(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearLinePolygon(Args&&... args) noexcept { EasyImage2::ClearLinePolygon(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::SolidPolygon(Args&&... args) noexcept { EasyImage2::SolidPolygon(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearSolidPolygon(Args&&... args) noexcept { EasyImage2::ClearSolidPolygon(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::FillPolygon(Args&&... args) noexcept { EasyImage2::FillPolygon(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearFillPolygon(Args&&... args) noexcept { EasyImage2::ClearFillPolygon(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::Polygon(Args&&... args) noexcept { EasyImage2::Polygon(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::ClearPolygon(Args&&... args) noexcept { EasyImage2::ClearPolygon(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::PolyLine(Args&&... args) noexcept { EasyImage2::PolyLine(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::PolyLineTo(Args&&... args) noexcept { EasyImage2::PolyLineTo(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::PolyBezier(Args&&... args) noexcept { EasyImage2::PolyBezier(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::PolyBezierTo(Args&&... args) noexcept { EasyImage2::PolyBezierTo(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::FloodFill(Args&&... args) noexcept { EasyImage2::FloodFill(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::Fill(Args&&... args) noexcept { EasyImage2::Fill(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::Solid(Args&&... args) noexcept { EasyImage2::Solid(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::Clear(Args&&... args) noexcept { EasyImage2::Clear(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::OutTextExXY(Args&&... args) noexcept { EasyImage2::OutTextExXY(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::OutTextXY(Args&&... args) noexcept { EasyImage2::OutTextXY(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::DrawCTextEx(Args&&... args) noexcept { EasyImage2::DrawCTextEx(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::DrawCText(Args&&... args) noexcept { EasyImage2::DrawCText(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::DrawTextEx(Args&&... args) noexcept { EasyImage2::DrawTextEx(_STD forward<Args>(args)...); this->Update(); }
template<class... Args>
void WINAPI EasyCanvas::DrawText(Args&&... args) noexcept { EasyImage2::DrawText(_STD forward<Args>(args)...); this->Update(); }

_EASYG_END

#undef DSOBJECT
#undef BEGINLINE
#undef ENDLINE
#undef BEGINSOLID
#undef ENDSOLID
#undef BEGINFILL
#undef ENDFILL
#undef BEGINCLEARLINE
#undef ENDCLEARLINE
#undef BEGINCLEARSOLID
#undef ENDCLEARSOLID
#undef BEGINCLEARFILL
#undef ENDCLEARFILL