#include "FileContextInfo.h"
#include "AppContext.h"
#include <wtypes.h>
#include "AssetPluginUtil.h"
#include "WindowFunctions.h"
//#include <vector>
//#include <string>
//#include <memory>
//#include "AppContext.h"
//#include <cassert>
//
//bool WindowFunctions::OnFileEntryLoadSuccess(AppContext::FileOpenTask* pTask, std::shared_ptr<FileContextInfo>& pContextInfo, int result)
//{
//	/*if (getMenu() != NULL)
//		EnableMenuItem(getMenu(), IDM_FILE_SAVEALL, MF_ENABLED);*/
//	//auto entryIt = pendingFileEntriesByTask.find(pTask);
//	//if (entryIt == pendingFileEntriesByTask.end()
//	//	&& pTask != nullptr && pContextInfo != nullptr && pContextInfo->getFileContext()
//	//	&& pContextInfo->getParentFileID() == 0)
//	//{
//	//	this->addPendingBaseFileEntry(pTask, pContextInfo->getFileContext()->getFilePath());
//	//	entryIt = pendingFileEntriesByTask.find(pTask);
//	//}
//	//if (entryIt != pendingFileEntriesByTask.end())
//	//{
//	//	FileEntryUIInfo& entry = *(entryIt->second);
//	//	pendingFileEntriesByTask.erase(pTask);
//	//	entry.failed = false;
//	//	entry.pending = false;
//	//	entry.pContextInfo = nullptr;
//	//	entry.setContextInfo(pContextInfo);
//	//	//entry.pContextInfo = pContextInfo;
//	//	//pContextInfo->incRef(); //Reference from FileEntryUIInfo
//	//	fileEntriesByContextInfo.insert(std::make_pair(pContextInfo.get(), &entry));
//
//	//	HWND hTree = GetDlgItem(this->hDlg, IDC_TREEFILES);
//
//	//	setEntryFileID(hTree, entry.hTreeItem, pContextInfo->getFileID());
//		IFileContext* pFileContext = pContextInfo->getFileContext();
//		switch (pFileContext->getType())
//		{
//		case FileContext_Assets:
//		{
//			EAssetsFileOpenStatus openStatus = static_cast<EAssetsFileOpenStatus>(result);
//			fileEntryCountersByType[FileContext_Assets]++;
//
//
//			assert(entry.standardDialogsCount == 0);
//			assert(entry.standardDialogsCount < entry.standardDialogs.size());
//			entry.standardDialogsCount = 0;
//			FileManipulateDialogInfo& assetsDialog = entry.standardDialogs[entry.standardDialogsCount++];
//			assetsDialog.hTreeItem = entry.hTreeItem;
//			assetsDialog.pEntry = &entry;
//			assetsDialog.type = FileManipulateDialog_AssetList;
//			updateEntryInfoRef(hTree, assetsDialog.hTreeItem, assetsDialog); //Intentional so the tree item is linked to the dialog action.
//
//			assert(entry.standardDialogsCount < entry.standardDialogs.size());
//			FileManipulateDialogInfo& dependenciesDialog = entry.standardDialogs[entry.standardDialogsCount++];
//			dependenciesDialog.hTreeItem = insertEntry(hTree, entry.hTreeItem, std::string("Dependencies"));
//			dependenciesDialog.pEntry = &entry;
//			dependenciesDialog.type = FileManipulateDialog_AssetsDependencies;
//			updateEntryInfoRef(hTree, dependenciesDialog.hTreeItem, dependenciesDialog);
//
//			assert(entry.standardDialogsCount < entry.standardDialogs.size());
//			FileManipulateDialogInfo& containersDialog = entry.standardDialogs[entry.standardDialogsCount++];
//			containersDialog.hTreeItem = insertEntry(hTree, entry.hTreeItem, std::string("Containers"));
//			containersDialog.pEntry = &entry;
//			containersDialog.type = FileManipulateDialog_AssetsContainers;
//			updateEntryInfoRef(hTree, containersDialog.hTreeItem, containersDialog);
//
//			assert(entry.standardDialogsCount < entry.standardDialogs.size());
//			FileManipulateDialogInfo& altAssetsDialog = entry.standardDialogs[entry.standardDialogsCount++];
//			altAssetsDialog.hTreeItem = insertEntry(hTree, entry.hTreeItem, std::string("Assets"));
//			altAssetsDialog.pEntry = &entry;
//			altAssetsDialog.type = FileManipulateDialog_AssetList;
//			updateEntryInfoRef(hTree, altAssetsDialog.hTreeItem, altAssetsDialog);
//		}
//		break;
//		case FileContext_Bundle:
//		{
//			EBundleFileOpenStatus openStatus = static_cast<EBundleFileOpenStatus>(result);
//			std::shared_ptr<BundleFileContextInfo> pBundleInfo = std::static_pointer_cast<BundleFileContextInfo, FileContextInfo>(pContextInfo);
//
//			if (openStatus == BundleFileOpenStatus_CompressedDirectory ||
//				openStatus == BundleFileOpenStatus_CompressedData)
//			{
//				entry.pending = true;
//				entry.pContextInfo = nullptr;
//				fileEntriesByContextInfo.erase(pContextInfo.get());
//
//				if (decompressTargetDir.empty())
//				{
//					//Let the user select a decompression output directory.
//					WCHAR* folderPath = nullptr;
//					if (decompressTargetDir_cancel ||
//						!ShowFolderSelectDialog(this->hDlg, &folderPath, L"Select a decompression output directory", UABE_FILEDIALOG_FILE_GUID))
//					{
//						decompressTargetDir_cancel = (pendingFileEntriesByTask.empty()) ? false : true;
//						updateEntryName(hTree, entry.hTreeItem, std::string("Failed : ") + entry.getShortName() + " (Compressed Bundle)");
//						entry.failed = true;
//						entry.pending = false;
//						entry.openLogText += "Decompression was cancelled\n";
//						return false; //Remove the bundle from the AppContext.
//					}
//					auto folderPathUTF8 = unique_WideToMultiByte(folderPath);
//					decompressTargetDir_cancel = false;
//					decompressTargetDir.assign(folderPathUTF8.get());
//					FreeCOMFilePathBuf(&folderPath);
//				}
//				updateEntryName(hTree, entry.hTreeItem, std::string(entry.getShortName()) + " (Compressed Bundle)");
//				std::shared_ptr<ITask> pDecompressTask = pBundleInfo->EnqueueDecompressTask(*pContext, pBundleInfo,
//					decompressTargetDir + "\\" + pBundleInfo->getFileName() + "-decompressed");
//				if (pendingFileEntriesByTask.empty())
//					decompressTargetDir.clear();
//				if (pDecompressTask == nullptr)
//				{
//					entry.failed = true;
//					entry.pending = false;
//					updateEntryName(hTree, entry.hTreeItem, std::string("Failed : ") + entry.getShortName() + " (Compressed Bundle)");
//					entry.openLogText += "Failed to enqueue decompression\n";
//					return false; //Remove the bundle from the AppContext.
//				}
//				else
//				{
//					pendingFileEntriesByTask.insert(std::make_pair(pDecompressTask.get(), &entry));
//				}
//
//				return true;
//			}
//
//			fileEntryCountersByType[FileContext_Bundle]++;
//
//			BundleFileContext* pBundleContext = pBundleInfo->getBundleFileContext();
//			if (pBundleInfo->getEntryCount() > 0)
//				setHasChildren(hTree, entry.hTreeItem, true);
//			assert(pBundleInfo->getEntryCount() <= UINT_MAX);
//			for (size_t i = 0; i < pBundleInfo->getEntryCount(); i++)
//			{
//				this->loadBundleEntry(pBundleInfo, (unsigned int)i);
//			}
//
//			assert(entry.standardDialogsCount == 0);
//			assert(entry.standardDialogsCount < entry.standardDialogs.size());
//			entry.standardDialogsCount = 0;
//			FileManipulateDialogInfo& bundleDialog = entry.standardDialogs[entry.standardDialogsCount++];
//			bundleDialog.hTreeItem = entry.hTreeItem;
//			bundleDialog.pEntry = &entry;
//			bundleDialog.type = FileManipulateDialog_Bundle;
//			updateEntryInfoRef(hTree, bundleDialog.hTreeItem, bundleDialog); //Intentional so the tree item is linked to the dialog action.
//		}
//		break;
//		case FileContext_Resources:
//		{
//			EResourcesFileOpenStatus openStatus = static_cast<EResourcesFileOpenStatus>(result);
//			fileEntryCountersByType[FileContext_Resources]++;
//		}
//		break;
//		case FileContext_Generic:
//		{
//			EGenericFileOpenStatus openStatus = static_cast<EGenericFileOpenStatus>(result);
//			fileEntryCountersByType[FileContext_Generic]++;
//		}
//		break;
//		default:
//			break;
//		}
//		updateEntryName(hTree, entry.hTreeItem, formatNameFor(pContextInfo.get(), &entry));
//		if (pendingFileEntriesByTask.empty())
//		{
//			//Forget the decompress target directory once all files are loaded.
//			decompressTargetDir_cancel = false;
//			decompressTargetDir.clear();
//		}
//		if (getSelectItem(hTree, entry.hTreeItem))
//		{
//			//If the item is selected and just finished loading, the manipulate dialog has to be created or notified.
//			this->onChangeFileSelection();
//		}
//		else
//		{
//			//The selection checkboxes have to be updated in case a new file has loaded that is not selected.
//			// -> deselect IDC_CKSELALL, make type-specific check boxes indeterminate.
//			std::vector<ITreeParameter*> newSelection;
//			MC_HTREELISTITEM selection = NULL;
//			while ((selection = MCTreeList_GetNextSelection(hTree, selection)) != NULL)
//			{
//				ITreeParameter* pCurParam = getEntryParam(hTree, selection);
//				newSelection.push_back(pCurParam);
//			}
//			this->doUpdateSelectionCheckboxes(newSelection);
//		}
//		return true;
//	}
//	return false;
//}


