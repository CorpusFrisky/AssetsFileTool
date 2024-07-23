#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <ShlObj.h>

#include "AssetsFileReader.h"
#include "FileOpenService.h"
#include "AppContext.h"
#include <filesystem>

namespace fs = std::filesystem;


std::wstring GetDownloadsFolder()
{
    wchar_t* path = nullptr;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &path);
    if (SUCCEEDED(hr))
    {
        std::wstring downloadsPath(path);
        CoTaskMemFree(path); // Free the allocated memory
        return downloadsPath;
    }
    else
    {
        // Handle error (e.g., fallback to a default path)
        return L"";
    }
}

int main() {
    fs::path downloadPath = GetDownloadsFolder();
    fs::path filePath = downloadPath / "sharedassets0-reliable_shop_v1.1.assets";
    std::string filePathStr = filePath.string();
    const char* pFilePath = filePathStr.c_str();

    std::shared_ptr<IAssetsReader> pReader = std::shared_ptr<IAssetsReader>(
        Create_AssetsReaderFromFile(pFilePath, true, RWOpenFlags_Immediately),
        Free_AssetsReader);

    AppContext* pAppContext = new AppContext(); \
    AppContext::FileOpenTask* task = new AppContext::FileOpenTask(pAppContext, pReader, false, pFilePath);
    task->execute();
    pAppContext->processMessage(AppContextMsg_OnFileOpenAsAssets, &task);

    return 0;
}

