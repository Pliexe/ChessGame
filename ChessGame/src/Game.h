/* Copyright (c) 2022 Szabadi László Zsolt
 * You should have received a copy of the GNU AGPL v3.0 license with
 * this file. If not, please write to: pliexe, or visit : https://github.com/Pliexe/VisualDiscordBotCreator/blob/master/LICENSE
 */
#pragma once


enum ChessPiece {
	None,
	Pawn,
	Bishop,
	Knight,
	Rook,
	Queen,
	King
};

class Game
{
private:

	HWND hWnd;
	int* tableContents;
	ID2D1HwndRenderTarget* renderTarget = NULL;
	ID2D1SolidColorBrush* whiteColor = NULL;
	IDWriteTextFormat* textFormat = NULL;

public:
	Game(HWND, ID2D1HwndRenderTarget*, IDWriteTextFormat*);
	~Game();
	void Update();
	void Render();

	void InitTable();
};

