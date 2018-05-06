#include "src/utils/Arduboy2Ext.h"

uint8_t menuSelection0 = 0;
uint8_t menuSelection1 = 0;
ItemType gateIndex = ItemType::AND;
uint8_t successCount = 0;

const uint8_t y[] = { 4, 13, 22, 31, 43 };


// ----------------------------------------------------------------------------------------------------------------------
//  Play the Game!
// ----------------------------------------------------------------------------------------------------------------------

void GameMenu() {

  bool islevelWiredCompletely = islevelFullyWired(&level, connectors);

  if (!islevelWiredCompletely && menuSelection0 == 1) menuSelection0 = 0;


  render();
  
  arduboy.fillRect(0, 0, 40, 55, BLACK);
  arduboy.drawRect(1, 1, 38, 53, WHITE);
  arduboy.drawHorizontalDottedLine( 5, 36, 41, WHITE);

  font4x6.setTextColor(WHITE);
  font4x6.setCursor(8, 4);
  font4x6.print(F("Back\nTest\nInfo\nHelp"));
  font4x6.setCursor(8, 43);
  font4x6.print(F("Quit"));
  font4x6.setTextColor(BLACK);
  font4x6.setCursor(8, y[menuSelection0]);
  arduboy.fillRect(4, y[menuSelection0], 32, 8, WHITE);

  if (!islevelWiredCompletely) {

    Sprites::drawOverwrite(30, 13, padlock, 0);

  }


  // Top level seelection ..

  switch (menuSelection0) {

    case 0:
      font4x6.print(F("Back"));
      break;

    case 1:
      font4x6.print(F("Test"));
      break;

    case 2:
      font4x6.print(F("Info"));
      Sprites::drawErase(31, 24, arrowRight, 0);
      break;

    case 3:
      font4x6.print(F("Help"));
      Sprites::drawErase(31, 33, arrowRight, 0);
      break;

    case 4:
      font4x6.setCursor(8, 43);
      font4x6.print(F("Quit"));
      break;

  }


  // Second level menu items ..

  font4x6.setTextColor(WHITE);

  switch (menuSelection1) {

    case 1:
      {

        if (successCount < 100) { successCount++; }

        arduboy.setRGBled(0, 0, 0);
        if (successCount > 0 && successCount < 16 && level.numberOfOutcomes >= 1)   { arduboy.setRGBled((!level.outcome[0].successful ? 32 : 0), (level.outcome[0].successful ? 32 : 0), 0); }
        if (successCount > 32 && successCount < 48 && level.numberOfOutcomes >= 2)  { arduboy.setRGBled((!level.outcome[1].successful ? 32 : 0), (level.outcome[1].successful ? 32 : 0), 0); }
        if (successCount > 64 && successCount < 80 && level.numberOfOutcomes >= 3)  { arduboy.setRGBled((!level.outcome[2].successful ? 32 : 0), (level.outcome[2].successful ? 32 : 0), 0); }

        arduboy.fillRect(36, 8, 78, 57, BLACK); 
        arduboy.drawRect(37, 9, 76, 55, WHITE); 
        arduboy.drawHorizontalDottedLine(40, 110, 19, WHITE);
        font4x6.setCursor(42, 10);
        font4x6.print("Test Results ");

        for (uint8_t x = 0; x < level.numberOfOutcomes; x++) {

          font4x6.setCursor(43, 24 + (x * 13));
          font4x6.print("Test ");
          font4x6.print(x + 1);
          font4x6.print(" ... ");

          if (level.outcome[x].successful) {

            Sprites::drawSelfMasked(96, 23 + (x * 13), smile, 0);

          }
          else {

            Sprites::drawSelfMasked(96, 23 + (x * 13), frown, 0);

          }

        }

        if (level.levelComplete()) {
          
          level.showMessage = true;    
          selection.item = 0;

        }

      }

      break;

    case 2:
      {
        arduboy.fillRect(36, 8, 78, 57, BLACK); 
        arduboy.drawRect(37, 9, 76, 55, WHITE); 
        arduboy.drawHorizontalDottedLine(40, 110, 19, WHITE);
        font4x6.setCursor(42, 10);
        font4x6.print("Level ");
        font4x6.print(level.id / 10);
        font4x6.print(level.id % 10);

        drawChallenges(43, 23);

      }

      break;

    case 3:
      {
        arduboy.fillRect(36, 8, 81, 57, BLACK); 
        arduboy.drawRect(37, 9, 79, 55, WHITE); 
        arduboy.fillRect(113, 9, 7, 55, WHITE); 
        Sprites::drawOverwrite(114, 10, arrowUp, 0);
        Sprites::drawOverwrite(114, 54, arrowDown, 0);
        arduboy.drawHorizontalDottedLine(40, 110, 19, WHITE);

        Sprites::drawOverwrite(51, 33, logicGates, static_cast<uint8_t>(gateIndex));

        font4x6.setCursor(40, 10);
        font4x6.print(reinterpret_cast<const __FlashStringHelper *>(pgm_read_word(&itemTypes[static_cast<uint8_t>(gateIndex)])));

        if (gateIndex != ItemType::NOT) {

          font4x6.setCursor(85, 10);
          font4x6.print(F("A B O"));
          font4x6.setCursor(42, 32);
          font4x6.print(F("A\nB"));
        }
        else {
          font4x6.setCursor(85, 10);
          font4x6.print(F("A   O"));
          font4x6.setCursor(42, 36);
          font4x6.print(F("A"));
        }
        font4x6.setCursor(71, 37);
        font4x6.print(F("O"));

        uint8_t start = (static_cast<uint8_t>(gateIndex) * 12);

        font4x6.setCursor(85, 21);
        renderDataLine(start);
        arduboy.drawHorizontalDottedLine(82, 110, 30, WHITE);
        font4x6.setCursor(85, 32);
        renderDataLine(start);
        arduboy.drawHorizontalDottedLine(82, 110, 41, WHITE);

        if (gateIndex != ItemType::NOT) {

          font4x6.setCursor(85, 43);
          renderDataLine(start);
          arduboy.drawHorizontalDottedLine(82, 110, 52, WHITE);
          font4x6.setCursor(85, 54);
          renderDataLine(start);
          arduboy.drawHorizontalDottedLine(82, 110, 63, WHITE);

        }

      }
      break;

  }


  // Handle keypresses ..

  if (arduboy.justPressed(UP_BUTTON) && menuSelection0 > 0 && menuSelection1 == 0)      { 
    if (menuSelection0 == 2 && !islevelWiredCompletely) {
      menuSelection0 = 0; 
    }
    else {
      menuSelection0--; 
    }
  }

  if (arduboy.justPressed(UP_BUTTON) && menuSelection1 == 3)                            { gateIndex++; if (gateIndex == ItemType::BLANK) gateIndex = ItemType::AND; }

  if (arduboy.justPressed(DOWN_BUTTON) && menuSelection0 < 4 && menuSelection1 == 0)    { 
    if (menuSelection0 == 0 && !islevelWiredCompletely) {
      menuSelection0 = 2; 
    }
    else {
      menuSelection0++; 
    }
  }

  if (arduboy.justPressed(DOWN_BUTTON) && menuSelection1 == 3)                          { gateIndex--; if (gateIndex == ItemType::BLANK) gateIndex = ItemType::NOT; }

  if (arduboy.justPressed(RIGHT_BUTTON) && menuSelection0 == 2 && menuSelection1 == 0)  { menuSelection1 = 2; }
  if (arduboy.justPressed(RIGHT_BUTTON) && menuSelection0 == 3 && menuSelection1 == 0)  { menuSelection1 = 3; }
  if (arduboy.justPressed(LEFT_BUTTON) && menuSelection1 > 0)                           { menuSelection1 = 0; arduboy.setRGBled(0, 0, 0); }
  if (arduboy.justPressed(B_BUTTON) && menuSelection1 == 0)                             { selection.reset(); gameState = GameState::GamePlay; }
  if (arduboy.justPressed(B_BUTTON) && menuSelection1 > 0)                              { menuSelection1 = 0; arduboy.setRGBled(0, 0, 0); }

  if (arduboy.justPressed(A_BUTTON)) {

    switch (menuSelection0) {

      case 0:
        selection.reset();
        gameState = GameState::GamePlay;
        break;

      case 1:
        successCount = 0;
        test(&level, connectors);
        sound.tones(level.levelComplete() ? success : fail);
        menuSelection1 = 1;
        break;

      case 2:
        menuSelection1 = 2;
        break;

      case 3:
        menuSelection1 = 3;
        break;

      case 4:
        menuSelection0 = 0;
        menuSelection1 = 0;
        gameState = GameState::IntroInit;
        break;
    
    }

  }

}


// --------------------------------------------------------------------------------------
//  Render a single puzzle outcome line ..
//
void renderDataLine(uint8_t &start) {

  font4x6.print(pgm_read_byte(&data[start++]));
  font4x6.print(" ");

  uint8_t x = pgm_read_byte(&data[start++]);

  if (x < 2) {
    font4x6.print(x);
  }
  else {
    font4x6.print(" ");
  }

  font4x6.print(" ");
  font4x6.print(pgm_read_byte(&data[start++]));

}