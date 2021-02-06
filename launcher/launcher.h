#pragma once

#include <windows.h>
#include <shlobj.h>
#pragma comment(lib, "shell32.lib")

#include <cstdio>
#include <tlhelp32.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "json.hpp"

int Launch(std::string dll);
int Error(HANDLE process, const char *fmt);
DWORD GetDungeonsProcessID();
DWORD GetProcessID(const char *exeFile);
bool LaunchGame();
bool LaunchStandaloneGame();
bool LaunchWinstoreGame();
bool HasModule(const DWORD &pid, const char *moduleName);
