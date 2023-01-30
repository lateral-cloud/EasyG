#pragma once

#include "EasyGHead.h"

_EASYG_BEGIN

void SetSize(LPSIZE size, LONG cx, LONG cy) {
	size->cx = cx;
	size->cy = cy;
}

void SetRect(LPRECT rect, LONG left, LONG top, const SIZE* size) {
	rect->left = left;
	rect->top = top;
	rect->right = size->cx;
	rect->bottom = size->cy;
}

void SetRect(LPRECT rect, const SIZE* size, LONG right, LONG bottom) {
	rect->left = size->cx;
	rect->top = size->cy;
	rect->right = right;
	rect->bottom = bottom;
}

void SetRect(LPRECT rect, const SIZE* size, const SIZE* size2) {
	rect->left = size->cx;
	rect->top = size->cy;
	rect->right = size2->cx;
	rect->bottom = size2->cy;
}

RECT MakeRect(LONG left, LONG top, LONG right, LONG bottom) {
	return RECT{ left,top,right,bottom };
}

RECT MakeRect(LONG left, LONG top, const SIZE* size) {
	return RECT{ left,top,size->cx,size->cy };
}

RECT MakeRect(const SIZE* size, LONG right, LONG bottom) {
	return RECT{ size->cx,size->cy,right,bottom };
}

RECT MakeRect(const SIZE* size, const SIZE* size2) {
	return RECT{ size->cx,size->cy,size2->cx,size2->cy };
}

class EasySizeMaker {
public:
	PSIZE m_pSize;

	EasySizeMaker() {
		m_pSize = new SIZE;
	}

	~EasySizeMaker() {
		delete m_pSize;
	}

	PSIZE MakeSize(LONG cx, LONG cy) noexcept {
		*m_pSize = SIZE{ cx,cy };
		return m_pSize;
	}

	PSIZE MakeSize(const SIZE& size) noexcept {
		*m_pSize = size;
		return m_pSize;
	}
};

EasySizeMaker ESM;

class EasyPointMaker {
public:
	PPOINT m_pPoint;

	EasyPointMaker() {
		m_pPoint = new POINT;
	}

	~EasyPointMaker() {
		delete m_pPoint;
	}

	PPOINT MakePoint(LONG x, LONG y) noexcept {
		*m_pPoint = POINT{ x,y };
		return m_pPoint;
	}

	PPOINT MakePoint(const POINT& point) noexcept {
		*m_pPoint = point;
		return m_pPoint;
	}
};

EasyPointMaker EPM;

class EasyRectMaker {
public:
	PRECT m_pRect;

	EasyRectMaker() {
		m_pRect = new RECT;
	}

	~EasyRectMaker() {
		delete m_pRect;
	}

	PRECT MakeRectLT(LONG l, LONG t) noexcept {
		*m_pRect = RECT{ l,t,0,0 };
		return m_pRect;
	}

	PRECT MakeRectRB(LONG r, LONG b) noexcept {
		*m_pRect = RECT{ 0,0,r,b };
		return m_pRect;
	}

	PRECT MakeRect(LONG l, LONG t, LONG r, LONG b) noexcept {
		*m_pRect = RECT{ l,t,r,b };
		return m_pRect;
	}

	PRECT MakeRect(const RECT& rect) noexcept {
		*m_pRect = rect;
		return m_pRect;
	}

	PRECT MakeRect(LONG cx, LONG cy) noexcept {
		return MakeRectRB(cx, cy);
	}

	PRECT MakeRect(const SIZE& size) noexcept {
		return MakeRectRB(size.cx, size.cy);
	}
};

EasyRectMaker ERM;

#ifdef EasyToUseSize

#define PRECT_OPEN_NONE				left,top,right,bottom
#define PRECT_OPEN(prect)			(prect)->left,(prect)->top,(prect)->right,(prect)->bottom
#define PSIZE_OPEN_NONE				cx,cy
#define PSIZE_OPEN_WH				width,height		
#define PSIZE_OPEN(psize)			(psize)->cx,(psize)->cy
#define PPOINT_OPEN_NONE			x,y
#define PPOINT_OPEN_NONE_NUM(n)		x ## n,y ## n
#define PPOINT_OPEN(ppoint)			(ppoint)->x,(ppoint)->y
#define PPOINT_OPEN_NUM(ppoint,n)	(ppoint)->x ## n,(ppoint)->y ## n
#define CircleToEllipse(x,y,r)		x - r, y + r, x + r, y - r

#endif

class EasyMaker : public EasySizeMaker, public EasyRectMaker, public EasyPointMaker
{
};

EasyMaker EM;

_EASYG_END