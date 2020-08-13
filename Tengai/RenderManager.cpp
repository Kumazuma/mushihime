#include "stdafx.h"
#include "RenderManager.h"

RenderManager* pRenderManager = nullptr;

RenderManager::RenderManager()
{
	hdc = GetDC(g_hwnd);
	hBitmap = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	hBackBufferDC = CreateCompatibleDC(hdc);
	SelectObject(hBackBufferDC, hBitmap);
	SetBkMode(hBackBufferDC, TRANSPARENT);
}

RenderManager::~RenderManager()
{
	ReleaseDC(g_hwnd, hdc);
	//일단 해제해 주고
	if (hBackBufferDC != nullptr)
	{
		SelectObject(hBackBufferDC, nullptr);
		DeleteDC(hBackBufferDC);
	}
	//백버퍼도 지운다.
	if (hBitmap != nullptr)
	{
		DeleteObject(hBitmap);
	}
	for (auto& item : cachedBrushs)
	{
		DeleteObject(item.second);
	}
	for (auto& item : cachedPen)
	{
		DeleteObject(item.second);
	}
	for (auto& item : cachedFonts)
	{
		DeleteObject(item.second);
	}
	cachedFonts.clear();
	cachedPen.clear();
	cachedBrushs.clear();
}

RenderManager * RenderManager::GetInstance()
{
	if (pRenderManager == nullptr)
	{
		pRenderManager = new RenderManager;
	}
	return pRenderManager;
}

void RenderManager::Release()
{
	delete pRenderManager;
	pRenderManager = nullptr;
}

void RenderManager::DrawRect(const RECT& _rc)
{
	Rectangle(pRenderManager->hBackBufferDC, _rc.left, _rc.top, _rc.right, _rc.bottom);
}

void RenderManager::DrawRect(const RECT& _rc, COLORREF _innerColor)
{
	HBRUSH hBrush{ pRenderManager->GetBrush(_innerColor) };
	HGDIOBJ oldBrush{ SelectObject(pRenderManager->hBackBufferDC, hBrush) };
	
	Rectangle(pRenderManager->hBackBufferDC, _rc.left, _rc.top, _rc.right, _rc.bottom);

	SelectObject(pRenderManager->hBackBufferDC, oldBrush);
}

void RenderManager::DrawRect(const RECT& _rc, COLORREF _innerColor, COLORREF _outlineColor)
{

	HBRUSH hBrush{ pRenderManager->GetBrush(_innerColor) };
	HPEN hPen{ pRenderManager->GetPen(_outlineColor) };
	HGDIOBJ oldBrush = SelectObject(pRenderManager->hBackBufferDC, hBrush);
	HGDIOBJ oldPen = SelectObject(pRenderManager->hBackBufferDC, hPen);

	Rectangle(pRenderManager->hBackBufferDC, _rc.left, _rc.top, _rc.right, _rc.bottom);

	SelectObject(pRenderManager->hBackBufferDC, oldBrush);
	SelectObject(pRenderManager->hBackBufferDC, oldPen);

}

void RenderManager::DrawSimpleCollider(const RECT& _rc, COLORREF _outlineColor)
{
	HPEN hPen{ pRenderManager->GetPen(_outlineColor) };
	HGDIOBJ oldPen = SelectObject(pRenderManager->hBackBufferDC, hPen);
	Rectangle(pRenderManager->hBackBufferDC, _rc.left, _rc.top, _rc.right, _rc.bottom);
	SelectObject(pRenderManager->hBackBufferDC, oldPen);
}

void RenderManager::DrawCircle(const RECT& _rc)
{
	Ellipse(pRenderManager->hBackBufferDC, _rc.left, _rc.top, _rc.right, _rc.bottom);
}

void RenderManager::DrawCircle(const RECT& _rc, COLORREF _innerColor, COLORREF _outlineColor)
{
	HBRUSH hBrush{ pRenderManager->GetBrush(_innerColor) };
	HPEN hPen{ pRenderManager->GetPen(_outlineColor) };
	HGDIOBJ oldBrush = SelectObject(pRenderManager->hBackBufferDC, hBrush);
	HGDIOBJ oldPen = SelectObject(pRenderManager->hBackBufferDC, hPen);
	Ellipse(pRenderManager->hBackBufferDC, _rc.left, _rc.top, _rc.right, _rc.bottom);
	SelectObject(pRenderManager->hBackBufferDC, oldBrush);
	SelectObject(pRenderManager->hBackBufferDC, oldPen);
}

void RenderManager::DrawString(const WCHAR* _str, int _x, int _y, TextAlign _textAlign)
{
	SIZE size{};
	const int len{ static_cast<int>(wcslen(_str)) };
	int divVal{ 0 };
	GetTextExtentPointW(pRenderManager->hBackBufferDC, _str, len, &size);
	switch (_textAlign)
	{
	case TextAlign::Center:
		divVal = -1;
		break;
	case TextAlign::Right:
		divVal = -2;
		break;
	}
	_x += static_cast<int>(size.cx * divVal / 2.f);
	
	//DrawTextExW(RenderManager->hBackBufferDC, _str, 0, wcslen(_str), )
	TextOutW(pRenderManager->hBackBufferDC, _x, _y, _str, len);
}

