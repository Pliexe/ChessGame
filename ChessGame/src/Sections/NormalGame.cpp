/* Copyright (c) 2022 Szabadi László Zsolt
 * You should have received a copy of the GNU AGPL v3.0 license with
 * this file. If not, please write to: pliexe, or visit : https://github.com/Pliexe/VisualDiscordBotCreator/blob/master/LICENSE
 */
#include "NormalGame.h"
#pragma comment (lib, "winmm.lib") // For some reason adding it to linker won't work

bool NormalGame::Init()
{

	if (FAILED(graphics->CreateSolidColorBrush(&backgroundBrush, 1.0f, 1.0f, 1.0f))) return false;
	if (FAILED(graphics->CreateSolidColorBrush(&foregroundBrush, 0.46f, 0.58f, 0.33f))) return false;
	if (FAILED(graphics->CreateSolidColorBrush(&attackBrush, 1.0f, 0.0f, 0.0f, 0.5f))) return false;
	if (FAILED(graphics->CreateSolidColorBrush(&moveBrush, 0.98f, 0.72f, 0.01f, 0.5f))) return false;

	if (FAILED(graphics->CreateTextFormat(&labelTextFormat, L"Verdana", NULL, 35))) return false;

	labelTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	labelTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	if (!graphics->CreateBitmapImage(L"data\\p_b.png", &black_pawn)) return false;
	if (!graphics->CreateBitmapImage(L"data\\p_w.png", &white_pawn)) return false;

	if (!graphics->CreateBitmapImage(L"data\\b_b.png", &black_bishop)) return false;
	if (!graphics->CreateBitmapImage(L"data\\b_w.png", &white_bishop)) return false;

	if (!graphics->CreateBitmapImage(L"data\\knight_b.png", &black_knight)) return false;
	if (!graphics->CreateBitmapImage(L"data\\knight_w.png", &white_knight)) return false;

	if (!graphics->CreateBitmapImage(L"data\\r_b.png", &black_rook)) return false;
	if (!graphics->CreateBitmapImage(L"data\\r_w.png", &white_rook)) return false;

	if (!graphics->CreateBitmapImage(L"data\\q_b.png", &black_queen)) return false;
	if (!graphics->CreateBitmapImage(L"data\\q_w.png", &white_queen)) return false;

	if (!graphics->CreateBitmapImage(L"data\\k_b.png", &black_king)) return false;
	if (!graphics->CreateBitmapImage(L"data\\k_w.png", &white_king)) return false;

	tableContents = (int*)calloc(64, sizeof(int));

	if (tableContents == NULL) {
		MessageBox(NULL, L"Out of memory!", L"Critical!", MB_ICONERROR);
		exit(-2);
	}

	for (int i = 8; i < 16; i++)
		*(tableContents + i) = (int)ChessPiece::NewPawn * -1;

	for (int i = 48; i < 56; i++)
		*(tableContents + i) = (int)ChessPiece::NewPawn;

	*(tableContents + 56) = (int)ChessPiece::Rook;
	*(tableContents + 57) = (int)ChessPiece::Knight;
	*(tableContents + 58) = (int)ChessPiece::Bishop;
	*(tableContents + 59) = (int)ChessPiece::Queen;
	*(tableContents + 60) = (int)ChessPiece::King;
	*(tableContents + 61) = (int)ChessPiece::Bishop;
	*(tableContents + 62) = (int)ChessPiece::Knight;
	*(tableContents + 63) = (int)ChessPiece::Rook;

	for (int i = 0; i < 8; i++)
		*(tableContents + i) = *(tableContents + 56 + i) * -1;

	return true;
}

void NormalGame::Update()
{
}

