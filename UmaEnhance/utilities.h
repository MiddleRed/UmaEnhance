#pragma once
#include "uma.h"

void writeFile(string file_name, char* buffer, int len);

string currentTime();

wstring s2ws(string str);

string ws2s(wstring wstr);