#pragma once
#include "penguin/types.h"
#include "penguin/constants.h"
#include "penguin/savefile.h"
#include "penguin/crc.h"
#include <vector>

#pragma pack(push, 1) // no padding pls
const char regionIndicators[6] = {'E', 'P', 'J', 'K', 'C', 'W'}; // North American, European, Japanese, Korean, Chinese, Taiwanese

static std::vector<std::string> w1Levels = {
    "1-1",
    "1-2",
    "1-3",
    "1-4",
    "1-5",
    "1-6",
    "1-Tower",
    "1-Castle",
    "1-Cannon",
};

static std::vector<std::string> w2Levels = {
    "2-1",
    "2-2",
    "2-3",
    "2-4",
    "2-5",
    "2-6",
    "2-Tower",
    "2-Castle",
    "2-Cannon",
};

static std::vector<std::string> w3Levels = {
    "3-1",
    "3-2",
    "3-3",
    "3-4",
    "3-5",
    "3-Ghost House",
    "3-Tower",
    "3-Castle",
    "3-Cannon",
};

static std::vector<std::string> w4Levels = {
    "4-1",
    "4-2",
    "4-3",
    "4-4",
    "4-5",
    "4-Ghost House",
    "4-Tower",
    "4-Castle",
    "4-Airship",
    "4-Cannon",
};

static std::vector<std::string> w5Levels = {
    "5-1",
    "5-2",
    "5-3",
    "5-4",
    "5-5",
    "5-Ghost House",
    "5-Tower",
    "5-Castle",
    "5-Cannon",
};

static std::vector<std::string> w6Levels = {
    "6-1",
    "6-2",
    "6-3",
    "6-4",
    "6-5",
    "6-6",
    "6-Tower",
    "6-Castle",
    "6-Airship",
    "6-Cannon",
};

static std::vector<std::string> w7Levels = {
    "7-1",
    "7-2",
    "7-3",
    "7-4",
    "7-5",
    "7-6",
    "7-Ghost House",
    "7-Tower",
    "7-Castle",
};

static std::vector<std::string> w8Levels = {
    "8-1",
    "8-2",
    "8-3",
    "8-4",
    "8-5",
    "8-6",
    "8-7",
    "8-Tower",
    "8-Castle",
    "8-Airship",
};

static std::vector<std::string> w9Levels = {
    "9-1",
    "9-2",
    "9-3",
    "9-4",
    "9-5",
    "9-6",
    "9-7",
    "9-8",
};
// made a seperate class for this because it
// would look weird if it was in mainwindow

enum class LogSite : u8 {
    Log_Window,
    Log_StatusBar
};

enum EditorState : u8 {
    State_Normal,           // the program will be in this state most of the time
    State_OpeningFile,      // used *while* the program is in the process of opening a file
    State_SwitchingWorlds,  // used *while* the program is switching worlds
};

struct Vec2i {
    int x;
    int y;
};

struct Settings {
    bool askToClose; // ask if you want to close the program first
    LogSite logSite; // where information regarding the program is logged
    bool maintainPosition; // keep the last position of the editor, or reset to default
    Vec2i pos;
};


class PenguinData {
public:
    SaveData savedata;      // the savedata being worked with
    SaveData savedSavedata; // the savedata stored (but not saved to file) when the working savedata is saved or opened.
    Settings settings;
    bool fileOpen;
    bool fileSaved;
    EditorState currentState;
    u8 currentSlot; // the save slot currently being edited. 0-2 normal, 3-5 quick
    u8 currentPlayer;
    u8 currentWorld;
    std::vector<std::vector<std::string>> levelLabelArrays = {w1Levels, w2Levels, w3Levels, w4Levels, w5Levels, w6Levels, w7Levels, w8Levels, w9Levels};
    u8 worldLevelIndices[9]     = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // used to store which level is selected for each world
    u8 worldRescueIndices[9]    = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    // this function is a slightly modified version of the crc_32 function from libcrc.
    // https://github.com/lammertb/libcrc/blob/master/src/crc32.c
    static u32 calculateChecksum(const char* data, unsigned int num_bytes);

    static u32 swapEndianness32(u32 value);
    static u16 swapEndianness16(u16 value);
    static void setFlag(u32& input, bool status, u8 bit);
    static void setFlag(u16& input, bool status, u8 bit);
    static void setFlag(u8& input, bool status, u8 bit);
};
#pragma pack(pop)