void NormalGame::Render()
{
	for (int i = 0; i < 64; i++) {
		if (i == 0) {


			for (int j = 0; j < 8; j++)
				graphics->DrawTextW(std::to_wstring((j - 8) * -1).c_str(), labelTextFormat, backgroundBrush
					, 0, (j + 1) * size, size, (j + 1) * size - size);
		}

		if (i == 8) {
			for (int j = 2; j <= 9; j++)
				graphics->DrawTextW(std::wstring(1, (char)(95 + j)).c_str(), labelTextFormat, backgroundBrush
					, j * size, size * 16, j * size - size, size);
		}

		int row = trunc(i / 8);

		float left = (i % 8) * size + size;
		float top = row * size;
		float right = (i % 8) * size + size * 2;
		float bottom = row * size + size;

		graphics->FillRectangle(((i + row % 2) % 2) ? foregroundBrush : backgroundBrush, left, top, right, bottom);
		
		if (IsValidMove(selection, i)) {
			graphics->FillEllipse(moveBrush, left + size / 2, top + size / 2, size / 4, size / 4);
		}

		if(IsValidAttack(selection, i))
			graphics->FillRectangle(attackBrush, left, top, right, bottom);

		if (selection == i) continue;

		ID2D1Bitmap* piece = getPiece(*(tableContents + i));

		if (piece != NULL)
			graphics->DrawBitmap(piece, left, top, right, bottom);
	}

	if (selection > 0) {
		POINT p;
		if (GetCursorPos(&p)) {
			if (ScreenToClient(hWnd, &p)) {
				std::cout << "Mouse at: x: " << p.x << ", y: " << p.y << std::endl;
				float half = size / 2;
				float left = p.x - half;
				float top = p.y - half;
				float right = p.x + half;
				float bottom = p.y + half;

				ID2D1Bitmap* piece = getPiece(*(tableContents + selection));

				if (piece != NULL)
					graphics->DrawBitmap(piece, left, top, right, bottom);
			}
		}
	}
}

ID2D1Bitmap* NormalGame::getPiece(int id)
{
	bool black = id < 0;
	if (id < 0) id *= -1;

	switch (id)
	{
	case ChessPiece::NewPawn:
	case ChessPiece::Pawn:
		return black ? black_pawn : white_pawn;
	case ChessPiece::Rook:
		return black ? black_rook : white_rook;
	case ChessPiece::Knight:
		return black ? black_knight : white_knight;
	case ChessPiece::Bishop:
		return black ? black_bishop : white_bishop;
	case ChessPiece::King:
		return black ? black_king : white_king;
	case ChessPiece::Queen:
		return black ? black_queen : white_queen;
	default:
		return NULL;
	}
}

void NormalGame::OnClick(int x, int y)
{
	std::cout << "Clicked: x: " << x << ", y: " << y << std::endl;

	if (x > size && x < size * 9 && y < size * 8) {
		
		if (selection > 0)
		{
			int newLocation = trunc(y / size) * 8 + trunc((x - size) / size);
			if (IsValidMove(selection, newLocation)) {
				int piece = *(tableContents + selection);
				int row = trunc(selection / 8);

				switch (abs(piece))
				{
				case ChessPiece::NewPawn:
				//case ChessPiece::Pawn:
					if ((row == 6 || row == 1) && abs(selection - newLocation) == 16) *(tableContents + newLocation) = piece;
					else *(tableContents + newLocation) = piece > 0 ? ChessPiece::Pawn : ChessPiece::Pawn * -1;
					break;
				default:
					*(tableContents + newLocation) = piece;
					break;
				}
				*(tableContents + selection) = 0;
				lastMove = newLocation;
				PlaySound((rand() % 100) > 50 ? L"sfx\\move_1.wav" : L"sfx\\move_2.wav", NULL, SND_FILENAME);
			}
			else if (IsValidAttack(selection, newLocation)) {
				int piece = *(tableContents + selection);
				int row = trunc(selection / 8);


				switch (abs(piece))
				{
				case ChessPiece::NewPawn:
					if(abs(*(tableContents + lastMove)) == ChessPiece::Pawn) 
						*(tableContents + lastMove) = 0;

					*(tableContents + newLocation) = piece > 0 ? ChessPiece::Pawn : ChessPiece::Pawn * -1;
					break;
				default:
					*(tableContents + newLocation) = piece;
					break;
				}
				*(tableContents + selection) = 0;
				lastMove = newLocation;
				PlaySound((rand() % 100) > 50 ? L"sfx\\attack_1.wav" : L"sfx\\attack_2.wav", NULL, SND_FILENAME);
			}
			selection = -1;
		}
		else {
			selection = trunc(y / size) * 8 + trunc((x - size) / size);
		}
	}
}

