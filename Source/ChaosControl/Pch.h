#pragma once

#define WIN32_LEAN_AND_MEAN

// Detours
#include <Windows.h>
#include <detours.h>
#include <tchar.h>
// BlueBlur must be included before STL


#include <BlueBlur.h>

// Standard library
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <deque>
#include <string>

// Dependencies
#include <Helpers.h>
#include <INIReader.h>
#include <StringHelper.h>


#define EXPORT extern "C" __declspec(dllexport)
#define ASMHOOK __declspec(naked)