int WindowFunctions::getContainerInfo(AppContext* pContext, AssetsFileContextInfo* pContextInfo, pathid_t pathID,
	std::string& baseName, std::string& containerListName)
{
	unsigned int fileID = pContextInfo->getFileID();
	bool hasContainerBase = false;
	uint64_t baseCount = 0;
	uint64_t dependantCount = 0;
	baseName.clear();
	containerListName.clear();
	std::string exampleContainerName;
	{
		AssetContainerList* pMainContainersList = pContextInfo->tryLockContainersRead();
		if (pMainContainersList)
		{
			std::vector<const ContainerEntry*> mainContainers = pMainContainersList->getContainers(0, pathID);
			if (mainContainers.size() > 0)
			{
				hasContainerBase = true;
				exampleContainerName = mainContainers[0]->name;
				baseCount += mainContainers.size();
			}
			std::vector<const ContainerEntry*> parentContainers = pMainContainersList->getParentContainers(0, pathID);
			if (parentContainers.size() > 0)
			{
				dependantCount += parentContainers.size();
				if (exampleContainerName.empty())
					exampleContainerName = parentContainers[0]->name;
			}
			pContextInfo->unlockContainersRead();
		}
	}
	for (size_t i = 0; i < pContextInfo->getContainerSources().size(); i++)
	{
		FileContextInfo_ptr pDepContextInfo = pContext->getContextInfo(pContextInfo->getContainerSources()[i]);
		if (pDepContextInfo != nullptr &&
			pDepContextInfo->getFileContext() &&
			pDepContextInfo->getFileContext()->getType() == FileContext_Assets)
		{
			AssetsFileContextInfo* pDepContextInfo_Assets = static_cast<AssetsFileContextInfo*>(pDepContextInfo.get());
			const std::vector<unsigned int> referenceFileIDs = pDepContextInfo_Assets->getReferences();
			auto ownIDIt = std::find(referenceFileIDs.begin(), referenceFileIDs.end(), fileID);
			if (ownIDIt != referenceFileIDs.end())
			{
				size_t relFileID = std::distance(referenceFileIDs.begin(), ownIDIt) + 1;
				assert(relFileID < UINT_MAX);

				AssetContainerList* pDepContainersList = pDepContextInfo_Assets->tryLockContainersRead();
				if (pDepContainersList)
				{
					std::vector<const ContainerEntry*> mainContainers = pDepContainersList->getContainers((unsigned int)relFileID, pathID);
					if (mainContainers.size() > 0)
					{
						if (!hasContainerBase || exampleContainerName.empty())
							exampleContainerName = mainContainers[0]->name;
						hasContainerBase = true;
						baseCount += mainContainers.size();
					}
					std::vector<const ContainerEntry*> parentContainers = pDepContainersList->getParentContainers((unsigned int)relFileID, pathID);
					if (parentContainers.size() > 0)
					{
						dependantCount += parentContainers.size();
						if (exampleContainerName.empty())
							exampleContainerName = parentContainers[0]->name;
					}
					pDepContextInfo_Assets->unlockContainersRead();
				}
			}
		}
	}
	char nameExt[128]; nameExt[0] = 0;
	if (hasContainerBase)
	{
		if (baseCount > 1)
			sprintf_s(nameExt, " (Base, %llu other base containers, %llu dependants)", baseCount - 1, dependantCount);
		else
			sprintf_s(nameExt, " (Base, %llu dependants)", dependantCount);
	}
	else if (!exampleContainerName.empty())
	{
		if (dependantCount > 1)
			sprintf_s(nameExt, " (and %llu other dependant containers)", dependantCount - 1);
		else
			strcpy_s(nameExt, " (dependant container)");
	}
	if (hasContainerBase)
	{
		auto slashIt = std::find(exampleContainerName.rbegin(), exampleContainerName.rend(), '/');
		if (slashIt != exampleContainerName.rend() && slashIt != exampleContainerName.rbegin())
			baseName.assign(slashIt.base(), exampleContainerName.end()); //<reverse_iterator>.base() returns a forward iterator for the next item (in forward direction).
		else
			baseName = exampleContainerName;
	}
	containerListName = std::move(exampleContainerName);
	if (nameExt[0] != 0)
		containerListName += nameExt;
}

