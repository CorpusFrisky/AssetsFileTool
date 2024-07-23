#include <iostream>
#include <fstream>
#include <string>

#include "AssetsFileReader.h"
#include "FileOpenService.h"
#include "AppContext.h"


const char* filePath = "C:\\Users\\17046\\Downloads\\sharedassets0-reliable_shop_v1.1.assets";

int main() {
    std::shared_ptr<IAssetsReader> pReader = std::shared_ptr<IAssetsReader>(
        Create_AssetsReaderFromFile(filePath, true, RWOpenFlags_Immediately),
        Free_AssetsReader);

    /*FileOpenService *fileOpenService = new FileOpenService(std::move(pReader), false, filePath, 0, 0, true, true, false, false);
    fileOpenService->execute();*/
    /*AssetsFileTable *fileTable = ((AssetsFileContext*)fileOpenService->pFileContext)->getAssetsFileTable();
    AssetFileInfoEx* pFileInfo = fileTable->getAssetInfo(114);*/

    AppContext* pAppContext = new AppContext(); \
        //auto ppTask = (std::shared_ptr<FileOpenTask>*)args;
        //AppContext *pContext, std::shared_ptr<IAssetsReader> pReader, bool readerIsModified, const std::string &path,
    AppContext::FileOpenTask* task = new AppContext::FileOpenTask(pAppContext, pReader, false, filePath);
    task->execute();
    pAppContext->processMessage(AppContextMsg_OnFileOpenAsAssets, &task);

    return 0;
}

