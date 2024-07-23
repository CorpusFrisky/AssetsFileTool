#include "FileOpenService.h"
#include "FileContext.h"

FileOpenService::FileOpenService(std::shared_ptr<IAssetsReader> _pReader, bool readerIsModified, const std::string& path,
	unsigned int parentFileID, unsigned int directoryEntryIdx,
	bool tryAsBundle, bool tryAsAssets, bool tryAsResources, bool tryAsGeneric) 
{
	this->pReader = _pReader;
	this->readerIsModified = readerIsModified;
	this->filePath = filePath;
	
	//name = "Open file: " + path;
}

int FileOpenService::execute()
{
	//TODO (maybe) : make this task cancelable
	this->pFileContext = nullptr;
	this->bundleOpenStatus = (EBundleFileOpenStatus)0;
	this->assetsOpenStatus = (EAssetsFileOpenStatus)0;
	/*if (tryAsBundle)
	{
		progressManager.setProgressDesc("Opening as a bundle file");
		BundleFileContext* pBundleContext = new BundleFileContext(filePath, pReader, readerIsModified);
		EBundleFileOpenStatus bundleOpenStatus = pBundleContext->OpenInsideTask(&progressManager, 0, 200);
		if (bundleOpenStatus >= 0 && bundleOpenStatus != BundleFileOpenStatus_Pend)
		{
			progressManager.setProgress(200, 200);
			this->pFileContext = pBundleContext;
			this->bundleOpenStatus = bundleOpenStatus;
			return 1;
		}
		delete pBundleContext;
	}
	if (tryAsAssets)*/
	{
		AssetsFileContext* pAssetsContext = new AssetsFileContext(filePath, pReader, readerIsModified);
		EAssetsFileOpenStatus assetsOpenStatus = pAssetsContext->OpenInsideTask(true, 100, 200);
		if (assetsOpenStatus >= 0 && assetsOpenStatus != AssetsFileOpenStatus_Pend)
		{
			this->pFileContext = pAssetsContext;
			this->assetsOpenStatus = assetsOpenStatus;
			return 2;
		}
		delete pAssetsContext;
	}
	//if (tryAsResources/* && ((filePath.size() >= 5 && !filePath.compare(filePath.size() - 5, std::string::npos, ".resS"))
	//	|| (filePath.size() >= 9 && !filePath.compare(filePath.size() - 9, std::string::npos, ".resource"))
	//	|| (filePath.size() >= 10 && !filePath.compare(filePath.size() - 10, std::string::npos, ".resources")))*/)
	//{
	//	progressManager.setProgressDesc("Opening as a resources file");
	//	ResourcesFileContext* pResourcesContext = new ResourcesFileContext(filePath, pReader, readerIsModified);
	//	EResourcesFileOpenStatus resourcesOpenStatus = pResourcesContext->Open();
	//	if (resourcesOpenStatus >= 0)
	//	{
	//		progressManager.setProgress(200, 200);
	//		this->pFileContext = pResourcesContext;
	//		return 3;
	//	}
	//	delete pResourcesContext;
	//}
	//if (tryAsGeneric)
	//{
	//	progressManager.setProgressDesc("Opening as a generic file");
	//	GenericFileContext* pGenericContext = new GenericFileContext(filePath, pReader, readerIsModified);
	//	EGenericFileOpenStatus genericOpenStatus = pGenericContext->Open();
	//	if (genericOpenStatus >= 0)
	//	{
	//		progressManager.setProgress(200, 200);
	//		this->pFileContext = pGenericContext;
	//		return 4;
	//	}
	//	delete pGenericContext;
	//}
	this->pReader.reset();
	return -1;
}