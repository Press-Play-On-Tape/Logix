#include "src/utils/Arduboy2Ext.h"


// --------------------------------------------------------------------------------------
//  Render the play field ..
//
void render() {


  // Render highlight ..

  const uint8_t highlight_X[] = { 11, 40, 68, 96 };
  const uint8_t highlight_Y[] = { 3, 21, 39, 5, 23, 41, 3, 21, 39, 4, 22, 40 };


  // Render logic gates ..

  switch (selection.item) {

    case 0 ... 11:
      Sprites::drawSelfMasked(highlight_X[column(selection.item)], highlight_Y[selection.item], gateHighlight, 0);
      Sprites::drawOverwrite(123, 44, menu, 0);
      break;

    case SELECTED_MENU:
      if (gameState == GameState::GamePlay) Sprites::drawOverwrite(122, 38, menuSelect, 0);
      break;

    default: break;

  }


  // Render connectors ..

  for (uint8_t x = 0; x < NUMBER_OF_CONNECTORS; x++) {

    Connector *connector = &connectors[x];

    if (connector->active) {

      if (connector->x1 > 0 && connector->x2 > 0)     { arduboy.drawLine(connector->x1, connector->y1, connector->x2, connector->y2); }
      if (connector->x2 > 0 && connector->x3 > 0)     { arduboy.drawLine(connector->x2, connector->y2, connector->x3, connector->y3); }
      if (connector->x3 > 0 && connector->x4 > 0)     { arduboy.drawLine(connector->x3, connector->y3, connector->x4, connector->y4); }

    }

  }


  // Render current connector.  Flash between black and white ..

  uint8_t colour = (arduboy.getFrameCount() % 32 < 16 ? WHITE : BLACK);

  if (currentConnector.x1 > 0 && currentConnector.x2 > 0 && currentConnector.x1 == currentConnector.x2)     { arduboy.drawVerticalDottedLine(currentConnector.y1, currentConnector.y2, currentConnector.x1, colour); }
  if (currentConnector.x1 > 0 && currentConnector.x2 > 0 && currentConnector.y1 == currentConnector.y2)     { arduboy.drawHorizontalDottedLine(currentConnector.x1, currentConnector.x2, currentConnector.y1, colour); }

  if (currentConnector.x2 > 0 && currentConnector.x3 > 0 && currentConnector.x2 == currentConnector.x3)     { arduboy.drawVerticalDottedLine(currentConnector.y2, currentConnector.y3, currentConnector.x2, colour); }
  if (currentConnector.x2 > 0 && currentConnector.x3 > 0 && currentConnector.y2 == currentConnector.y3)     { arduboy.drawHorizontalDottedLine(currentConnector.x2, currentConnector.x3, currentConnector.y2, colour); }

  if (currentConnector.x3 > 0 && currentConnector.x4 > 0 && currentConnector.x3 == currentConnector.x4)     { arduboy.drawVerticalDottedLine(currentConnector.y3, currentConnector.y4, currentConnector.x3, colour); }
  if (currentConnector.x3 > 0 && currentConnector.x4 > 0 && currentConnector.y3 == currentConnector.y4)     { arduboy.drawHorizontalDottedLine(currentConnector.x3, currentConnector.x4, currentConnector.y3, colour); }


  // Render logic gates ..

  if (level.items[0].type >= ItemType::SWITCH_UP && level.items[0].type <= ItemType::SWITCH_DOWN)   Sprites::drawSelfMasked(13, 8, switches, static_cast<uint8_t>(level.items[0].type) - static_cast<uint8_t>(ItemType::SWITCH_UP));
  if (level.items[1].type >= ItemType::SWITCH_UP && level.items[1].type <= ItemType::SWITCH_DOWN)   Sprites::drawSelfMasked(13, 26, switches, static_cast<uint8_t>(level.items[1].type) - static_cast<uint8_t>(ItemType::SWITCH_UP));
  if (level.items[2].type >= ItemType::SWITCH_UP && level.items[2].type <= ItemType::SWITCH_DOWN)   Sprites::drawSelfMasked(13, 44, switches, static_cast<uint8_t>(level.items[2].type) - static_cast<uint8_t>(ItemType::SWITCH_UP));

  Sprites::drawExternalMask(42, 7, logicGates, logicGates_Masks, static_cast<uint8_t>(level.items[3].type), static_cast<uint8_t>(level.items[3].type));
  Sprites::drawExternalMask(42, 25, logicGates, logicGates_Masks, static_cast<uint8_t>(level.items[4].type), static_cast<uint8_t>(level.items[4].type));
  Sprites::drawExternalMask(42, 43, logicGates, logicGates_Masks, static_cast<uint8_t>(level.items[5].type), static_cast<uint8_t>(level.items[5].type));

  Sprites::drawExternalMask(70, 5, logicGates, logicGates_Masks, static_cast<uint8_t>(level.items[6].type), static_cast<uint8_t>(level.items[6].type));
  Sprites::drawExternalMask(70, 23, logicGates, logicGates_Masks, static_cast<uint8_t>(level.items[7].type), static_cast<uint8_t>(level.items[7].type));
  Sprites::drawExternalMask(70, 41, logicGates, logicGates_Masks, static_cast<uint8_t>(level.items[8].type), static_cast<uint8_t>(level.items[8].type));

  if (level.items[9].type == ItemType::LED)  { 
   
    Sprites::drawSelfMasked(97, 3, LED, (level.items[9].output() ? (arduboy.getFrameCountAlternate(FLASHING_LED_DELAY) ? 1 : 2) : 0));
    arduboy.drawLine(113, 14, 118, 14);
    arduboy.drawLine(118, 14, 118, 63);
    
  }

  if (level.items[10].type == ItemType::LED) {
    
    Sprites::drawSelfMasked(97, 21, LED, (level.items[10].output() ? (arduboy.getFrameCountAlternate(FLASHING_LED_DELAY) ? 1 : 2) : 0));
    arduboy.drawLine(113, 32, 118, 32);
    arduboy.drawLine(118, 32, 118, 63);

  }

  if (level.items[11].type == ItemType::LED) {
    
    Sprites::drawSelfMasked(97, 39, LED, (level.items[11].output() ? (arduboy.getFrameCountAlternate(FLASHING_LED_DELAY) ? 1 : 2) : 0));
    arduboy.drawLine(113, 50, 118, 50);
    arduboy.drawLine(118, 50, 118, 63);

  }


  // Draw remainder of layout ..

  arduboy.drawLine(0, 0, 118, 0);
  arduboy.drawLine(0, 5, 0, 55);
  arduboy.drawLine(4, 7, 4, 58);
  arduboy.drawLine(0, 63, 118, 63);
  arduboy.drawVerticalDottedLine(0, 63, 120, WHITE);
  Sprites::drawSelfMasked(0, 0, plus, 0);
  Sprites::drawSelfMasked(0, 58, minus, 0);


  // If the gate selector and the connetor selector are not being shown, show the level ..

  if (displayGateSelector == 0 && !displayConnectorSelect) {

    Sprites::drawOverwrite(123, 0, level_sideways, 0);
    Sprites::drawOverwrite(123, 24, numbers_sideways, level.id / 10);
    Sprites::drawOverwrite(123, 29, numbers_sideways, level.id % 10);
    
  }


  // Display gate selector (up / down) ..

  if (displayGateSelector == 1) {

    if (selection.item <= 8) { Sprites::drawExternalMask( 119, -1, upDown, upDown_Mask, 0, 0); }

  }

  if (displayGateSelector == 2) {

    if (selection.item <= 8) { Sprites::drawExternalMask( 119, -1, upDown_Locked, upDown_Mask, 0, 0); }

  }


  // Display connector selection ..

  if (displayConnectorSelect ) {

    if (selection.item <= 8) { Sprites::drawExternalMask( 119, -1, connectorSelect, connectorSelect_mask, selection.connector + (level.items[selection.item].type == ItemType::NOT ? 3 : 0), 0); }

  }


  // Show next level message?

  if (level.showMessage && gameState == GameState::GamePlay) {

    if (levelNumber == NUMBER_OF_PUZZLES - 1) {

      arduboy.fillRect(15, 20, 86, 24, BLACK);
      arduboy.drawHorizontalDottedLine(18, 100, 23, WHITE);
      arduboy.drawHorizontalDottedLine(18, 100, 38, WHITE);

      font4x6.setCursor(21, 27);
      font4x6.setTextColor(WHITE);
      font4x6.print("Congratulations!");

    }
    else {

      arduboy.fillRect(17, 20, 82, 24, BLACK);
      arduboy.drawHorizontalDottedLine(20, 96, 23, WHITE);
      arduboy.drawHorizontalDottedLine(20, 96, 39, WHITE);

      font4x6.setCursor(22, 27);
      font4x6.setTextColor(WHITE);
      font4x6.print("Next level? ");

      if (showMessageYes) {
          arduboy.fillRect(81, 27, 6, 8, WHITE);
          font4x6.setTextColor(BLACK);
      }
      else {
          font4x6.setTextColor(WHITE);
      }

      font4x6.print("Y");
      font4x6.setCursor(90, 27);

      if (!showMessageYes) {
          arduboy.fillRect(89, 27, 6, 8, WHITE);
          font4x6.setTextColor(BLACK);
      }
      else {
          font4x6.setTextColor(WHITE);
      }

      font4x6.print("N");

    }

  }

}


