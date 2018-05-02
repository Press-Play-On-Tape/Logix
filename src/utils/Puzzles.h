#pragma once

#include "Enums.h"


const uint8_t PROGMEM puzzle_00[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), 
    static_cast<uint8_t>(ItemType::AND), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::NOT), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::NAND), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), 

};

#define NUMBER_OF_PUZZLES 1

const uint8_t *puzzles[] =    { puzzle_00 };