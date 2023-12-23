#pragma once
#include "penguin/types.h"
#include "penguin/constants.h"
#include "penguin/savefile.h"
#include "penguin/crc.h"

#pragma pack(push, 1) // no padding pls
const char regionIndicators[6] = {'E', 'P', 'J', 'K', 'C', 'W'}; // North American, European, Japanese, Korean, Chinese, Taiwanese

// made a seperate class for this because it
// would look weird if it was in mainwindow

enum class LogSite : u8 {
    Log_Window,
    Log_StatusBar
};

enum EditorState : u8 {
    State_Normal, // the program will be in this state most of the time
    State_OpeningFile, // used *while* the program is in the process of opening a file
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
    SaveData savedata; // the savedata being worked with
    Settings settings;
    bool fileOpen;
    bool fileSaved;
    EditorState currentState;
    u8 currentSlot; // the save slot currently being edited. 0-2 normal, 3-5 quick
    u8 currentPlayer;
    // this function is a slightly modified version of the crc_32 function from libcrc.
    // https://github.com/lammertb/libcrc/blob/master/src/crc32.c
    static u32 calculateChecksum(const char* data, unsigned int num_bytes);

    static u32 swapEndianness32(u32 value);
    static u16 swapEndianness16(u16 value);
    static void setFlag(u32& input, bool status, u8 bit);
    static void setFlag(u16& input, bool status, u8 bit);
    static void setFlag(u8& inpu, bool status, u8 bit);
};
#pragma pack(pop)
