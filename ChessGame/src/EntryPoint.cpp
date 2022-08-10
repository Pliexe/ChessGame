/* Copyright (c) 2022 Szabadi László Zsolt
 * You should have received a copy of the GNU AGPL v3.0 license with
 * this file. If not, please write to: pliexe, or visit : https://github.com/Pliexe/VisualDiscordBotCreator/blob/master/LICENSE
 */
#include "pch.h"
#include "Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE legacyPrevInstance, LPWSTR cmd, int nCmdShow)
{
	bool consoleEnabled = false;

#ifndef _DEBUG
	if (__argc > 1) {
		for (int i = 0; i < __argc; i++)
			if (strcmp(__argv[i], "-debug") || strcmp(__argv[i], "-console") || strcmp(__argv[i], "-logs"))
			{
#endif
				consoleEnabled = true;

				AllocConsole();

				SetConsoleTitle(L"Debug Console");

				freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
				freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);

#ifndef _DEBUG
			}
	}
#endif

	/*FILE* o;
	FILE* e;

	freopen_s(&o, "logs.txt", "w", stdout);
	freopen_s(&e, "error.txt", "w", stdout);*/

	/*if (!consoleEnabled) {
		

		
	}*/

	std::cout << "Starting Application" << std::endl;

	Application* app = new Application();

	app->Initialize(nCmdShow);
	app->GameLoop();

	delete app;
}