#pragma once


enum class EGameDifficulty : uint8_t
{
	EGameDifficulty__Invalid       = 0,
	EGameDifficulty__Difficulty    = 1,
	EGameDifficulty__Difficulty01  = 2,
	EGameDifficulty__Difficulty02  = 3,
	EGameDifficulty__ENUM_COUNT    = 4,
	EGameDifficulty__EGameDifficulty_MAX = 5
};

enum class ELevelNames : uint8_t
{
	ELevelNames__Invalid           = 0,
	ELevelNames__squidcoast        = 1,
	ELevelNames__creeperwoods      = 2,
	ELevelNames__pumpkinpastures   = 3,
	ELevelNames__soggyswamp        = 4,
	ELevelNames__mooncorecaverns   = 5,
	ELevelNames__fieryforge        = 6,
	ELevelNames__deserttemple      = 7,
	ELevelNames__slimysewers       = 8,
	ELevelNames__highblockhalls    = 9,
	ELevelNames__obsidianpinnacle  = 10,
	ELevelNames__cacticanyon       = 11,
	ELevelNames__creeperwoodsdaily = 12,
	ELevelNames__creeperwoodsweekly = 13,
	ELevelNames__creeperwoodsseasonal = 14,
	ELevelNames__pumpkinpasturesdaily = 15,
	ELevelNames__pumpkinpasturesweekly = 16,
	ELevelNames__pumpkinpasturesseasonal = 17,
	ELevelNames__soggyswampdaily   = 18,
	ELevelNames__soggyswampweekly  = 19,
	ELevelNames__soggyswampseasonal = 20,
	ELevelNames__mooncorecavernsdaily = 21,
	ELevelNames__mooncorecavernsweekly = 22,
	ELevelNames__mooncorecavernsseasonal = 23,
	ELevelNames__fieryforgedaily   = 24,
	ELevelNames__fieryforgeweekly  = 25,
	ELevelNames__fieryforgeseasonal = 26,
	ELevelNames__deserttempledaily = 27,
	ELevelNames__deserttempleweekly = 28,
	ELevelNames__deserttempleseasonal = 29,
	ELevelNames__slimysewersdaily  = 30,
	ELevelNames__slimysewersweekly = 31,
	ELevelNames__slimysewersseasonal = 32,
	ELevelNames__highblockhallsdaily = 33,
	ELevelNames__highblockhallsweekly = 34,
	ELevelNames__highblockhallsseasonal = 35,
	ELevelNames__obsidianpinnacledaily = 36,
	ELevelNames__obsidianpinnacleweekly = 37,
	ELevelNames__obsidianpinnacleseasonal = 38,
	ELevelNames__cacticanyondaily  = 39,
	ELevelNames__cacticanyonweekly = 40,
	ELevelNames__cacticanyonseasonal = 41,
	ELevelNames__creepycrypt       = 42,
	ELevelNames__soggycave         = 43,
	ELevelNames__underhalls        = 44,
	ELevelNames__archhaven         = 45,
	ELevelNames__lowertemple       = 46,
	ELevelNames__mooshroomisland   = 47,
	ELevelNames__dingyjungle       = 48,
	ELevelNames__overgrowntemple   = 49,
	ELevelNames__bamboobluff       = 50,
	ELevelNames__frozenfjord       = 51,
	ELevelNames__lonelyfortress    = 52,
	ELevelNames__lostsettlement    = 53,
	ELevelNames__creepycryptdaily  = 54,
	ELevelNames__creepycryptweekly = 55,
	ELevelNames__creepycryptseasonal = 56,
	ELevelNames__soggycavedaily    = 57,
	ELevelNames__soggycaveweekly   = 58,
	ELevelNames__soggycaveseasonal = 59,
	ELevelNames__underhallsdaily   = 60,
	ELevelNames__underhallsweekly  = 61,
	ELevelNames__underhallsseasonal = 62,
	ELevelNames__archhavendaily    = 63,
	ELevelNames__archhavenweekly   = 64,
	ELevelNames__archhavenseasonal = 65,
	ELevelNames__lowertempledaily  = 66,
	ELevelNames__lowertempleweekly = 67,
	ELevelNames__lowertempleseasonal = 68,
	ELevelNames__mooshroomislanddaily = 69,
	ELevelNames__mooshroomislandweekly = 70,
	ELevelNames__mooshroomislandseasonal = 71,
	ELevelNames__dingyjungledaily  = 72,
	ELevelNames__dingyjungleweekly = 73,
	ELevelNames__dingyjungleseasonal = 74,
	ELevelNames__overgrowntempledaily = 75,
	ELevelNames__overgrowntempleweekly = 76,
	ELevelNames__overgrowntempleseasonal = 77,
	ELevelNames__bamboobluffdaily  = 78,
	ELevelNames__bamboobluffweekly = 79,
	ELevelNames__bamboobluffseasonal = 80,
	ELevelNames__frozenfjorddaily  = 81,
	ELevelNames__frozenfjordweekly = 82,
	ELevelNames__frozenfjordseasonal = 83,
	ELevelNames__lonelyfortressdaily = 84,
	ELevelNames__lonelyfortressweekly = 85,
	ELevelNames__lonelyfortressseasonal = 86,
	ELevelNames__lostsettlementdaily = 87,
	ELevelNames__lostsettlementweekly = 88,
	ELevelNames__lostsettlementseasonal = 89,
	ELevelNames__windsweptpeaks    = 90,
	ELevelNames__galesanctum       = 91,
	ELevelNames__endlessrampart    = 92,
	ELevelNames__windsweptpeaksdaily = 93,
	ELevelNames__windsweptpeaksweekly = 94,
	ELevelNames__windsweptpeaksseasonal = 95,
	ELevelNames__galesanctumdaily  = 96,
	ELevelNames__galesanctumweekly = 97,
	ELevelNames__galesanctumseasonal = 98,
	ELevelNames__endlessrampartdaily = 99,
	ELevelNames__endlessrampartweekly = 100,
	ELevelNames__endlessrampartseasonal = 101,
	ELevelNames__count             = 102,
	ELevelNames__ELevelNames_MAX   = 103
};

