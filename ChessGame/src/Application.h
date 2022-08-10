/* Copyright (c) 2022 Szabadi László Zsolt
 * You should have received a copy of the GNU AGPL v3.0 license with
 * this file. If not, please write to: pliexe, or visit : https://github.com/Pliexe/VisualDiscordBotCreator/blob/master/LICENSE
 */
#pragma once

#include "pch.h"
#include "Graphics/Graphics.h"
#include "InputManager.h"
#include "GameSectionTemplate.h"

// Game Sections

#include "Sections/NormalGame.h"

class Application
{
private:
	Graphics* graphics;
	//InputManager& inputManager;
	GameSectionTemplate* currentGameSection;

	HWND hWnd;

public:

	Application();
	~Application();

	LRESULT CALLBACK WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Initialize(int nCmdShow);
	void GameLoop();
};

