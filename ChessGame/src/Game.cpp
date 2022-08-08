/* Copyright (c) 2022 Szabadi László Zsolt
 * You should have received a copy of the GNU AGPL v3.0 license with
 * this file. If not, please write to: pliexe, or visit : https://github.com/Pliexe/VisualDiscordBotCreator/blob/master/LICENSE
 */
#include "pch.h"
#include "Game.h"

void getBitmapDecoder(LPCWSTR path)
{

}

Game::Game(HWND hWnd, ID2D1HwndRenderTarget* renderTarget, IDWriteTextFormat* textFormat)
{
	this->hWnd = hWnd;
	this->renderTarget = renderTarget;
	this->textFormat = textFormat;
	tableContents = (int*)malloc(sizeof(int) * 8 * 8);

	if (tableContents == NULL) {
		MessageBox(NULL, L"Not enough memory!", L"Error", MB_ICONERROR);
		exit(-50);
	}

	// 
	for (int i = 0; i < 64; i++)
		*(tableContents + i) = 0;

	if (renderTarget->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1), &whiteColor) != S_OK) {
		MessageBox(NULL, L"Failed to create brush!", L"Drawing Error!", MB_ICONERROR);
		exit(-300);
	}

#pragma region Load Chess Pieces



#pragma endregion

}

Game::~Game()
{
	free(tableContents);
	whiteColor->Release();
}

void Game::Update()
{
}

void Game::Render()
{
	renderTarget->Clear(D2D1::ColorF(0, 0, 0));

	const int size = 50;

	for (int i = 0; i < 9; i++)
	{
		if (i == 0) {
			for (int j = 2; j <= 9; j++)
				renderTarget->DrawText(
					std::wstring(1, (char)(95 + j)).c_str(),
					1,
					textFormat,
					D2D1::RectF(j * size, 0, j * size - size, size),
					whiteColor
				);

			for (int j = 1; j <= 8; j++)
				renderTarget->DrawText(
					std::to_wstring((j - 9) * -1).c_str(),
					1,
					textFormat,
					D2D1::RectF(0, (j + 1) * size, size, (j + 1) * size - size),
					whiteColor
				);
		}
		else {
			for (int j = (i % 2) ? 2 : 3; j <= 9; j += 2)
			{
				renderTarget->FillRectangle(D2D1::RectF(j * size, i * size, j * size - size, i * size + size), whiteColor);
				//Sleep(200);
			}
		}
	}

	//renderTarget->BeginDraw();
	/*for (int i = 0; i < 64; i+=2)
	{
		int row = trunc(i / 8);
		renderTarget->FillRectangle(D2D1::RectF((i * 40) - row * 320, row * 40, (i * 40 + 40) - row * 320, 40), whiteColor);
	}*/
}

void Game::InitTable()
{
	for (int i = 0; i < 8; i++)
		*(tableContents + i) = -1;

	for (int i = 0; i < 8; i++)
		*(tableContents + i + 51) = -1;


}
