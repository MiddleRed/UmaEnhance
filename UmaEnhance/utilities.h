#pragma once
#include <string>
#include "src/thread-pool/include/ThreadPool.h"
#include <Windows.h>
// You should not include any other not-library cpp file there.

void writeFile(std::string file_name, char* buffer, int len);

std::string currentTime();

std::wstring s2ws(std::string str);
std::string ws2s(std::wstring wstr);

extern ThreadPool threadPool;
void initThreadPool();