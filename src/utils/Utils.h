#pragma once

#include "Enums.h"
#include "Puzzles.h"


uint8_t column(uint8_t itemId) {
  return itemId / 3;
}

uint8_t row(uint8_t itemId) {
  return itemId % 3;
}

uint8_t indexFromColRow(uint8_t col, uint8_t row) {
  return (col * 3) + row;
}


// --------------------------------------------------------------------------------------
//  Determine the yOffset based on the item's column ..
//
uint8_t yOffset(uint8_t itemId) {

  uint8_t col = column(itemId);

  switch (col) {

    case 0:   return 0;
    case 1:   return 2;
    case 2:   return 0;
    case 3:   return 6;

  }

  return 255;

}


void moveSelectedOtherDown(Selection *selection, Gate gates[]) {

  bool moved = false;

  int8_t rowOther = 0;
  int8_t colOther = 0;
  int8_t colSelected = column(selection->item);

  if (selection->otherItem != NO_GATE_SELECTED) {

    if (gates[selection->otherItem].type != ItemType::NOT && selection->otherConnector == CONNECTOR_INPUT_A && column(selection->otherItem) < 3) { 

      selection->otherConnector = CONNECTOR_INPUT_B;
      moved = true;

    }
    else {

      switch (selection->otherItem) {

        case TERMINAL_PLUS:
          rowOther = -1;
          colOther = column(selection->item) - 1;
          break;

        case 0 ... 11:
          rowOther = row(selection->otherItem);    
          colOther = column(selection->otherItem);
          break;
        
      }

      if (rowOther < 2) {

        for (uint8_t x = rowOther + 1; x < 3; x++) {

          if (gates[indexFromColRow(colOther, x)].type != ItemType::BLANK) {

            selection->otherItem = indexFromColRow(colOther, x);

            if (colSelected < colOther) {
              selection->otherConnector = CONNECTOR_INPUT_A;
            }
            else {
              selection->otherConnector = CONNECTOR_OUTPUT;
            }

            moved = true;
            break;

          }

        }

      }

    }

  }

}


void moveSelectedOtherUp(Selection *selection, Gate gates[]) {

  bool moved = false;
  int8_t rowOther = 0;
  int8_t colOther = 0;
  int8_t colSelected = column(selection->item);

  if (selection->otherItem != TERMINAL_PLUS && selection->otherItem != NO_GATE_SELECTED) {

    if (gates[selection->otherItem].type != ItemType::NOT && selection->otherConnector == CONNECTOR_INPUT_B) { 

      selection->otherConnector = CONNECTOR_INPUT_A;
      moved = true;

    }
    else {

      switch (selection->otherItem) {

        case TERMINAL_MINUS:
          rowOther = 3;
          colOther = column(selection->item) - 1;
          break;

        case 0 ... 11:
          rowOther = row(selection->otherItem);    
          colOther = column(selection->otherItem);
          break;
        
      }

      if (rowOther > 0) {

        for (int8_t x = rowOther - 1; x >= 0; x--) {

          if (gates[indexFromColRow(colOther, x)].type != ItemType::BLANK) {

            selection->otherItem = indexFromColRow(colOther, x);

            if (colSelected < colOther) {
              selection->otherConnector = ( colOther < 3 ? CONNECTOR_INPUT_B : CONNECTOR_INPUT_A);
            }
            else {
              selection->otherConnector = CONNECTOR_OUTPUT;
            }

            moved = true;
            break;

          }

        }

      }

    }

  }

}


void moveSelectedOtherRight(Selection *selection, Gate gates[]) {

  uint8_t currentItem = (selection->otherItem == NO_GATE_SELECTED ? selection->item : selection->otherItem);
  selection->otherConnector = CONNECTOR_INPUT_A;

  switch (currentItem) {

    case NO_GATE_SELECTED:
    case 0 ... 2:
      if      (gates[3].type != ItemType::BLANK) { selection->otherItem = 3; }
      else if (gates[4].type != ItemType::BLANK) { selection->otherItem = 4; }
      else if (gates[5].type != ItemType::BLANK) { selection->otherItem = 5; }

      else if (gates[6].type != ItemType::BLANK) { selection->otherItem = 6; }
      else if (gates[7].type != ItemType::BLANK) { selection->otherItem = 7; }
      else if (gates[8].type != ItemType::BLANK) { selection->otherItem = 8; }

      else if (gates[9].type == ItemType::LED)  { selection->otherItem = 9; }
      else if (gates[10].type == ItemType::LED) { selection->otherItem = 10; }
      else if (gates[11].type == ItemType::LED) { selection->otherItem = 11; }
      break;
    
    case 3 ... 5:
      if      (gates[6].type != ItemType::BLANK) { selection->otherItem = 6; }
      else if (gates[7].type != ItemType::BLANK) { selection->otherItem = 7; }
      else if (gates[8].type != ItemType::BLANK) { selection->otherItem = 8; }
      else if (gates[9].type == ItemType::LED)  { selection->otherItem = 9; }
      else if (gates[10].type == ItemType::LED) { selection->otherItem = 10; }
      else if (gates[11].type == ItemType::LED) { selection->otherItem = 11; }
      break;

    case 6 ... 8:
      if      (gates[9].type == ItemType::LED)  { selection->otherItem = 9; }
      else if (gates[10].type == ItemType::LED) { selection->otherItem = 10; }
      else if (gates[11].type == ItemType::LED) { selection->otherItem = 11; }
      break;

  }

  if (column(selection->item) == column(selection->otherItem)) { selection->otherItem = NO_GATE_SELECTED; }

}