bool NormalGame::IsValidMove(int field, int to)
{
	int value = *(tableContents + field);
	int toValue = *(tableContents + to);
	if (toValue > 0 || toValue < 0) return false;

	switch (abs(value))
	{
	case ChessPiece::King:
	{
		//if (toValue > 0 || toValue < 0) return false;
		if ((to == field + 8 || to == field + 9 || to == field + 7 || to == field - 8 || to == field - 9 || to == field - 7 || to == field - 1 || to == field + 1)) return true;
		else return false;
	}
	case ChessPiece::Rook:
	{
		for (int i = field - 8; i >= 0; i -= 8)
		{
			if (*(tableContents + i) != ChessPiece::None) break;
			if (to == i) return true;
		}

		for (int i = field + 8; i < 64; i += 8)
		{
			if (*(tableContents + i) != ChessPiece::None) break;
			if (to == i) return true;
		}

		for (int i = field - 1; i >= (trunc(field / 8) * 8); i -= 1)
		{
			if (*(tableContents + i) != ChessPiece::None) break;
			if (to == i) return true;
		}

		for (int i = field + 1; i < (trunc(field / 8) * 8 + 8); i += 1)
		{
			if (*(tableContents + i) != ChessPiece::None) break;
			if (to == i) return true;
		}

		return false;
	}
	case ChessPiece::Knight:
	{
		int row = trunc(field / 8);
		if (
			((field - 15 == to) && (trunc((field - 15) / 8) == row - 2)) || ((field - 17 == to) && (trunc((field - 17) / 8) == row - 2)) || 
			((field - 10 == to) && (trunc((field - 10) / 8) == row - 1)) || ((field - 6 == to) && (trunc((field - 6) / 8) == row - 1)) ||
			((field + 10 == to) && (trunc((field + 10) / 8) == row + 1)) || ((field + 6 == to) && (trunc((field + 6) / 8) == row + 1)) ||
			((field + 15 == to) && (trunc((field + 15) / 8) == row + 2)) || ((field + 17 == to) && (trunc((field + 17) / 8) == row + 2))
			) return true;
		else return false;
	}
	case ChessPiece::NewPawn:
	{
		int row = trunc(field / 8);
		if ((row == 1 || row == 6) && to == (field + (value > 0 ? -16 : 16))) return true;
	}
	case ChessPiece::Pawn:
		if (to == (field + (value > 0 ? -8 : 8))) return true;
		else return false;
	default:
		return false;
	}
}

bool NormalGame::IsValidAttack(int field, int to)
{
	int value = *(tableContents + field);
	int toValue = *(tableContents + to);



	//if (value >= 0 ? (toValue >= 0) : (toValue <= 0)) return false;
	switch (abs(value))
	{
	case ChessPiece::King:
	{
		/*if ((to == field + 8 || to == field + 9 || to == field + 7 || to == field - 8 || to == field - 9 || to == field - 7 || to == field - 1 || to == field + 1)) return true;
		else return false;*/
		return false;
	}
	case ChessPiece::Knight:
	{
		int row = trunc(field / 8);
		if (toValue != ChessPiece::None && (value < 0 ? (toValue < 0) : (toValue > 0)) &&
  			((field - 15 == to) && (trunc((field - 15) / 8) == row - 2)) || ((field - 17 == to) && (trunc((field - 17) / 8) == row - 2)) ||
			((field - 10 == to) && (trunc((field - 10) / 8) == row - 1)) || ((field - 6 == to) && (trunc((field - 6) / 8) == row - 1)) ||
			((field + 10 == to) && (trunc((field + 10) / 8) == row + 1)) || ((field + 6 == to) && (trunc((field + 6) / 8) == row + 1)) ||
			((field + 15 == to) && (trunc((field + 15) / 8) == row + 2)) || ((field + 17 == to) && (trunc((field + 17) / 8) == row + 2))
			) return true;
		else return false;
	}
	case ChessPiece::Pawn:
		if (abs(field - lastMove) == 1 && (field > 0 ? (lastMove == to + 8) : (lastMove == to - 8)) && abs(*(tableContents + lastMove)) == ChessPiece::NewPawn) return true;
	case ChessPiece::NewPawn:
	{
		//if (toValue != ChessPiece::None && ((to == (field + (value > 0 ? -9 : 9)) && (field + (value > 0 ? -9 : 9) + 1) % 8 != 0) || (to == (field + (value > 0 ? -7 : 7))) && (trunc((to == (field + (value > 0 ? -7 : 7))) / 8) == trunc(field / 8)))) return true;

		if (toValue != ChessPiece::None && toValue < 0 && (trunc(to / 8) == trunc((field + (value > 0 ? -8 : 8)) / 8)) && (((to == (field + (value > 0 ? -9 : 9))) || (to == (field + (value > 0 ? -7 : 7)))))) return true;
		else return false;
		//return false;
	}
	default:
		return false;
	}
}