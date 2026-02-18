#pragma once

#include <Windows.h>
#include <unordered_map>
#include <array>

#include "utils.h"
#include "game/Datatypes/TesObjectRefr.h"

class FalloutMain
{
public:
	std::uintptr_t vtable;//0x0
	char padding0008[0x380];
	std::uintptr_t platformSessionManagerPtr;//0x388
};

class PlatformSessionManager
{
public:
	std::uintptr_t vtable;//0x0
	char padding0008[0x88];
	std::uintptr_t characterManagerPtr; //0x90
	std::uintptr_t clientAccountManagerPtr;//0x98
	std::uintptr_t entitlementMappingsManagerPtr;//0xA0
	std::uintptr_t groupMatchmakingManagerPtr;//0xA8
	std::uintptr_t clientLobbyDataPtr;//0xB0
};
// Updated offsets for version 1.7.23.11 0x4B0 to 0x4C0
class BhkCharProxyController
{
public:
	std::uintptr_t vtable;//0x0
	BYTE padding0008[0x4B8];
	std::uintptr_t hknpBsCharacterProxyPtr;//0x4C0
};

class HknpBsCharacterProxy
{
public:
	std::uintptr_t vtable;//0x0
	BYTE padding0008[0x48];
	float rotation[2];//0x50
	BYTE padding0058[0x18];
	Vector3 position;//0x70
	BYTE padding007C[0x24];
	float velocityA[4];//0xA0
	float velocityB[4];//0xB0
};
class ClientAccountManager
{
public:
	std::uintptr_t vtable;//0x0
	BYTE padding0008[0x38];
	std::uintptr_t clientAccountArrayPtr;//0x40
	BYTE padding0048[0x8];
	int clientAccountArraySizeA;//0x50
	BYTE padding0054[0x4];
	int clientAccountArraySizeB;//0x58
};

class LeveledList
{
public:
	std::uintptr_t vtable;//0x0
	char padding0008[0x18];
	std::uint32_t formId;//0x20
	char padding0024[0x8C];
	std::uintptr_t listEntryArrayPtr;//0xB0
	char padding00B8[0x9];
	BYTE listEntryArraySize;//0xC1
};

class ClientAccountBuffer
{
public:
	std::uintptr_t namePtr;//0x0
	std::uintptr_t clientAccountPtr;//0x8
};

class ClientAccount
{
public:
	std::uintptr_t vtable;//0x0
	char padding0008[0x8];
	BYTE nameData[0x10];//0x10
	int nameLength;//0x20
	char padding0024[0x58];
	std::uint32_t formId;//0x7C
};

class Component
{
public:
	std::uintptr_t componentReferencePtr;//0x0
	std::uintptr_t componentCountReferencePtr;//0x8
};

class ReferenceList
{
public:
	std::uintptr_t arrayPtr;//0x0
	char padding0008[0x8];
	int arraySize;//0x10
};

class ItemCount
{
public:
	char padding0000[0x8];
	int count;//0x8
	char padding000C[0x4];
};

class ItemInstancedArray
{
public:
	char padding0000[0x10];
	std::uintptr_t arrayPtr;//0x10
	std::uintptr_t arrayEnd;//0x18
};

class ExtraTextDisplayData
{
public:
	std::uintptr_t vtable;//0x0
	char padding0008[0x8];
	std::uintptr_t instancedNamePtr;//0x10
};

class BgsQuestText
{
public:
	std::uintptr_t vtable;//0x0
	std::uintptr_t formIdPtr;//0x8
	char padding0010[0x28];
	std::uintptr_t codePtr;//0x38
};

class ListEntry
{
public:
	std::uintptr_t referencePtr;//0x0
	std::uintptr_t extraData;//0x8
	std::uintptr_t conditions;//0x10
	float quantity;//0x18
	std::uint32_t quantityPadding;//0x1C
	std::uintptr_t quantityGlobal;//0x20
	std::uintptr_t quantityGlobalPadding;//0x28;
	float chanceNoneValue;//0x30
	std::uint32_t chanceNoneValuePadding;//0x34
	std::uintptr_t chanceNoneGlobal;//0x38
	std::uintptr_t chanceNoneCurveTable;//0x40
	float minimumLevel;//0x48
	std::uint32_t minimumLevelPadding;//0x4C
	std::uintptr_t minimumLevelGlobal;//0x50
	std::uintptr_t minimumLevelCurveTable;//0x58
	std::uintptr_t minimumLevelCurveTablePadding;//0x68
};

