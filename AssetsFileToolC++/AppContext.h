#pragma once
#include "api.h"
#include "FileContext.h"
#include "AssetContainerList.h"
#include "FileContextInfo.h"
#include "AssetIterator.h"
//#include "FileModTree.h"
//#include "IAssetBatchImportDesc.h"
//#include "PluginManager.h"
#include <vector>
#include <unordered_map>
//#include <memory>
#include <stdint.h>
#include <shared_mutex>

enum EAppContextMsg
{
	AppContextMsg_OnFileOpenFail, //args: std::shared_ptr<FileOpenTask>*
	AppContextMsg_OnFileOpenAsBundle, //args: std::shared_ptr<FileOpenTask>*
	AppContextMsg_OnFileOpenAsAssets, //args: std::shared_ptr<FileOpenTask>*
	AppContextMsg_OnFileOpenAsResources, //args: std::shared_ptr<FileOpenTask>*
	AppContextMsg_OnFileOpenAsGeneric, //args: std::shared_ptr<FileOpenTask>*
	AppContextMsg_OnContainersLoaded, //args : std::shared_ptr<AssetsFileContextInfo::ContainersTask>*
	AppContextMsg_OnBundleDecompressed, //args : std::tuple<std::shared_ptr<BundleFileContextInfo::DecompressTask>,TaskResult>*
	AppContextMsg_OnAssetChanged, //args : std::tuple<unsigned int,pathid_t,bool>* (file ID; path ID; was removed?).
	AppContextMsg_DoMainThreadCallback, //args : std::tuple<void(*)(uintptr_t,uintptr_t), uintptr_t, uintptr_t>* (callback; param1; param2), created with operator new.
	AppContextMsg_OnBundleEntryChanged, //args : std::tuple<unsigned int,size_t>* (file ID; entry index).
	AppContextMsg_COUNT
};

#define AppContextErr_FileNotFound 1
class AppContext /*: TaskProgressCallback*/
{
	//std::unordered_map<unsigned int, std::shared_ptr<FileContextInfo>> contextInfoByFileID;
	std::unordered_multimap<std::string, std::shared_ptr<FileContextInfo>> contextInfoByFileName;
	std::shared_mutex contextInfoMapMutex;
	
public:
	std::unordered_map<unsigned int, std::shared_ptr<FileContextInfo>> contextInfoByFileID;

	//PluginMapping plugins;

	class FileOpenTask
	{
		std::shared_ptr<BundleFileContextInfo> pParentContextInfo; //Only to maintain object lifetime (bundled files only).

		AppContext *pContext;
		std::shared_ptr<IAssetsReader> pReader; bool readerIsModified;
		std::string filePath;
		std::string name;
		bool tryAsBundle, tryAsAssets, tryAsResources, tryAsGeneric;
	public:
		EBundleFileOpenStatus bundleOpenStatus;
		EAssetsFileOpenStatus assetsOpenStatus;
		IFileContext *pFileContext;
		std::string logText;
		unsigned int parentFileID, directoryEntryIdx; //For bundled files
		std::unique_ptr<class VisibleFileEntry> modificationsToApply; //Just carried by the task, so the open result event handler can use it later on.
		FileOpenTask(AppContext *pContext, std::shared_ptr<IAssetsReader> pReader, bool readerIsModified, const std::string &path,
			unsigned int parentFileID = 0, unsigned int directoryEntryIdx = 0, 
			bool tryAsBundle=true, bool tryAsAssets=true, bool tryAsResources=true, bool tryAsGeneric=false);
		void setParentContextInfo(std::shared_ptr<BundleFileContextInfo> &pParentContextInfo);
		const std::string &getName();
		int execute();
	};

protected:

	//void OnCompletion(std::shared_ptr<ITask> &pTask, TaskResult result);
	//void OnGenerateContainers(AssetsFileContextInfo *info);
	//void OnChangeAsset_Async(AssetsFileContextInfo *info, pathid_t pathID, bool removed);
	//void OnChangeBundleEntry_Async(BundleFileContextInfo *info, size_t index);

