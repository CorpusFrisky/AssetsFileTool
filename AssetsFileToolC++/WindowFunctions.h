#pragma once
#include "AppContext.h"

class WindowFunctions
{
public:
	struct ListEntry
	{
		unsigned int fileID;
		pathid_t pathID;
		bool isSelected;
		ListEntry(unsigned int fileID, pathid_t pathID, bool isSelected = false)
			: fileID(fileID), pathID(pathID), isSelected(isSelected)
		{}
	};

	struct AssetInfo
	{
		std::string name; //(from the m_Name field)
		std::string containerName;
		std::string typeName;
		uint64_t size;
		uint32_t typeID;
		uint16_t monoScriptID;
		bool isModified; //Has a replacer.
		inline size_t getSize()
		{
			//Rough approximation (overcounts or undercounts the std::string memory size).
			return name.size() + containerName.size() + typeName.size() + sizeof(AssetInfo);
		}
	};

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

	static int getContainerInfo(AppContext* pContext, AssetsFileContextInfo* pContextInfo, pathid_t pathID,
		std::string& baseName, std::string& containerListName);

	bool TryRetrieveAssetNameField(AppContext* pContext, AssetsFileContextInfo* pContextInfo, AssetIdentifier* identifier, std::string& nameOut);
	AssetUtilDesc makeExportDescForSelection(size_t selection, std::vector<ListEntry> listEntries, FileContextInfo_ptr pContextInfo,
		AppContext* pAppContext, AssetIdentifier* identifier, AssetsFileContextInfo* pAssetFileContextInfo);
	AssetInfo* getAssetInfo(AssetIdentifier* identifier, AppContext* pContext, AssetsFileContextInfo* pContextInfo, pathid_t pathID);

};