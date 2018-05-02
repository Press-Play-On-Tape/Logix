#include "src/utils/Arduboy2Ext.h"


// --------------------------------------------------------------------------------------
//  Render the play field ..
//
void render() {


  // Render highlight ..

  const uint8_t highlight_X[] = { 11, 40, 68, 96 };
  const uint8_t highlight_Y[] = { 3, 21, 39, 5, 23, 41, 3, 21, 39, 4, 22, 40 };


  // Render logic gates ..

  Sprites::drawSelfMasked(highlight_X[column(selection.item)], highlight_Y[selection.item], gateHighlight, 0);


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

  if (level.items[9].type == ItemType::LED)  { Sprites::drawSelfMasked(97, 3, LED, 0); }
  if (level.items[10].type == ItemType::LED) { Sprites::drawSelfMasked(97, 21, LED, 0); }
  if (level.items[11].type == ItemType::LED) { Sprites::drawSelfMasked(97, 39, LED, 0); }



  // Display gate selector (up / down) ..

  if (displayGateSelector == 1) {

    if (selection.item <= 8) { Sprites::drawExternalMask( 119, -1, upDown, upDown_Mask, 0, 0); }

  }


  // Display connector selection ..

  if (displayConnectorSelect ) {

    if (selection.item <= 8) { Sprites::drawExternalMask( 119, -1, connectorSelect, connectorSelect_mask, selection.connector + (level.items[selection.item].type == ItemType::NOT ? 3 : 0), 0); }

  }


  arduboy.drawLine(0, 0, 0, 63);
  arduboy.drawLine(4, 0, 4, 63);

}

