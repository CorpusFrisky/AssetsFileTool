#pragma once
#include <locale>
#include "AssetsFileReader.h"
#include "FileContext.h"

class FileOpenService
{
//	std::shared_ptr<BundleFileContextInfo> pParentContextInfo; //Only to maintain object lifetime (bundled files only).
//
//	AppContext* pContext;
	std::shared_ptr<IAssetsReader> pReader; bool readerIsModified;
	std::string filePath;
//	std::string name;
//	bool tryAsBundle, tryAsAssets, tryAsResources, tryAsGeneric;
public:
	EBundleFileOpenStatus bundleOpenStatus;
	EAssetsFileOpenStatus assetsOpenStatus;
	IFileContext* pFileContext;
//	std::string logText;
//	unsigned int parentFileID, directoryEntryIdx; //For bundled files
//	std::unique_ptr<class VisibleFileEntry> modificationsToApply; //Just carried by the task, so the open result event handler can use it later on.
	FileOpenService(std::shared_ptr<IAssetsReader> pReader, bool readerIsModified, const std::string& path,
		unsigned int parentFileID = 0, unsigned int directoryEntryIdx = 0,
		bool tryAsBundle = true, bool tryAsAssets = true, bool tryAsResources = true, bool tryAsGeneric = false);
//	UABE_Generic_API void setParentContextInfo(std::shared_ptr<BundleFileContextInfo>& pParentContextInfo);
//	UABE_Generic_API const std::string& getName();
	int execute();
};