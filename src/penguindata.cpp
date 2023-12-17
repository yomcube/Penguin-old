#include "penguin.h"

u32 PenguinData::calculateChecksum(const char* data, u32 num_bytes) {

    u32 crc = 0xFFFFFFFFul;
    const char* ptr = data;

    if (ptr != NULL) {
        for (u32 i = 0; i < num_bytes; i++) {
            crc = (crc >> 8) ^ crc_tab32[(crc ^ (uint32_t)*ptr++) & 0x000000FFul];
        }
    }

    // swap endianness
    crc = ((crc >> 24) & 0xFFul) | ((crc >> 8) & 0xFF00ul) | ((crc << 8) & 0xFF0000ul) | ((crc << 24) & 0xFF000000ul);
    return (crc ^ 0xFFFFFFFFul);
}

u32 PenguinData::swapEndianness32(u32 value) {
    return ((value >> 24) & 0x000000FF) |
           ((value >> 8) & 0x0000FF00) |
           ((value << 8) & 0x00FF0000) |
           ((value << 24) & 0xFF000000);
}

u16 PenguinData::swapEndianness16(u16 value) {
    return ((value >> 8) & 0x00FF) | ((value << 8) & 0xFF00);
}

void PenguinData::setFlag(u32& input, bool status, u8 bit) {
    if (status) { // set bit
        input |= (1 << bit);
    }
    else { // clear bit
        input &= ~(1 << bit);
    }
}

void PenguinData::setFlag(u16& input, bool status, u8 bit) {
    if (status) {
        input |= (1 << bit);
    } else {
        input &= ~(1 << bit);
    }
}

void PenguinData::setFlag(u8& input, bool status, u8 bit) {
    if (status) {
        input |= (1 << bit);
    } else {
        input &= ~(1 << bit);
    }
}