struct NameTypeCacheValue
{
	DWORD nameChildIdx;
	bool hasName;
	AssetTypeTemplateField templateBase;
	inline NameTypeCacheValue()
		: nameChildIdx(0), hasName(false), templateBase()
	{}
	inline NameTypeCacheValue(DWORD nameChildIdx, AssetTypeTemplateField&& templateBase)
		: nameChildIdx(nameChildIdx), templateBase(std::move(templateBase))
	{}
	NameTypeCacheValue(const NameTypeCacheValue& other) = delete;
	inline NameTypeCacheValue(NameTypeCacheValue&& other)
		: nameChildIdx(other.nameChildIdx), hasName(other.hasName), templateBase(std::move(other.templateBase))
	{
		other.hasName = false;
	}
	NameTypeCacheValue& operator=(const NameTypeCacheValue& other) = delete;
	inline NameTypeCacheValue& operator=(NameTypeCacheValue&& other)
	{
		nameChildIdx = other.nameChildIdx;
		hasName = other.hasName;
		templateBase = std::move(other.templateBase);
		other.hasName = false;
		return (*this);
	}
};

bool WindowFunctions::TryRetrieveAssetNameField(AppContext* pContext, AssetsFileContextInfo* pContextInfo, AssetIdentifier* identifier, std::string& nameOut)
	//,	std::unordered_map<NameTypeCacheKey, NameTypeCacheValue>& nameTypesCache)
{
	if (identifier->pAssetInfo && !identifier->pReplacer)
	{
		IAssetsReader_ptr pReader(pContextInfo->getAssetsFileContext()->createReaderView(), Free_AssetsReader);

		if (identifier->pAssetInfo->ReadName(pContextInfo->getAssetsFileContext()->getAssetsFile(), nameOut, pReader.get()))
			return true;
	}
	bool ret = false;
	{
		NameTypeCacheValue* pNameTypeValue = nullptr;
		/*NameTypeCacheKey key(pContextInfo->getFileID(), identifier.getClassID(), identifier.getMonoScriptID());
		auto typeFileCacheIt = nameTypesCache.find(key);
		if (typeFileCacheIt != nameTypesCache.end())
			pNameTypeValue = &typeFileCacheIt->second;
		else*/
		{
			NameTypeCacheValue newValue;
			newValue.hasName = false;
			newValue.nameChildIdx = 0;
			if (pContextInfo->MakeTemplateField(&newValue.templateBase, *pContext, identifier->getClassID(), identifier->getMonoScriptID(), identifier))
			{
				AssetTypeTemplateField& templateBase = newValue.templateBase;
				for (DWORD i = 0; i < templateBase.children.size(); i++)
				{
					if (templateBase.children[i].name == "m_Name"
						&& (templateBase.children[i].valueType == ValueType_String))
					{
						newValue.hasName = true;
						newValue.nameChildIdx = i;
						break;
					}
				}
			}
			pNameTypeValue = &newValue;//&(nameTypesCache[key] = std::move(newValue));
		}
		if (pNameTypeValue->hasName)
		{
			IAssetsReader_ptr pReader = identifier->makeReader();
			if (pReader)
			{
				AssetTypeTemplateField* pTemplateBase = &pNameTypeValue->templateBase;
				AssetTypeValueField* pBase;

				uint32_t origChildCount = (uint32_t)pNameTypeValue->templateBase.children.size();
				std::vector<AssetTypeTemplateField> childrenTmp = std::move(pNameTypeValue->templateBase.children);
				pNameTypeValue->templateBase.children.assign(childrenTmp.begin(), childrenTmp.begin() + (pNameTypeValue->nameChildIdx + 1));
				bool bigEndian = false;
				pContextInfo->getEndianness(bigEndian);
				AssetTypeInstance instance(1, &pTemplateBase, identifier->getDataSize(), pReader.get(), bigEndian);
				pBase = instance.GetBaseField();
				if (pBase)
				{
					AssetTypeValueField* pNameField = pBase->Get("m_Name");
					if (!pNameField->IsDummy() && pNameField->GetValue())
					{
						nameOut.assign(pNameField->GetValue()->AsString());
						ret = true;
					}
				}
				pNameTypeValue->templateBase.children = std::move(childrenTmp);
			}
		}
	}
	return ret;
}

