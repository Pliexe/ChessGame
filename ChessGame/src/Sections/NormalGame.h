/* Copyright (c) 2022 Szabadi László Zsolt
 * You should have received a copy of the GNU AGPL v3.0 license with
 * this file. If not, please write to: pliexe, or visit : https://github.com/Pliexe/VisualDiscordBotCreator/blob/master/LICENSE
 */
#pragma once

#include "pch.h"
#include "Graphics/Graphics.h"
#include "GameSectionTemplate.h"

enum ChessPiece {
	None,
	NewPawn,
	Pawn,
	Bishop,
	Knight,
	Rook,
	Queen,
	King
};

class NormalGame : public GameSectionTemplate
{
private:
	
	int* tableContents;
	ID2D1SolidColorBrush* backgroundBrush;
	ID2D1SolidColorBrush* foregroundBrush;
	ID2D1SolidColorBrush* attackBrush;
	ID2D1SolidColorBrush* moveBrush;
	IDWriteTextFormat* labelTextFormat;
	static const int size = 50;

	int selection;
	int lastMove = -1;

#pragma region Chess Pieces

	ID2D1Bitmap* black_pawn;
	ID2D1Bitmap* white_pawn;

	ID2D1Bitmap* black_bishop;
	ID2D1Bitmap* white_bishop;

	ID2D1Bitmap* black_knight;
	ID2D1Bitmap* white_knight;

	ID2D1Bitmap* black_rook;
	ID2D1Bitmap* white_rook;

	ID2D1Bitmap* black_queen;
	ID2D1Bitmap* white_queen;

	ID2D1Bitmap* black_king;
	ID2D1Bitmap* white_king;

#pragma endregion


public:

	NormalGame(Graphics* g, HWND hWnd) : GameSectionTemplate(g, hWnd)
	{
		backgroundBrush = NULL;
		foregroundBrush = NULL;
		attackBrush = NULL;
		moveBrush = NULL;

		labelTextFormat = NULL;
		tableContents = NULL;

		black_pawn = NULL;
		white_pawn = NULL;
		black_bishop = NULL;
		white_bishop = NULL;
		black_knight = NULL;
		white_knight = NULL;
		black_rook = NULL;
		white_rook = NULL;
		black_queen = NULL;
		white_queen = NULL;
		black_king = NULL;
		white_king = NULL;

		selection = -1;
	}

	ID2D1Bitmap* getPiece(int id);

	~NormalGame() {
		SafeRelease(&backgroundBrush);
		SafeRelease(&labelTextFormat);
	}

	bool IsValidMove(int from, int to);
	bool IsValidAttack(int from, int to);

	bool Init();
	void Update();
	void Render();
	void OnClick(int x, int y);
};

