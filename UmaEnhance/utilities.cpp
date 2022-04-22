#include "utilities.h"

void writeFile(std::string file_name, char* buffer, int len)
{
	FILE* fp;
	fopen_s(&fp, file_name.c_str(), "wb");
	if (fp != nullptr)
	{
		fwrite(buffer, 1, len, fp);
		fclose(fp);
	}
}

std::string currentTime()
{
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
	return std::to_string(ms.count());
}

// String to Wstring  
std::wstring s2ws(std::string str)
{
    std::wstring result;
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), NULL, 0);
    TCHAR* buffer = new TCHAR[len + 1];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.size(), buffer, len);
    buffer[len] = '\0'; 
    result.append(buffer);
    delete[] buffer;
    return result;
}
// Wstring to String  
std::string ws2s(std::wstring wstr)
{
    std::string result;
    int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    char* buffer = new char[len + 1];
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    buffer[len] = '\0';
    result.append(buffer);
    delete[] buffer;
    return result;
}

ThreadPool threadPool(2);
bool isThreadPoolInit = false;
void initThreadPool()
{
    if (not isThreadPoolInit)
    {
        isThreadPoolInit = true;
        threadPool.init();
    }
}
