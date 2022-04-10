#include "utilities.h"

void writeFile(string file_name, char* buffer, int len)
{
	FILE* fp;
	fopen_s(&fp, file_name.c_str(), "wb");
	if (fp != nullptr)
	{
		fwrite(buffer, 1, len, fp);
		fclose(fp);
	}
}

string currentTime()
{
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	return std::to_string(ms.count());
}


// String to Wstring  
wstring s2ws(string str)
{
    wstring result;
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
    TCHAR* buffer = new TCHAR[len + 1];
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
    buffer[len] = '\0'; 
    result.append(buffer);
    delete[] buffer;
    return result;
}

// Wstring to String  
string ws2s(wstring wstr)
{
    string result;
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    char* buffer = new char[len + 1];
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    buffer[len] = '\0';
    result.append(buffer);
    delete[] buffer;
    return result;
}