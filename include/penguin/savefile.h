#pragma once
#include "penguin/types.h"
#include "penguin/constants.h"
// documentation n such goes here
// big endian
#pragma pack(push, 1)
struct SaveData {
    struct Header {
        char magic[4]; // 'SMN' + region identifier.
        /*
            E - North American
            P - European, Australian
            J - Japanese
            K - Korean
            C - Chinese
            W - Taiwanese
        */

        u8 version[2];						// version and subversion; always 0xE and 0x0.
        u8 lastSelectedFile;				// 0 to 2.
        u8 _7;								// unused. speculated to be padding.
        u16 freeModePlayCount[WORLD_COUNT][STAGE_COUNT];
        u16 coinBattlePlayCount[WORLD_COUNT][STAGE_COUNT];
        u16 extraModesUnlockedWorlds;		// worlds unlocked in extra modes.
        u16 _69A;							// unused. speculated to be padding.
        u32 crc32;							// calculated checksum (excluding the magic)
    };

    static_assert(sizeof(SaveData::Header) == 0x6A0, "Header size mismatch!"); // wrong size

    struct SaveSlot {
        u8 version[2];									// version and subversion; always 0xE and 0x0.
        u8 gameCompletion;
        u8 currentWorld;
        u8 currentSubworld;
        u8 currentPathNode;
        u8 w5VineReshuffleCounter;						/* used in World 5. this value decreases when a
                                                        level is played. upon reaching zero, the vines will reshuffle. */
        bool w3SwitchOn;								// used in World 3. indicates the red switch's status.
        u8 _8;											// unused.
        u8 stockItemCount[POWERUP_COUNT];
        u8 startingMushroomHouseType[WORLD_COUNT];
        u8 playerContinues[PLAYER_COUNT];
        u8 playerCoins[PLAYER_COUNT];
        u8 playerLives[PLAYER_COUNT];
        u8 playerSpawnFlag[PLAYER_COUNT];
        u8 playerCharacter[PLAYER_COUNT];
        u8 playerPowerup[PLAYER_COUNT];
        u8 worldCompletion[WORLD_COUNT];
        u8 enemyRevivalCount[WORLD_COUNT][AMBUSH_ENEMY_COUNT];
        u16 _64;										// unused.
        u16	staffCreditsHighScore;
        u32 score;										// regular score. yknow, the one you see in levels.
        u32 stageCompletion[WORLD_COUNT][STAGE_COUNT];	// level completion flags.
        bool hintMovieBought[HINT_MOVIE_COUNT];			// indicates whether or not a Hint Movie has been bought.
        u8 toadRescueLevel[WORLD_COUNT];				// Toad rescue level for each world.
        u8 enemySubworldNumber[WORLD_COUNT][AMBUSH_ENEMY_COUNT];	// subworld number for each map enemy.
        u8 enemyPosIndex[WORLD_COUNT][AMBUSH_ENEMY_COUNT]; // path node for each map enemy.
        u8 enemyWalkDirection[WORLD_COUNT][AMBUSH_ENEMY_COUNT];
        u8 deathCount[WORLD_COUNT][STAGE_COUNT];		// death count for each level.
        u8 deathCountW3Level4Switch;					// death count for the "red switch active" variant of World 3-4.
        u8 padding[0x13];
        u32 crc32;										// checksum.
    };

    static_assert(sizeof(SaveData::SaveSlot) == 0x980, "Save Slot size mismatch!");


    Header header;
    SaveSlot saveSlots[3];
    SaveSlot quickSaves[3];
};

static_assert(sizeof(SaveData) == 0x3FA0, "Save Data size mismatch!");

static std::string hintMovieTitles[HINT_MOVIE_COUNT - 6] = {
    "1-1: Super Skills",
    "1-1: Infinite 1-Ups",
    "1-2: Infinite 1-Ups",
    "1-3: Secret Goal",
    "1-3: Super Skills",
    "1-T: Super Skills",
    "2-1: Star Coin",
    "2-1: Super Skills",
    "2-2: Star Coin",
    "2-2: Super Skills",
    "2-3: Infinite 1-Ups",
    "2-4: Secret Goal",
    "2-5: Infinite 1-Ups",
    "2-5: Super Skills",
    "2-6: Secret Goal",
    "3-1: Star Coin",
    "3-1: Super Skills",
    "3-2: Super Skills",
    "3-2: Infinite 1-Ups",
    "3-3: Star Coin",
    "3-3: Infinite 1-Ups",
    "3-3: Super Skills",
    "3-G: Secret Goal",
    "3-5: Super Skills",
    "4-1: Infinite 1-Ups",
    "4-2: Infinite 1-Ups",
    "4-2: Super Skills",
    "4-3: Star Coin",
    "4-3: Super Skills",
    "4-T: Secret Goal",
    "4-4: Star Coin",
    "4-G: Secret Goal",
    "4-C: Infinite 1-Ups",
    "5-1: Infinite 1-Ups",
    "5-3: Infinite 1-Ups",
    "5-G: Star Coin",
    "5-G: Secret Goal",
    "5-C: Super Skills",
    "6-1: Infinite 1-Ups",
    "6-2: Star Coin",
    "6-3: Star Coin",
    "6-3: Super Skills",
    "6-5: Secret Goal",
    "6-6: Secret Goal",
    "6-C: Super Skills",
    "7-1: Super Skills",
    "7-3: Star Coin",
    "7-T: Secret Goal",
    "7-G: Secret Goal",
    "7-4: Infinite 1-Ups",
    "7-C: Star Coin",
    "8-2: Secret Goal",
    "8-3: Super Skills",
    "8-T: Star Coin",
    "8-A: Infinite 1-Ups",
    "8-C: Super Skills",
    "9-1: Super Skills",
    "9-2: Super Skills",
    "9-3: Infinite 1-Ups",
    "9-3: Super Skills",
    "9-4: Star Coin",
    "9-5: Super Skills",
    "9-6: Star Coin",
    "9-7: Super Skills",
};
#pragma pack(pop)
