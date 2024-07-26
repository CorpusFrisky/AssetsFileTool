//#include <string>
//
//#include "FileEntryUIInfo.h"
//
//
//FileEntryUIInfo::FileEntryUIInfo(/*MC_HTREELISTITEM hTreeItem, */const std::string& fullName, bool isFilePath) :
//	failed(false), pending(true), pContextInfo(nullptr), shortNameIndex(0),
//	/*hTreeItem(hTreeItem),*/ standardDialogsCount(0)
//{
//	b_isFileManipulateDialogInfo = false;
//	this->fullName.assign(fullName);
//	if (isFilePath)
//	{
//		const char* fullNameC = this->fullName.c_str();
//		for (shortNameIndex = fullName.size(); shortNameIndex > 0; shortNameIndex--)
//		{
//			if (fullNameC[shortNameIndex - 1] == '/' || fullNameC[shortNameIndex - 1] == '\\')
//				break;
//		}
//	}
//}
//FileEntryUIInfo::~FileEntryUIInfo()
//{
//	//Note that the destructor is also called during emplace_back, i.e. after a move constructor call.
//	if (!pending)
//		setContextInfo(std::shared_ptr<FileContextInfo>(nullptr));
//}
//