void moveSelectedOtherLeft(Selection *selection, Gate gates[]) {

  uint8_t currentItem = (selection->otherItem == NO_GATE_SELECTED ? selection->item : selection->otherItem);
  selection->otherConnector = CONNECTOR_INPUT_A;

  switch (currentItem) {

    case 3 ... 5:
      if      (gates[0].type != ItemType::BLANK) { selection->otherItem = 0; }
      else if (gates[1].type != ItemType::BLANK) { selection->otherItem = 1; }
      else if (gates[2].type != ItemType::BLANK) { selection->otherItem = 2; }
      break;

    case 6 ... 8:
      if      (gates[3].type != ItemType::BLANK) { selection->otherItem = 3; }
      else if (gates[4].type != ItemType::BLANK) { selection->otherItem = 4; }
      else if (gates[5].type != ItemType::BLANK) { selection->otherItem = 5; }

      else if (gates[0].type != ItemType::BLANK) { selection->otherItem = 0; }
      else if (gates[1].type != ItemType::BLANK) { selection->otherItem = 1; }
      else if (gates[2].type != ItemType::BLANK) { selection->otherItem = 2; }
      break;
    
    case 9 ... 11:
      if      (gates[6].type != ItemType::BLANK) { selection->otherItem = 6; }
      else if (gates[7].type != ItemType::BLANK) { selection->otherItem = 7; }
      else if (gates[8].type != ItemType::BLANK) { selection->otherItem = 8; }

      else if (gates[3].type != ItemType::BLANK) { selection->otherItem = 3; }
      else if (gates[4].type != ItemType::BLANK) { selection->otherItem = 4; }
      else if (gates[5].type != ItemType::BLANK) { selection->otherItem = 5; }

      else if (gates[0].type != ItemType::BLANK) { selection->otherItem = 0; }
      else if (gates[1].type != ItemType::BLANK) { selection->otherItem = 1; }
      else if (gates[2].type != ItemType::BLANK) { selection->otherItem = 2; }

      break;

  }

  if (column(selection->item) == column(selection->otherItem)) { selection->otherItem = NO_GATE_SELECTED; }

}

void createConnection(Selection *selection, Connector *currentConnector, Connector connectors[]) {

  bool added = false;

  for (uint8_t x = 0; x < NUMBER_OF_CONNECTORS; x++) {

    Connector *connector = &connectors[x];

    if (!connector->active) {

      switch (selection->otherItem) {

        case TERMINAL_MINUS:
        case TERMINAL_PLUS:

          connector->fromItem = selection->otherItem;
          connector->toItem = selection->item;
          connector->toInput = selection->connector;
          connector->active = true;
          added = true;
          break;

        default:

          if (selection->item < selection->otherItem) {

            connector->fromItem = selection->item;
            connector->toItem = selection->otherItem;
            connector->toInput = selection->otherConnector;
            connector->active = true;

          }
          else {

            connector->fromItem = selection->otherItem;
            connector->toItem = selection->item;
            connector->toInput = selection->connector;
            connector->active = true;

          }

          added = true;
          break;

      }

      connector->x1 = currentConnector->x1;
      connector->y1 = currentConnector->y1;
      connector->x2 = currentConnector->x2;
      connector->y2 = currentConnector->y2;
      connector->x3 = currentConnector->x3;
      connector->y3 = currentConnector->y3;
      connector->x4 = currentConnector->x4;
      connector->y4 = currentConnector->y4;

    }

    if (added) break;

  }


  currentConnector->reset();

}


void LoadLevel(Level *level, uint8_t levelId) {

  const uint8_t *levelToLoad = puzzles[levelId];
  uint16_t start = 0;

  level->id = (levelId + 1);

  for (uint8_t x = 0; x < NUMBER_OF_GATES; x++) {

    uint8_t item = pgm_read_byte(&levelToLoad[start++]);
    level->items[x].id = x;
    level->items[x].type = static_cast<ItemType>(item);
    level->items[x].editable = (x < 3 || level->items[x].type == ItemType::BLANK);

  }

}


// --------------------------------------------------------------------------------------
//  Clear all connections to the nominated item ..
//
void clearConnectors(Connector connectors[], uint8_t itemId) {

  for (uint8_t x = 0; x < NUMBER_OF_CONNECTORS; x++) {

    Connector *connector = &connectors[x];

    if (connector->fromItem == itemId || connector->toItem) {

      connector->active = false;

    }

  }

}


// --------------------------------------------------------------------------------------
//  Clear the selection status ..
//
void clearStatus(Selection *selection, Connector *currentConnector) {

  selection->reset();
  currentConnector->reset();

}
