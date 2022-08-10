/* Copyright (c) 2022 Szabadi László Zsolt
 * You should have received a copy of the GNU AGPL v3.0 license with
 * this file. If not, please write to: pliexe, or visit : https://github.com/Pliexe/VisualDiscordBotCreator/blob/master/LICENSE
 */
#pragma once

// Standard Library

#include <iostream>
#include <string>
#include <cmath>
#include <functional>
#include <vector>

// Windows

#include <Windowsx.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>

// Helpers

// https://docs.microsoft.com/en-us/windows/win32/medfound/saferelease
template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

// Helper function for mouse

//bool mouseWithinBounds() {
//    return false;
//}

// Exit Codes

#define GRAPHICS_ERROR_FACTORY_CREATION -2000
#define GRAPHICS_ERROR_RENDER_TARGET_CREATION -2001
#define GRAPHICS_ERROR_WRITETEXTFORMAT_CREATION -2002

#define GRAPHICS_ERROR(code) { std::cout << "GRAPHICS ERROR: " << code << std::endl; }

// Cheeky hack

//GameSectionTemplate* currentGameSection;