	/*template <class TCallable>
	requires std::invocable<TCallable, uintptr_t>
	static inline void mainThreadCallbackDispatcher(uintptr_t param1, uintptr_t param2)
	{
		if (param1 != 0)
		{
			std::unique_ptr<TCallable> pCallable(reinterpret_cast<TCallable*>(param1));
			(*pCallable)(param2);
		}
	}
	template <class TCallable>
	requires std::invocable<TCallable>
		static inline void mainThreadCallbackDispatcher(uintptr_t param1, uintptr_t param2)
	{
		if (param1 != 0)
		{
			std::unique_ptr<TCallable> pCallable(reinterpret_cast<TCallable*>(param1));
			(*pCallable)();
		}
	}*/
public:
	//TaskManager taskManager;
	//Passes a message to the main thread. On Win32, PostMessage can be used; for console, custom synchronization routines are required.
	//virtual void signalMainThread(EAppContextMsg message, void *args)=0;

	//Uses the signalMainThread mechanism to post a callback request on the main thread.
	//No guarantees are made regarding the callback timing.
	/*template <class TCallable>
	requires (std::invocable<TCallable, uintptr_t> || std::invocable<TCallable>) && std::copy_constructible<TCallable>
	inline void postMainThreadCallback(TCallable _callable, uintptr_t param = 0)
	{
		signalMainThread(AppContextMsg_DoMainThreadCallback,
			new std::tuple<void(*)(uintptr_t, uintptr_t), uintptr_t, uintptr_t>{
				&mainThreadCallbackDispatcher<TCallable>, reinterpret_cast<uintptr_t>(new TCallable(std::move(_callable))), param});
	}*/
	//Processes a message. Must be called by the main thread after a corresponding signalMainThread call.
	/*virtual*/ bool processMessage(EAppContextMsg message, void* args);
protected:
	ClassDatabasePackage classPackage; //Do not change during runtime.
	std::vector<std::shared_ptr<FileContextInfo>> contextInfo; //Only use on the main thread.
	unsigned int maxFileID;
	unsigned int lastError;
	bool autoDetectDependencies; //Automatically assign the references of newly loaded and existing AssetsFileContextInfo.
	
	//The following functions are called by processMessage.
	//FileOpenTask / BundleEntryOpenTask done
	virtual std::shared_ptr<FileContextInfo> OnFileOpenAsAssets(FileOpenTask* pTask, AssetsFileContext* pContext, EAssetsFileOpenStatus openStatus, unsigned int parentFileID, unsigned int directoryEntryIdx);

	/*virtual std::shared_ptr<FileContextInfo> OnFileOpenAsBundle(std::shared_ptr<FileOpenTask> pTask, BundleFileContext *pContext, EBundleFileOpenStatus openStatus, unsigned int parentFileID, unsigned int directoryEntryIdx);
	virtual std::shared_ptr<FileContextInfo> OnFileOpenAsResources(std::shared_ptr<FileOpenTask> pTask, ResourcesFileContext *pContext, unsigned int parentFileID, unsigned int directoryEntryIdx);
	virtual std::shared_ptr<FileContextInfo> OnFileOpenAsGeneric(std::shared_ptr<FileOpenTask> pTask, GenericFileContext *pContext, unsigned int parentFileID, unsigned int directoryEntryIdx);
	virtual void OnFileOpenFail(std::shared_ptr<FileOpenTask> pTask, std::string &logText);*/
	//AssetsFileContextInfo: ContainersTask done. First called for the assets file the ContainersTask was created for, and then for all dependencies of that assets file.
	public: 
		/*virtual */void OnUpdateContainers(AssetsFileContextInfo *info);
	//Called when an AssetsFileContextInfo reference has been resolved, or when the file name of an AssetsFile dependency entry has changed.
	//Is not called for newly loaded files that have some of their dependencies resolved immediately.
	/*virtual */void OnUpdateDependencies(AssetsFileContextInfo *info, size_t from, size_t to); //from/to: indices for info->references
protected:
	//BundleFileContextInfo: DecompressTask done or failed.
	//virtual void OnDecompressBundle(BundleFileContextInfo::DecompressTask *pTask, TaskResult result);
	//Called whenever a replacer was added (adding, modifying or removing an asset). The caller holds a reference to pFile.
	/*virtual */void OnChangeAsset(AssetsFileContextInfo *pFile, pathid_t pathID, bool wasRemoved);
	//Called whenever a bundle entry was changed (renamed, reader overridden, removed). The caller holds a reference to pFile.
	/*virtual */void OnChangeBundleEntry(BundleFileContextInfo *pFile, size_t index);
	