// --------------------------------------------------------------------------------------
//  Render current puzzle challenge ..
//
void drawChallenges(uint8_t xOffset, uint8_t yOffset) {

  for (uint8_t x = 0; x < level.numberOfOutcomes; x++) {

    font4x6.setCursor(xOffset + (x * 25) + (level.numberOfOutcomes == 2 ? 12 : 0), yOffset);
    font4x6.print("S L");
    font4x6.setCursor(xOffset + (x * 25) + (level.numberOfOutcomes == 2 ? 12 : 0), yOffset + 11);


    // Row 1 ---------------------------------------------------------

    if (level.outcome[x].switchState0 <= 1) {

      font4x6.print(level.outcome[x].switchState0);

    }
    else {

      font4x6.print(" ");

    }

    font4x6.print(" ");


    if (level.outcome[x].ledState0 <= 1) {
        font4x6.print(level.outcome[x].ledState0);
    }

    font4x6.print("\n");


    // Row 2 ---------------------------------------------------------

    if (level.outcome[x].switchState1 <= 1) {

      font4x6.print(level.outcome[x].switchState1);

    }
    else {

      font4x6.print(" ");

    }

    font4x6.print(" ");


    if (level.outcome[x].ledState1 <= 1) {
        font4x6.print(level.outcome[x].ledState1);
    }

    font4x6.print("\n");

    
    // Row 3 ---------------------------------------------------------

    if (level.outcome[x].switchState2 <= 1) {

      font4x6.print(level.outcome[x].switchState2);

    }
    else {

      font4x6.print(" ");

    }

    font4x6.print(" ");


    if (level.outcome[x].ledState2 <= 1) {
        font4x6.print(level.outcome[x].ledState2);
    }

    font4x6.print("\n");


    // Divider -------------------------------------------------------

    if (x < level.numberOfOutcomes - 1) {

      arduboy.drawVerticalDottedLine(yOffset + 12, yOffset + 36, xOffset + 19 + (level.numberOfOutcomes == 2 ? 12 : 0) + (x * 25), WHITE);

    }

  }

}


// --------------------------------------------------------------------------------------
//  Render cross hatch effect.  Counter range 0 - 32 ..
//
void renderCrossHatch(uint8_t counter) {

  for (uint8_t x = counter; x < 32; x++) {

    arduboy.drawHorizontalDottedLine((x%2), 128, x, BLACK);
    arduboy.drawHorizontalDottedLine((x%2), 128, 63 - x, BLACK);
    
  }

}