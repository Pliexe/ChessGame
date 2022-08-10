/* Copyright (c) 2022 Szabadi László Zsolt
 * You should have received a copy of the GNU AGPL v3.0 license with
 * this file. If not, please write to: pliexe, or visit : https://github.com/Pliexe/VisualDiscordBotCreator/blob/master/LICENSE
 */
#pragma once

#include "pch.h"

class Graphics
{
private:

	// COM INTERFACES FOR FACTORIES ETC
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;
	IDWriteFactory* writeFactory;

public:
	Graphics();
	~Graphics();

	bool Init(HWND);

	inline void BeginDraw() { renderTarget->BeginDraw(); }
	inline void EndDraw() { renderTarget->EndDraw(); }
	inline void ClearScreen() { renderTarget->Clear(D2D1::ColorF(0, 0, 0)); }

	ID2D1HwndRenderTarget* getRenderTarget() const { return renderTarget; }
	ID2D1Factory* getFactory() const { return factory; }
	IDWriteFactory* getWriteFactory() const { return writeFactory; }

	// Resource Creation

	HRESULT CreateSolidColorBrush(ID2D1SolidColorBrush** ref, float r, float g, float b, float a = 1.0);
	HRESULT CreateTextFormat(
		IDWriteTextFormat** ref,
		const WCHAR* fontFamilyName,
		IDWriteFontCollection* fontCollection,
		FLOAT fontSize,
		DWRITE_FONT_WEIGHT fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE fontStyle = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH fontStretch = DWRITE_FONT_STRETCH_NORMAL,
		const WCHAR* localeName = L""
	);
	bool CreateBitmapImage(const wchar_t* path, ID2D1Bitmap** bitmap);

	// Drawing
	
	void DrawTextW(std::string text, IDWriteTextFormat* textFormat, ID2D1Brush* brush, float left, float top, float right, float bottom) { DrawText(text, textFormat, brush, D2D1::RectF(left, top, right, bottom)); }
	void DrawTextW(std::string text, IDWriteTextFormat* textFormat, ID2D1Brush* brush, D2D1_RECT_F rect);
	void DrawTextW(const wchar_t* text, IDWriteTextFormat* textFormat, ID2D1Brush* brush, float left, float top, float right, float bottom) { DrawText(text, textFormat, brush, D2D1::RectF(left, top, right, bottom)); }
	void DrawTextW(const wchar_t* text, IDWriteTextFormat* textFormat, ID2D1Brush* brush, D2D1_RECT_F rect);

	void FillRectangle(ID2D1Brush* brush, float left, float top, float right, float bottom) { FillRectangle(brush, D2D1::RectF(left, top, right, bottom)); }
	void FillRectangle(ID2D1Brush* brush, D2D1_RECT_F rect);

	void FillEllipse(ID2D1Brush* brush, float left, float top, float right, float bottom) { FillEllipse(brush, D2D1::RectF(left, top, right, bottom)); }
	void FillEllipse(ID2D1Brush* brush, D2D1_RECT_F rect);

	void DrawBitmap(ID2D1Bitmap* bitmap, float left, float top, float right, float bottom, float opacity = 1.0f, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR) { DrawBitmap(bitmap, D2D1::RectF(left, top, right, bottom), opacity, interpolationMode); }
	void DrawBitmap(ID2D1Bitmap* bitmap, D2D1_RECT_F rect, float opacity = 1.0f, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode = D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
};

