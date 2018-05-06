#include "src/utils/Arduboy2Ext.h"
#include <ArduboyTones.h>
#include "src/images/Images.h"
#include "src/utils/Enums.h"
#include "src/utils/Utils.h"
#include "src/fonts/Font4x6.h"
#include "src/utils/Data.h"
#include "src/utils/Puzzles.h"
#include "src/utils/EEPROM_Utils.h"
#include "src/sounds/sounds.h"

Arduboy2Ext arduboy;
ArduboyTones sound(arduboy.audio.enabled);
Font4x6 font4x6 = Font4x6(9);

GameState gameState = GameState::IntroInit;
Connector connectors[NUMBER_OF_CONNECTORS];
Connector currentConnector;
Selection selection = {0, 0, NO_GATE_SELECTED, 0};

ItemType oldType;
Level level;
uint8_t levelNumber = 0;
uint8_t counter = 0;
uint8_t resetLevelCounter = 0;
bool showMessageYes = true;



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
  arduboy.audio.begin();
  
  EEPROM_Utils::initEEPROM(false);
  levelNumber = EEPROM_Utils::getLevel();

}


// ----------------------------------------------------------------------------------------------------------------------
//  Main Loop
// ----------------------------------------------------------------------------------------------------------------------

void loop() {

  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();

  switch (gameState) {

    case GameState::IntroInit:
        counter = COUNTER_DELAY;
        gameState = GameState::Intro;
        sound.tones(score);

    case GameState::Intro:
        Intro();
        break;

    case GameState::LevelInit:
    sound.noTone();
        LevelInit(levelNumber);

    case GameState::LevelDisplay:
        LevelDisplay();
        break;

    case GameState::GamePlay:
        GamePlay();
        break;

    case GameState::GameMenu:
        GameMenu();
        break;

  }

  arduboy.display(CLEAR_BUFFER);

}


// --------------------------------------------------------------------------------------
//  Display introduction page and animation ..
//
void Intro() {

  Sprites::drawOverwrite(0, 0, pcb, 0);

  if (arduboy.everyXFrames(2))    { if (counter > 0) counter--; }
  if (counter < 32)               { renderCrossHatch(counter); }
  if (counter == 0)               { Sprites::drawExternalMask(3, 14, logix, logix_mask, 0, 0); }


  // Reset levels?

  if (arduboy.pressed(UP_BUTTON) &&  arduboy.pressed(DOWN_BUTTON)) {

    resetLevelCounter++;

    switch (resetLevelCounter) {

      case 21 ... 60:
        arduboy.setRGBled(128 - (resetLevelCounter * 2), 0, 0);
        break;

      case 61:
        resetLevelCounter = 0;
        arduboy.setRGBled(0, 0, 0);
        EEPROM_Utils::initEEPROM(true);
        levelNumber = 0;
        return;

    }

  }
  else {

    if (resetLevelCounter > 0) {
    
      arduboy.setRGBled(0, 0, 0);
      resetLevelCounter = 0;

    }
    
  }


  // Start game when user presses A button ..

  if (arduboy.justPressed(A_BUTTON)) {gameState = GameState::LevelInit; }

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

  gameState = GameState::LevelDisplay;
  counter = COUNTER_DELAY_SHORT;

}


// --------------------------------------------------------------------------------------
//  Display 'Level 99' banner prior to play ..
//
void LevelDisplay() {

  Sprites::drawOverwrite(0, 0, pcb, 0);
  renderCrossHatch(0);

  arduboy.fillRect(23, 0, 81, 64, BLACK);
  arduboy.fillRect(25, 0, 77, 10, WHITE);

  font4x6.setCursor(28, 1);
  font4x6.setTextColor(BLACK);
  
  font4x6.print("Level ");
  font4x6.print(level.id / 10);
  font4x6.print(level.id % 10);
  font4x6.setTextColor(WHITE);

  arduboy.drawVerticalDottedLine(0, 64, 25, WHITE);
  arduboy.drawVerticalDottedLine(0, 64, 101, WHITE);
  arduboy.drawHorizontalDottedLine(25, 101, 63, WHITE);

  drawChallenges(32, 13);

  if (counter > 0) counter--;

  if (counter == 0) {
  
    font4x6.setCursor(57, 53);
    font4x6.print("Press");
    Sprites::drawSelfMasked(87, 53, aButton, 0);
  
  }

  if (arduboy.justPressed(A_BUTTON)) {gameState = GameState::GamePlay; }

}
