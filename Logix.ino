#include "src/utils/Arduboy2Ext.h"
#include "src/images/Images.h"
#include "src/utils/Enums.h"
#include "src/utils/Utils.h"
#include "src/fonts/Font4x6.h"
#include "src/utils/Data.h"
#include "src/utils/Puzzles.h"

Arduboy2Ext arduboy;
Font4x6 font4x6 = Font4x6(9);

GameState gameState = GameState::LevelInit;
Connector connectors[NUMBER_OF_CONNECTORS];
Connector currentConnector;
Selection selection = {0, 0, NO_GATE_SELECTED, 0};

ItemType oldType;
Level level;
uint8_t levelNumber = 0;
uint8_t counter = 0;


// ----------------------------------------------------------------------------------------------------------------------
//  Setup the game.
// ----------------------------------------------------------------------------------------------------------------------

void setup() {

  arduboy.boot();
  arduboy.flashlight();
  arduboy.systemButtons();
  arduboy.setFrameRate(60);
  arduboy.initRandomSeed();
  arduboy.setTextWrap(true);
  arduboy.setTextVertSpacing(9);

}


// ----------------------------------------------------------------------------------------------------------------------
//  Main Loop
// ----------------------------------------------------------------------------------------------------------------------

void loop() {

  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();

  switch (gameState) {

    case GameState::LevelInit:
        LevelInit(levelNumber);

    case GameState::GamePlay:
        GamePlay();
        break;

  }

  arduboy.display(CLEAR_BUFFER);

}




// --------------------------------------------------------------------------------------
//  Initialise level ready for play ..
//
void LevelInit(uint8_t levelNumber) {

  LoadLevel(&level, levelNumber);

  for (uint8_t x = 0; x < NUMBER_OF_CONNECTORS; x++) { 

    Connector *connector = &connectors[x];
    connector->reset();

  }

  gameState = GameState::GamePlay;
  counter = COUNTER_DELAY_SHORT;

}