struct FEndlessStruggle
{
	int                                                Value;                                                    // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData)
};

enum class EThreatLevel : uint8_t
{
	EThreatLevel__Invalid          = 0,
	EThreatLevel__Threat           = 1,
	EThreatLevel__Threat01         = 2,
	EThreatLevel__Threat02         = 3,
	EThreatLevel__Threat03         = 4,
	EThreatLevel__Threat04         = 5,
	EThreatLevel__Threat05         = 6,
	EThreatLevel__Threat06         = 7,
	EThreatLevel__ENUM_COUNT       = 8,
	EThreatLevel__EThreatLevel_MAX = 9
};

enum class EDLCName : uint8_t
{
	EDLCName__Invalid              = 0,
	EDLCName__TheJungleAwakens     = 1,
	EDLCName__TheCreepingWinter    = 2,
	EDLCName__Mountains            = 3,
	EDLCName__Nether               = 4,
	EDLCName__Oceans               = 5,
	EDLCName__TheEnd               = 6,
	EDLCName__count                = 7,
	EDLCName__EDLCName_MAX         = 8
};

struct FEnchantmentData
{
	unsigned char                                      UnknownData01[0xC];                                       // 0x000A(0x0002) MISSED OFFSET
};

struct FInventoryItemData
{
	unsigned char                                      UnknownData01[0x68];                                       // 0x005C(0x000C)
};

struct FMissionDifficulty
{
	uint8_t                                        	   mission;                                                  // 0x0000(0x0001) (BlueprintVisible, ZeroConstructor, IsPlainOldData)
	uint8_t                                    		   difficulty;                                               // 0x0001(0x0001) (BlueprintVisible, ZeroConstructor, IsPlainOldData)
	uint8_t                                       	   threatLevel;                                              // 0x0002(0x0001) (BlueprintVisible, ZeroConstructor, IsPlainOldData)
	unsigned char                                      UnknownData00[0x1];                                       // 0x0003(0x0001) MISSED OFFSET
	struct FEndlessStruggle                            EndlessStruggle;                                          // 0x0004(0x0004) (BlueprintVisible)
};

struct FMissionState
{
	struct FMissionDifficulty                          MissionDifficulty;                                        // 0x0000(0x0008)
	int                                                Seed;                                                     // 0x0008(0x0004) (BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData)
	unsigned char                                      UnknownData00[0x44];                                      // 0x000C(0x0004) MISSED OFFSET
};


struct FLevelSettings
{
	struct FMissionState                               MissionState;                                             // 0x0000(0x0050) (BlueprintVisible, BlueprintReadOnly)
	class FString                                     unrealMapName;                                            // 0x0050(0x0010) (BlueprintVisible, BlueprintReadOnly, ZeroConstructor)
	class FString                                     levelFilename;                                            // 0x0060(0x0010) (BlueprintVisible, BlueprintReadOnly, ZeroConstructor)
	TArray<class FString>                             progressionKeys;                                          // 0x0070(0x0010) (ZeroConstructor)
};