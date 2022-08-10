/* Copyright (c) 2022 Szabadi László Zsolt
 * You should have received a copy of the GNU AGPL v3.0 license with
 * this file. If not, please write to: pliexe, or visit : https://github.com/Pliexe/VisualDiscordBotCreator/blob/master/LICENSE
 */
#include "Graphics.h"

Graphics::Graphics()
{
	factory = NULL;
	renderTarget = NULL;
	writeFactory = NULL;
}

Graphics::~Graphics()
{
	SafeRelease(&factory);
	SafeRelease(&renderTarget);
	SafeRelease(&writeFactory);
}

bool Graphics::Init(HWND hWnd)
{
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory)))
	{
		GRAPHICS_ERROR(GRAPHICS_ERROR_FACTORY_CREATION);
		return false;
	}

	RECT rect;
	GetClientRect(hWnd, &rect);

	if (FAILED(factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hWnd, D2D1::SizeU(rect.right, rect.bottom)
		),
		&renderTarget
	))) {
		GRAPHICS_ERROR(GRAPHICS_ERROR_RENDER_TARGET_CREATION);
		return false;
	}

	if (FAILED(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(writeFactory),
		reinterpret_cast<IUnknown**>(&writeFactory)
	))) return false;

	return true;
}

HRESULT Graphics::CreateSolidColorBrush(ID2D1SolidColorBrush** ref, float r, float g, float b, float a)
{
	return renderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), ref);
}

HRESULT Graphics::CreateTextFormat(IDWriteTextFormat** ref, const WCHAR* fontFamilyName, IDWriteFontCollection* fontCollection, FLOAT fontSize, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, const WCHAR* localeName)
{
	return writeFactory->CreateTextFormat(
		fontFamilyName, fontCollection, fontWeight, fontStyle, fontStretch,
		fontSize, localeName, ref
	);
}

bool Graphics::CreateBitmapImage(const wchar_t* path, ID2D1Bitmap** bmp)
{
	CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	IWICImagingFactory* wicFactory = NULL;
	if (FAILED(CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&wicFactory
	))) return false;

	IWICBitmapDecoder* wicDecoder = NULL;

	if (FAILED(wicFactory->CreateDecoderFromFilename(
		path,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&wicDecoder
	))) return false;

	IWICBitmapFrameDecode* wicFrame = NULL;
	
	if (FAILED(wicDecoder->GetFrame(0, &wicFrame))) return false;

	IWICFormatConverter* wicConverter = NULL;

	if (FAILED(wicFactory->CreateFormatConverter(&wicConverter))) return false;

	if (FAILED(wicConverter->Initialize(
		wicFrame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.0,
		WICBitmapPaletteTypeCustom
	))) return false;

	if (FAILED(renderTarget->CreateBitmapFromWicBitmap(
		wicConverter,
		NULL,
		bmp
	))) return false;


	SafeRelease(&wicFactory);
	SafeRelease(&wicDecoder);
	SafeRelease(&wicConverter);
	SafeRelease(&wicFrame);

	return true;
}

void Graphics::DrawTextW(std::string text, IDWriteTextFormat* textFormat, ID2D1Brush* brush, D2D1_RECT_F rect)
{
	DrawText(std::wstring(text.begin(), text.end()).c_str(), textFormat, brush, rect);
}

void Graphics::DrawTextW(const wchar_t* text, IDWriteTextFormat* textFormat, ID2D1Brush* brush, D2D1_RECT_F rect)
{
	renderTarget->DrawTextW(text, wcslen(text), textFormat, rect, brush);
}

void Graphics::FillRectangle(ID2D1Brush* brush, D2D1_RECT_F rect)
{
	renderTarget->FillRectangle(rect, brush);
}

void Graphics::FillEllipse(ID2D1Brush* brush, D2D1_RECT_F rect)
{
	renderTarget->FillEllipse(D2D1::Ellipse(D2D1::Point2F(rect.left, rect.top), rect.right, rect.bottom), brush);
}

void Graphics::DrawBitmap(ID2D1Bitmap* bitmap, D2D1_RECT_F rect, float opacity, D2D1_BITMAP_INTERPOLATION_MODE interpolationMode)
{
	renderTarget->DrawBitmap(
		bitmap, rect,
		opacity, interpolationMode,
		D2D1::RectF(0.0f, 0.0f, bitmap->GetSize().width, bitmap->GetSize().height)
	);
}
