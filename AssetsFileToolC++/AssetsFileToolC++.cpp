#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <ShlObj.h>

#include "AssetsFileReader.h"
#include "FileOpenService.h"
#include "AppContext.h"
#include <filesystem>
#include "TextAsset.cpp"
#include "WindowFunctions.h"

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

    AppContext* pAppContext = new AppContext();

    fs::path classDbDirectory = "C:\\Users\\17046\\source\\repos\\AssetsFileToolC++\\";
    std::string classDbDirectoryStr = classDbDirectory.string();
    std::string loadErrorMessage;
    pAppContext->LoadClassDatabasePackage(classDbDirectoryStr, loadErrorMessage);

    AppContext::FileOpenTask* task = new AppContext::FileOpenTask(pAppContext, pReader, false, pFilePath);
    task->execute();
    pAppContext->processMessage(AppContextMsg_OnFileOpenAsAssets, &task);

    std::shared_ptr<FileContextInfo> pFileContextInfo = pAppContext->contextInfoByFileID[1];    
    AssetsFileContextInfo* pAssetsFileContextInfo = reinterpret_cast<AssetsFileContextInfo*>(pFileContextInfo.get());
    pAssetsFileContextInfo->FindClassDatabase(pAppContext->classPackage);
    std::shared_ptr<AssetsFileContextInfo> pInfo(pAssetsFileContextInfo);

    AssetsFileTable* pAssetsFileTable = pInfo->getAssetsFileContext()->getAssetsFileTable();

    WindowFunctions* wf = new WindowFunctions();
    std::vector<WindowFunctions::ListEntry> listEntries;
    std::vector<AssetUtilDesc> audEntries;
    for (int i = 0; i < pAssetsFileTable->assetFileInfoCount; i++)
    {
        AssetIdentifier* ai = new AssetIdentifier(pInfo, i);
        ai->resolve(*pAppContext);
        WindowFunctions::ListEntry* listEntry = new WindowFunctions::ListEntry(ai->fileID, ai->pathID, true);
        listEntries.push_back(*listEntry);
        AssetUtilDesc aud = wf->makeExportDescForSelection(i, listEntries, pFileContextInfo, pAppContext, ai, pAssetsFileContextInfo);
        audEntries.push_back(aud);  
    }

    fs::path outFilePath = downloadPath / "testExport2.xml";
    std::string exportLocation = outFilePath.string();
    TextAssetExportTask* pExportTask = new TextAssetExportTask(*pAppContext, audEntries, "");
    pExportTask->exportAsset(audEntries[123], exportLocation);

    return 0;
}