void RenderManager::DrawString(const WCHAR* _str, int _x, int _y, const WCHAR* _font, int _fontSize, COLORREF _color, TextAlign _textAlign)
{
	if (_font == nullptr)
	{
		_font = L"";
	}
	HFONT hFont{ pRenderManager->GetFont(_font, _fontSize) };
	HFONT oldFont = (HFONT)SelectObject(pRenderManager->hBackBufferDC, hFont);
	COLORREF oldColor = SetTextColor(pRenderManager->hBackBufferDC, _color);
	const int textLen{ static_cast<int>( wcslen(_str)) };
	SIZE size{};
	GetTextExtentPointW(pRenderManager->hBackBufferDC, _str, textLen , &size);
	int divVal{ 0 };
	switch (_textAlign)
	{
	case TextAlign::Center:
		divVal = -1;
		break;
	case TextAlign::Right:
		divVal = -2;
		break;
	}
	_x += static_cast<int>(size.cx * divVal / 2.f);
	//DrawTextExW(RenderManager->hBackBufferDC, _str, 0, wcslen(_str), )
	TextOutW(pRenderManager->hBackBufferDC, _x, _y, _str, textLen);
	SelectObject(pRenderManager->hBackBufferDC, oldFont);
	SetTextColor(pRenderManager->hBackBufferDC, oldColor);
}

void RenderManager::DrawTriangle(const DirectX::XMFLOAT2(&triangle)[3], COLORREF color)
{
	//TODO: now not implementation color fill
	HDC hDC{ pRenderManager->hBackBufferDC };
	BeginPath(hDC);
	const DirectX::XMFLOAT2* it{ triangle };
	const DirectX::XMFLOAT2* const end{ triangle + 3 };
	MoveToEx(
		hDC,
		static_cast<int>(it->x),
		static_cast<int>(it->y),
		nullptr
	);
	++it;
	for (; it != end; ++it)
	{
		LineTo(
			hDC,
			static_cast<int>(it->x),
			static_cast<int>(it->y)
		);
	}
	it = triangle;
	LineTo(
		hDC,
		static_cast<int>(it->x),
		static_cast<int>(it->y)
	);
	EndPath(hDC);
	HBRUSH hBrush{ pRenderManager->GetBrush(color)  };
	HBRUSH hOldBrush{ (HBRUSH)SelectObject(hDC, hBrush) };
	StrokeAndFillPath(hDC);
	SelectObject(hDC, hOldBrush);
}

void RenderManager::DrawTriangle(const DirectX::XMFLOAT2(&triangle)[3])
{
	HDC hDC{ pRenderManager->hBackBufferDC };
	BeginPath(hDC);
	const DirectX::XMFLOAT2* it{ triangle };
	const DirectX::XMFLOAT2* const end{ triangle + 3 };
	MoveToEx(
		hDC,
		static_cast<int>(it->x),
		static_cast<int>(it->y),
		nullptr
	);
	++it;
	for (; it != end; ++it)
	{
		LineTo(
			hDC,
			static_cast<int>(it->x),
			static_cast<int>(it->y)
		);
	}
	it = triangle;
	LineTo(
		hDC,
		static_cast<int>(it->x),
		static_cast<int>(it->y)
	);
	EndPath(hDC);
	StrokeAndFillPath(hDC);
}

void RenderManager::DrawTriangle(const DirectX::XMVECTOR pt1, const DirectX::XMVECTOR pt2, const DirectX::XMVECTOR pt3)
{
	HDC hDC{ pRenderManager->hBackBufferDC };
	BeginPath(hDC);
	DirectX::XMFLOAT2 triangle[3]{};
	DirectX::XMStoreFloat2(triangle + 0, pt1);
	DirectX::XMStoreFloat2(triangle + 1, pt2);
	DirectX::XMStoreFloat2(triangle + 2, pt3);
	DrawTriangle(triangle);
}

void RenderManager::DrawTriangle(const DirectX::XMVECTOR(&points)[3])
{
	DirectX::XMFLOAT2 triangle[3]{};
	for (int i = 0; i < 3; ++i)
	{
		DirectX::XMStoreFloat2(triangle + i, points[i]);
	}
	DrawTriangle(triangle);
}

void RenderManager::Present()
{
	BitBlt(pRenderManager->hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		pRenderManager->hBackBufferDC, 0, 0, SRCCOPY);
}

void RenderManager::Clear()
{
	FillRect(pRenderManager->hBackBufferDC, &pRenderManager->area,
		(HBRUSH)GetStockObject(WHITE_BRUSH));
}

HBRUSH RenderManager::GetBrush(COLORREF color)
{
	auto it{ cachedBrushs.find(color) };
	if (it == cachedBrushs.end())
	{
		it = cachedBrushs.emplace(color, CreateSolidBrush(color)).first;
	}
	return it->second;
}

HPEN RenderManager::GetPen(COLORREF color)
{
	auto it{ cachedPen.find(color) };
	if (it == cachedPen.end())
	{
		it = cachedPen.emplace(color, CreatePen(PS_SOLID, 1, color)).first;
	}
	return it->second;
}

HFONT RenderManager::GetFont(const std::wstring& fontName, size_t fontSize)
{
	int radix{ fontSize == 0 ? 0 : static_cast<int>(log10(static_cast<float>(fontSize))) };
	std::wstring key{ fontName };
	for (int i{ radix }; i >= 0; --i)
	{
		int s{ static_cast<int>(fontSize / powf(10.f,static_cast<float>(i) )) % 10 };
		key.push_back(L'0' + s);
	}
	auto it{ cachedFonts.find(key) };
	if (it == cachedFonts.end())
	{
		HFONT hFont = CreateFontW(fontSize, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, fontName.c_str());
		it = cachedFonts.emplace(key, hFont).first;
	}
	return it->second;
}
