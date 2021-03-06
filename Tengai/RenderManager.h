#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include <unordered_map>
enum class TextAlign
{
	Left,
	Center,
	Right
};
class RenderManager
{
private:
	RenderManager();
	~RenderManager();

public:
	static RenderManager* GetInstance();
	static void Release();
	static void DrawRect(const RECT& _rc);
	static void DrawRect(const RECT& _rc, COLORREF _innerColor);
	static void DrawRect(const RECT& _rc, COLORREF _innerColor, COLORREF _outlineColor);
	static void DrawSimpleCollider(const RECT& _rc, COLORREF _outlineColor);
	static void DrawCircle(const RECT& _rc);
	static void DrawCircle(const RECT& _rc, COLORREF _innerColor, COLORREF _outlineColor);
	static void DrawString(const WCHAR* _str,int _x, int _y, TextAlign _textAlign = TextAlign::Left);
	static void DrawString(const WCHAR* _str, int _x, int _y, const WCHAR* _font, int _fontSize, COLORREF _color, TextAlign _textAlign = TextAlign::Left);
	static void DrawTriangle(const DirectX::XMFLOAT2(&triangle)[3], COLORREF color);
	static void DrawTriangle(const DirectX::XMFLOAT2(&triangle)[3]);
	static void DrawTriangle(const DirectX::XMVECTOR pt1, const DirectX::XMVECTOR pt2, const DirectX::XMVECTOR pt3);
	static void DrawTriangle(const DirectX::XMVECTOR(&triangle)[3]);
	static void Present();
	static void Clear();
private:
	HBRUSH GetBrush(COLORREF color);
	HPEN GetPen(COLORREF color);
	HFONT GetFont(const std::wstring& fontName, size_t fontSize);
private:
	HDC hdc;
	HBITMAP hBitmap;
	HDC hBackBufferDC;
	RECT area = { 0,0,WINDOW_WIDTH,WINDOW_HEIGHT };
	unordered_map<COLORREF, HBRUSH> cachedBrushs;
	unordered_map<COLORREF, HPEN> cachedPen;
	unordered_map<std::wstring, HFONT> cachedFonts;
};