class ModInstance
{
public:
	std::uintptr_t modListPtr;//0x0
	int modListSize;//0x8
};

class ObjectInstanceExtra
{
public:
	char padding0000[0x10];
	std::uintptr_t modDataPtr;//0x10
};

class Chargen
{
public:
	char padding[0x28];
	float thin;//0x28
	float muscular;//0x2C
	float large;//0x30
};

class CustomEntry
{
public:
	std::uintptr_t entityPtr;
	std::uintptr_t baseObjectPtr;
	std::uint32_t entityFormId;
	std::uint32_t baseObjectFormId;
	std::uintptr_t flag;
	std::string name;
};

enum class ItemTypes
{
	Invalid,
	Other,

	Npc,
	Container,
	Flora,

	Weapons,
	Apparel,

	Misc,

	Aid,
	AidBobblehead,
	AidMagazine,

	NotesKnownPlan,
	NotesUnknownPlan,
	NotesTreasureMap,
	Notes,

	Mod,
	Junk,
	Ammo,
	Currency,
};

struct ItemInfo
{
	ItemTypes type;

	TesObjectRefr refr;
	TesItem base;
};

class ErectusMemory final {
public:
	static std::uintptr_t GetAddress(std::uint32_t formId);
	static std::uintptr_t GetPtr(std::uint32_t formId);

	//send damage
	static std::unordered_map<int, std::string> GetFavoritedWeapons();
	static std::uint32_t GetFavoritedWeaponId(BYTE favouriteIndex);
	static char FavoriteIndex2Slot(BYTE favoriteIndex);

	//nuke codes
	static void UpdateNukeCodes();
	inline static std::array<int, 8> alphaCode = { };
	inline static std::array<int, 8> bravoCode = { };
	inline static std::array<int, 8> charlieCode = { };

	//teleporter
	static bool SaveTeleportPosition(int index);
	static bool RequestTeleport(int index);

	//noclip
	static void Noclip(bool enabled);

	//esp
	static bool UpdateBufferEntityList();
	static bool UpdateBufferPlayerList();
	inline static std::vector<CustomEntry> entityDataBuffer{};
	inline static std::vector<CustomEntry> playerDataBuffer{};

	static bool ReferenceSwap(std::uint32_t& sourceFormId, std::uint32_t& destinationFormId);

	static bool ChargenEditing();

	static bool IsFloraHarvested(char harvestFlagA, char harvestFlagB);

	static bool IsTargetValid(std::uintptr_t targetPtr);
	static bool IsTargetValid(const TesObjectRefr& targetData);

	inline static std::uintptr_t targetLockedEntityPtr = 0;

	static void    GetCustomEntityData(const TesItem& baseObject, std::uintptr_t& entityFlag, int& enabledDistance);
	static bool    CheckFormIdArray(std::uint32_t formId, const bool* enabledArray, const std::uint32_t* formIdArray, int size);
	static std::uintptr_t RttiGetNamePtr(std::uintptr_t vtable);
	static bool    VtableSwap(std::uintptr_t dst, std::uintptr_t src);

	static ItemInfo GetItemInfo(const TesObjectRefr& entity);
	static std::string GetEntityName(std::uintptr_t ptr);

	static bool CheckReferenceKeywordMisc(const TesItem& referenceData, std::uint32_t formId);
	static bool CheckReferenceKeywordBook(const TesItem& referenceData, std::uint32_t formId);

private:
	[[nodiscard]] static std::uint32_t GenerateCrc32(std::uint32_t formId) noexcept;

	static std::array<int, 8> GetNukeCode(std::uint32_t formId);
	static std::string GetPlayerName(const ClientAccount& clientAccountData);

	static bool CheckWhitelistedFlux(const TesItem& referenceData);
	
	static bool IsItem(const TesItem& referenceData);
	static bool IsRecipeKnown(std::uint32_t formId);

	static bool MovePlayer();

	static std::uint32_t GetEntityId(const TesObjectRefr& entityData);
	static std::uintptr_t GetNukeCodePtr(std::uint32_t formId);
	static std::string GetInstancedItemName(std::uintptr_t displayPtr);

	virtual void Dummy() = 0;
};


