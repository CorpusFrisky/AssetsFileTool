//#pragma once
//#include <list>
//#include <memory>
//#include "FileContextInfo.h"
//	
//
//class ITreeParameter
//{
//public:
//	bool b_isFileManipulateDialogInfo;
//	inline class FileManipulateDialogInfo* asFileManipulateDialogInfo()
//	{
//		if (isFileManipulateDialogInfo())
//			return (class FileManipulateDialogInfo*)this;
//		return nullptr;
//	}
//	inline class FileEntryUIInfo* asFileEntryInfo()
//	{
//		if (isFileEntryInfo())
//			return (class FileEntryUIInfo*)this;
//		return nullptr;
//	}
//	inline bool isFileManipulateDialogInfo() const { return b_isFileManipulateDialogInfo; }
//	inline bool isFileEntryInfo() const { return !b_isFileManipulateDialogInfo; }
//};
//
//class FileEntryUIInfo : public ITreeParameter
//{
//	size_t shortNameIndex;
//public:
//	//Pending file entry constructor
//	FileEntryUIInfo(/*MC_HTREELISTITEM hTreeItem, */const std::string& fullName, bool isFilePath = true);
//	~FileEntryUIInfo();
//	//Iterator for the std::list this is stored in. Needs to be set manually after inserting the entry into the list!
//	std::list<FileEntryUIInfo>::iterator myiter;
//	bool failed;
//	bool pending; //If true, implies that pContextInfo is null and that there are no child entries in the tree.
//	//Note that hTreeItem may appear in the first standardDialogs entry.
//	//MC_HTREELISTITEM hTreeItem;
//	std::shared_ptr<FileContextInfo> pContextInfo;
//	//ITask* pTask;
//
//	//No duplicate hTreeItems are allowed. Only the first entry in standardDialogs may use the file entry's hTreeItem.
//	size_t standardDialogsCount;
//	//std::array<FileManipulateDialogInfo, 6> standardDialogs;
//	std::list<FileManipulateDialogInfo> subDialogs;
//
//	std::string openLogText;
//	std::string fullName;
//public:
//	class DialogsIterator
//	{
//		FileEntryUIInfo& entryInfo;
//		size_t standardDialogsI;
//		std::list<FileManipulateDialogInfo>::iterator iter;
//	public:
//		inline DialogsIterator(FileEntryUIInfo& entryInfo)
//			: entryInfo(entryInfo), standardDialogsI(0), iter(entryInfo.subDialogs.begin())
//		{}
//		inline FileManipulateDialogInfo* operator->() { return &**this; }
//		inline FileManipulateDialogInfo& operator*()
//		{
//			/*if (standardDialogsI < entryInfo.standardDialogsCount)
//				return entryInfo.standardDialogs[standardDialogsI];*/
//			return *iter;
//		}
//		inline DialogsIterator& operator++()
//		{
//			standardDialogsI++;
//			if (standardDialogsI > entryInfo.standardDialogsCount)
//				iter++;
//			return *this;
//		}
//		inline bool end()
//		{
//			return (standardDialogsI >= entryInfo.standardDialogsCount && iter == entryInfo.subDialogs.end());
//		}
//	};
//	inline const char* getShortName()
//	{
//		return &(fullName.c_str()[shortNameIndex]);
//	}
//	inline void setContextInfo(std::shared_ptr<FileContextInfo> pContextInfo)
//	{
//		if (!pending)
//			this->pContextInfo = pContextInfo;
//		else
//			assert(false);
//	}
//	inline std::shared_ptr<FileContextInfo> getContextInfo()
//	{
//		if (!pending) return pContextInfo;
//		return nullptr;
//	}
//	inline FileContextInfo* getContextInfoPtr()
//	{
//		if (!pending) return pContextInfo.get();
//		return nullptr;
//	}
//	/*inline ITask* getTask()
//	{
//		if (pending) return pTask;
//		return nullptr;	
//	}*/
//	inline DialogsIterator getDialogsIterator()
//	{
//		return DialogsIterator(*this);
//	}
//	//Updates the name for inner files (esp. bundle entries).
//	inline void updateName(std::string newName)
//	{
//		if (shortNameIndex == 0)
//			fullName = std::move(newName);
//	}
//};