typedef std::shared_ptr<FileContextInfo> FileContextInfo_ptr;

AssetUtilDesc WindowFunctions::makeExportDescForSelection(size_t selection, std::vector<ListEntry> listEntries, FileContextInfo_ptr pContextInfo,
	AppContext* pAppContext, AssetIdentifier* identifier, AssetsFileContextInfo* pAssetFileContextInfo) {
	AssetUtilDesc ret;
	unsigned int fileID = listEntries[selection].fileID;
	pathid_t pathID = listEntries[selection].pathID;

	if (pContextInfo->getFileContext() && pContextInfo->getFileContext()->getType() == FileContext_Assets)
	{
		ret.assetsFileName = pContextInfo->getFileName();
		AssetInfo* pEntry = getAssetInfo(identifier, pAppContext, pAssetFileContextInfo, pathID);

		if (pEntry != nullptr)
			ret.assetName = pEntry->name;
		ret.asset = AssetIdentifier(reinterpret_pointer_cast<AssetsFileContextInfo>(pContextInfo), pathID);
		if (!ret.asset.resolve(*pAppContext))
			ret.asset = AssetIdentifier();
	}
	return ret;
}

WindowFunctions::AssetInfo* WindowFunctions::getAssetInfo(AssetIdentifier* identifier, AppContext* pContext, AssetsFileContextInfo* pContextInfo, pathid_t pathID)
{
	AssetInfo* info = new AssetInfo();
	info->typeID = identifier->getClassID();
	info->monoScriptID = identifier->getMonoScriptID();
	info->isModified = (identifier->pReplacer != nullptr);
	info->size = identifier->getDataSize();
	getContainerInfo(pContext, pContextInfo, pathID, info->name, info->containerName);
	info->typeName = pContextInfo->GetClassName_(*pContext, info->typeID, info->monoScriptID, identifier);
	if (info->name.empty())
		TryRetrieveAssetNameField(pContext, pContextInfo, identifier, info->name/*, nameTypesCache*/);
	if (info->typeName.empty())
	{
		char sprntTmp[20];
		sprintf_s(sprntTmp, "0x%08X", info->typeID);
		info->typeName.assign(sprntTmp);
	}

	return info;
}
