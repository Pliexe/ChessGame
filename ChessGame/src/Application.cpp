/* Copyright (c) 2022 Szabadi László Zsolt
 * You should have received a copy of the GNU AGPL v3.0 license with
 * this file. If not, please write to: pliexe, or visit : https://github.com/Pliexe/VisualDiscordBotCreator/blob/master/LICENSE
 */
#include "Application.h"

Application::Application()
{
	graphics = NULL;
	currentGameSection = NULL;
	hWnd = NULL;
}

Application::~Application()
{
}

LRESULT Application::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONUP:
		if (currentGameSection) currentGameSection->OnClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}
}

LRESULT Application::StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Application* pThis = NULL;

	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		pThis = (Application*)pCreate->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

		pThis->hWnd = hWnd;
	}
	else pThis = (Application*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (pThis)
		return pThis->WindowProc(uMsg, wParam, lParam);
	else return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Application::Initialize(int nCmdShow)
{
#pragma region Create Window Class

	WNDCLASSEX windowClass;

	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.lpfnWndProc = Application::StaticWindowProc;
	windowClass.lpszClassName = L"MainWindow";
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowClass);

#pragma endregion

#pragma region Create Window

	RECT rect = { 0, 0, 800, 600 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	HWND hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MainWindow", L"Chess Game", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, GetModuleHandle(NULL), this);

	if (!hWnd) {
		MessageBox(hWnd, L"Failed to create Window!", L"Error!", MB_ICONERROR);
		exit(-1);
	}

#pragma endregion

#pragma region Init Graphics

	graphics = new Graphics();
	if (!graphics->Init(hWnd)) {
		std::string lastErr = std::system_category().message(GetLastError());
		MessageBox(NULL, std::wstring(lastErr.begin(), lastErr.end()).c_str(), L"Failed to initialize game section!", MB_ICONERROR);
		exit(-2);
	}

#pragma endregion

#pragma region Init Menu

	// Will be replaced with main menu when out of testing

	currentGameSection = new NormalGame(graphics, hWnd);

	if (!currentGameSection->Init()) {
		MessageBox(hWnd, L"Failed to create menu!", L"ERROR", MB_ICONERROR);
		exit(-69);
	}

#pragma endregion

	ShowWindow(hWnd, nCmdShow);

}

void Application::GameLoop()
{
	MSG message;
	message.message = WM_NULL;

	while (message.message != WM_QUIT) {
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else {
			if (currentGameSection) {
				graphics->BeginDraw();
				graphics->ClearScreen();
				currentGameSection->Update();
				currentGameSection->Render();
				graphics->EndDraw();
			}
		}
	}
}
