/* Copyright (c) 2022 Szabadi László Zsolt
 * You should have received a copy of the GNU AGPL v3.0 license with
 * this file. If not, please write to: pliexe, or visit : https://github.com/Pliexe/VisualDiscordBotCreator/blob/master/LICENSE
 */
#pragma once

#include "pch.h"
#include "Graphics/Graphics.h"
#include "Event.h"

class GameSectionTemplate {
protected:

	Graphics* graphics;
	HWND hWnd;

public:
	GameSectionTemplate() = delete;
	GameSectionTemplate(const GameSectionTemplate&) = delete;
	GameSectionTemplate(Graphics* graphics, HWND hWnd)
	{
		this->graphics = graphics;
		this->hWnd = hWnd;
	}

	virtual bool Init() = 0;

	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void OnClick(int x, int y) = 0;

	//virtual void OnClick(RECT rect) = 0;
};