/* Copyright (c) 2022 Szabadi László Zsolt
 * You should have received a copy of the GNU AGPL v3.0 license with
 * this file. If not, please write to: pliexe, or visit : https://github.com/Pliexe/VisualDiscordBotCreator/blob/master/LICENSE
 */
#include "pch.h"
#include "Game.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}
}

void Draw(ID2D1RenderTarget* renderTarget) {

}

bool InitGraphics(HWND hWnd, ID2D1Factory** factory, ID2D1HwndRenderTarget** renderTarget, IDWriteFactory** writeFactory) {

	if (D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		factory
	) != S_OK) return false;

	RECT rect;
	GetClientRect(hWnd, &rect);

	if ((*factory)->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hWnd, D2D1::SizeU(rect.right, rect.bottom)
		),
		renderTarget
	) != S_OK) return false;

	if (DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(*writeFactory),
		reinterpret_cast<IUnknown**>(writeFactory)
	) != S_OK) return false;


	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE legacyPrevInstance, LPWSTR cmd, int nCmdShow)
{
#pragma region Graphics Tools/Pointers

	ID2D1Factory* factory = NULL;
	ID2D1HwndRenderTarget* renderTarget = NULL;
	IDWriteFactory* writeFactory = NULL;

#pragma endregion

#pragma region Create Window Class

	WNDCLASSEX windowClass;

	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.hInstance = hInstance;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.lpszClassName = L"MainWindow";
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowClass);

#pragma endregion

#pragma region Create Window

	RECT rect = { 0, 0, 800, 600 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	HWND hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MainWindow", L"Chess Game", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, 0);

	if (!hWnd) {
		MessageBox(hWnd, L"Failed to create Window!", L"Error!", MB_ICONERROR);
		return -1;
	}

#pragma endregion

#pragma region Init Graphics

	if (!InitGraphics(hWnd, &factory, &renderTarget, &writeFactory)) {
		MessageBox(NULL, L"Failed to initialize Graphics!", L"Error", MB_ICONERROR);

#pragma region Release Graphics Resources

		if (factory) factory->Release();
		if (renderTarget) renderTarget->Release();

#pragma endregion

		return -2;
	}

#pragma endregion

	IDWriteTextFormat* tF = NULL;
	ID2D1SolidColorBrush* textColor = NULL;
	
	if (writeFactory->CreateTextFormat(
		L"Verdana",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		33,
		L"",
		&tF
	) != S_OK) {
		MessageBox(NULL, L"Failed to create textFormat!", L"Drawing Error!", MB_ICONERROR);
		exit(-301);
	}

	tF->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	tF->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	if (renderTarget->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1), &textColor) != S_OK) {
		MessageBox(NULL, L"Failed to create brush!", L"Drawing Error!", MB_ICONERROR);
		exit(-302);
	}

	Game game(hWnd, renderTarget, tF);

	ShowWindow(hWnd, nCmdShow);

#pragma region Game Loop

	MSG message;
	message.message = WM_NULL;

	while (message.message != WM_QUIT) {
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else {
			renderTarget->BeginDraw();
			//Draw(renderTarget);
			game.Update();
			game.Render();
			renderTarget->EndDraw();
		}
	}

#pragma endregion

#pragma region Release Graphics Resources

	if (factory) factory->Release();
	if (renderTarget) renderTarget->Release();

#pragma endregion

	return 0;
}