	//Load the class database package. appBaseDir is the directory where UABE is located with a trailing path separator.
	//Should be called early on, i.e. before any file contexts are opened.
	//Can be overridden to override default loading behaviour (look for classdata.tpk in current dir and then in appBaseDir).
	/*virtual */bool LoadClassDatabasePackage(const std::string &appBaseDir, std::string &errorMessage);

	//When inheriting this function, call AppContext::AddContextInfo() first to initialize the fileID (and to insert it into the AppContext lists)!
	/*virtual */bool AddContextInfo(std::shared_ptr<FileContextInfo> &info, unsigned int directoryEntryIdx);
	/*virtual */void RemoveContextInfo(FileContextInfo *info);
	
	//Call from the main (e.g. UI) thread only.
	//std::shared_ptr<ITask> CreateBundleEntryOpenTask(std::shared_ptr<BundleFileContextInfo> &pBundleContextInfo, unsigned int directoryEntryIdx);
public:
	//Call from the main (e.g. UI) thread only.
	//Tries to find a fileID that matches the dependency from the given file.
	//If allowSeveral is set, the function also succeeds if there are several matches (specifically, it returns the first result).
	unsigned int TryResolveDependency(AssetsFileContextInfo* pFileFrom, const AssetsFileDependency &dependency, bool allowSeveral);

	//Call from the main (e.g. UI) thread only.
	//basedOnExistingFile: If false and the file cannot be opened, open a 'zero length reader' instead of failing.
	/*std::shared_ptr<ITask> CreateFileOpenTask(const std::string &path, bool basedOnExistingFile = true);
	void OpenTask_SetModifications(ITask *pTask, std::unique_ptr<class VisibleFileEntry> modificationsToApply);*/

	FileContextInfo_ptr getContextInfo(unsigned int fileID);
	//Finds all matching loaded files by their name, optionally filtering by the location of another file.
	//Also supports "archive:/<Bundle name>/<file name>" paths.
	//Otherwise, assumes that relFileName is a simple file name without directory separators.
	// Used to resolve resource file references.
	// Example: Two resource files with the same name are loaded:
	//  - GameA_Data/sharedassets0.resources
	//  - GameB_Data/sharedassets0.resources
	//  Now, a reference from within GameB_Data/sharedassets0.assets (pFileFrom) to sharedassets0.resources is to be resolved.
	//  The result should be just the file context for GameB_Data/sharedassets0.resources.
	//  If the caller set pFileFrom to nullptr, both resources file contexts will be returned.
	std::vector<FileContextInfo_ptr> getContextInfo(const std::string &relFileName, FileContextInfo *pFileFrom = nullptr);

	//virtual bool ShowAssetBatchImportDialog(IAssetBatchImportDesc* pDesc, std::string basePath)=0;

	////Asks the user to provide an export file or directory path. An empty string signals cancelling the action.
	//// -> If assets.size() == 0, returns an empty string.
	//// -> If assets.size() == 1, the user is asked to select one output file path.
	//// -> If assets.size() >  1, the user is asked to select an output directory.
	////Assumes that all AssetIdentifiers in assets are resolved.
	//virtual std::string QueryAssetExportLocation(const std::vector<struct AssetUtilDesc>& assets,
	//	const std::string &extension, const std::string &extensionFilter) = 0;

	////Asks the user to provide an import file or directory path. An empty return value signals cancelling the action.
	////The user may possibly change the set of assets to actually import.
	////The returned vector indices correspond to the indices in the (potentially modified) assets vector.
	//// -> If assets.size() == 0, returns an empty vector.
	//// -> If assets.size() == 1, the user is asked to select one file from the filesystem.
	//// -> If assets.size() >  1, the user is asked to select an input directory and a batch import dialog is shown.
	////Assumes all assets have resolved AssetIdentifiers already.
	//virtual std::vector<std::string> QueryAssetImportLocation(std::vector<AssetUtilDesc>& assets,
	//	std::string extension, std::string extensionRegex, std::string extensionFilter) = 0;

	AppContext();
	/*virtual*/ ~AppContext();

	/*inline const PluginMapping& getPlugins()
	{
		return plugins;
	}*/

	friend class AssetsFileContextInfo;
	friend class BundleFileContextInfo;
	friend class ResourcesFileContextInfo;
};