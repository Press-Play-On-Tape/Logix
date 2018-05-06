#include "src/utils/Arduboy2Ext.h"


uint8_t displayGateSelector = 0;
bool displayConnectorSelect = false;



//----------------------------------------------------------------------------------------------------------------------
// Play the Game!
//----------------------------------------------------------------------------------------------------------------------

void GamePlay() {

  if (level.showMessage) {

    selection.reset();
    GamePlay_ShowMessage();

  }
  else {

    GamePlay_Normal();

  }

}


// --------------------------------------------------------------------------------------
//  If the user has just completed the previous puzzle, prompt for the next level ..
//
void GamePlay_ShowMessage() {

  if (arduboy.justPressed(LEFT_BUTTON) && !showMessageYes) showMessageYes = !showMessageYes;
  if (arduboy.justPressed(RIGHT_BUTTON) && showMessageYes) showMessageYes = !showMessageYes;

  if (levelNumber != NUMBER_OF_PUZZLES - 1) {
    
    if (arduboy.justPressed(A_BUTTON) && showMessageYes)  {

      levelNumber++;
      EEPROM_Utils::setLevel(levelNumber);
      level.showMessage = false;
      gameState = GameState::LevelInit;

    }

    if (arduboy.justPressed(A_BUTTON) && !showMessageYes)  {

      level.showMessage = false;

    }

  }

  render();

}


// --------------------------------------------------------------------------------------
//  Normal play ..
//
void GamePlay_Normal() {

  displayGateSelector = 0;
  displayConnectorSelect = false;


  //----------------------------------------------------------------------------------------------------------------------
  // Has the user selected the menu?

  if (selection.item == SELECTED_MENU && arduboy.justPressed(A_BUTTON)) { 
  
    gameState = GameState::GameMenu; 
      
  }


  //----------------------------------------------------------------------------------------------------------------------
  // Handle simple navigation around the screen ..
  
  else if (!arduboy.pressed(B_BUTTON) && !arduboy.justReleased(B_BUTTON) && !arduboy.pressed(A_BUTTON) && !arduboy.justReleased(A_BUTTON)) {

    if (column(selection.item) <= 3) {

      if (arduboy.justPressed(UP_BUTTON) && selection.item % 3 > 0)            { selection.item = selection.item - 1; clearStatus(&selection, &currentConnector); }
      if (arduboy.justPressed(DOWN_BUTTON) && selection.item % 3 < 2)          { selection.item = selection.item + 1; clearStatus(&selection, &currentConnector); }

    }

    if (arduboy.justPressed(LEFT_BUTTON) && selection.item > 2 && selection.item != SELECTED_MENU)   { selection.item = selection.item - 3; clearStatus(&selection, &currentConnector); }
    else if (arduboy.justPressed(LEFT_BUTTON) && selection.item == SELECTED_MENU)  { selection.item = 9; clearStatus(&selection, &currentConnector); }

    if (arduboy.justPressed(RIGHT_BUTTON) && selection.item < 9)               { selection.item = selection.item + 3; clearStatus(&selection, &currentConnector); }
    else if (arduboy.justPressed(RIGHT_BUTTON) && selection.item >= 9)         { selection.item = SELECTED_MENU; clearStatus(&selection, &currentConnector); }

  }


  //----------------------------------------------------------------------------------------------------------------------
  // Handle the new connection ..

  else if (arduboy.pressed(A_BUTTON)) {

    if (level.items[selection.item].type != ItemType::BLANK) {

      if (arduboy.justPressed(A_BUTTON)) { 
        
        selection.connector = (selection.connector + 1) % 3; 

        if (level.items[selection.item].type == ItemType::NOT && selection.connector == 1) selection.connector = 2;

        selection.otherItem = NO_GATE_SELECTED; 
        currentConnector.reset();

      }

      displayConnectorSelect = true;


      switch (selection.item) {

        case 0 ... 2: // switches

          switch (selection.connector) {

            case CONNECTOR_INPUT_A ... CONNECTOR_INPUT_B:
              {
                if (arduboy.justPressed(LEFT_BUTTON)) { 
                  
                  switch (selection.otherItem) {

                    case NO_GATE_SELECTED: 
                      selection.otherItem = TERMINAL_MINUS;
                      break;

                    case TERMINAL_MINUS:
                      selection.otherItem = TERMINAL_PLUS;
                      break;
                    
                  }

                }

                if (arduboy.justPressed(RIGHT_BUTTON)) { 
                  
                  switch (selection.otherItem) {

                    case TERMINAL_PLUS: 
                      selection.otherItem = TERMINAL_MINUS;
                      break;

                    case TERMINAL_MINUS:
                      selection.otherItem = NO_GATE_SELECTED;
                      break;
                    
                  }

                }
                
                currentConnector.reset();

                if (selection.otherItem == TERMINAL_PLUS) {

                  currentConnector.x1 = 12;
                  currentConnector.y1 = 9 + (selection.item * 18) + (selection.connector == 0 ? 0 : 6); 
                  currentConnector.x2 = 1;
                  currentConnector.y2 = currentConnector.y1;

                }

                if (selection.otherItem == TERMINAL_MINUS) {

                  currentConnector.x1 = 12;
                  currentConnector.y1 = 9 + (selection.item * 18) + (selection.connector == 0 ? 0 : 6); 
                  currentConnector.x2 = 4;
                  currentConnector.y2 = currentConnector.y1;

                }

              }
              break;

            case CONNECTOR_OUTPUT:
              {
                if (arduboy.justPressed(UP_BUTTON))     { moveSelectedOtherUp(&selection, level.items, MOVEMENT_NO_RESTRICTIONS); }
                if (arduboy.justPressed(DOWN_BUTTON))   { moveSelectedOtherDown(&selection, level.items, MOVEMENT_NO_RESTRICTIONS); }
                if (arduboy.justPressed(RIGHT_BUTTON))  { moveSelectedOtherRight(&selection, level.items, MOVEMENT_PREVENT_LEFT | MOVEMENT_ALLOW_RIGHT); }
                if (arduboy.justPressed(LEFT_BUTTON))   { moveSelectedOtherLeft(&selection, level.items, MOVEMENT_PREVENT_LEFT | MOVEMENT_ALLOW_RIGHT); }

                if (selection.otherItem != NO_GATE_SELECTED) {

                  int8_t colDiff = column(selection.otherItem) - column(selection.item);


                  // Draw connector ..

                  currentConnector.x1 = 30; 
                  currentConnector.y1 = 12 + yOffset(selection.item) + (selection.item * 18); 

                  currentConnector.x2 = currentConnector.x1 + ((colDiff - 1) * 28) + 2 + row(selection.item) * 2; 
                  currentConnector.y2 = currentConnector.y1;
                  currentConnector.x3 = currentConnector.x2;
                  currentConnector.y3 = 8 + yOffset(selection.otherItem) + (row(selection.otherItem) * 18) + (level.items[selection.otherItem].type == ItemType::NOT ? 4 : 0) + (selection.otherConnector == CONNECTOR_INPUT_A ? 0 : 8);
                  currentConnector.x4 = currentConnector.x1 + 11 + ((colDiff - 1) * 28);
                  currentConnector.y4 = currentConnector.y3;

                }
                else {

                  currentConnector.reset();

                }
                
              }
              break;

          }
          break;

        case 3 ... 8: // gates

          switch (selection.connector) {

            case CONNECTOR_INPUT_A ... CONNECTOR_INPUT_B:
              {
                if (arduboy.justPressed(UP_BUTTON))     { moveSelectedOtherUp(&selection, level.items, MOVEMENT_ALLOW_PLUS); }
                if (arduboy.justPressed(DOWN_BUTTON))   { moveSelectedOtherDown(&selection, level.items, MOVEMENT_ALLOW_MINUS); }
                if (arduboy.justPressed(RIGHT_BUTTON))  { moveSelectedOtherRight(&selection, level.items, MOVEMENT_ALLOW_LEFT | MOVEMENT_PREVENT_RIGHT); }
                if (arduboy.justPressed(LEFT_BUTTON))   { moveSelectedOtherLeft(&selection, level.items, MOVEMENT_ALLOW_LEFT | MOVEMENT_PREVENT_RIGHT); }

                currentConnector.reset();
                currentConnector.x1 = 13 + (column(selection.item) * 28);
                currentConnector.y1 = 8 + (level.items[selection.item].type == ItemType::NOT ? 4 : 0) + yOffset(selection.item) + (row(selection.item) * 18) + (selection.connector == 0 ? 0 : 8); 

                switch (selection.otherItem) {

                  case TERMINAL_MINUS:
                    currentConnector.x2 = currentConnector.x1 - 2 - (row(selection.item) * 2);
                    currentConnector.y2 = currentConnector.y1; 
                    currentConnector.x3 = currentConnector.x2;
                    currentConnector.y3 = 62; 
                    break;

                  case TERMINAL_PLUS:
                    currentConnector.x2 = currentConnector.x1 - 2 - (row(selection.item) * 2);
                    currentConnector.y2 = currentConnector.y1; 
                    currentConnector.x3 = currentConnector.x2;
                    currentConnector.y3 = 2; 
                    break;
                    
                  case NO_GATE_SELECTED: break;

                  default:
                  {
                    int8_t colDiff = column(selection.item) - column(selection.otherItem);
                    currentConnector.x2 = 20 + currentConnector.x1 - (colDiff * 28) + (row(selection.otherItem) * 2);
                    currentConnector.y2 = currentConnector.y1; 
                    currentConnector.x3 = currentConnector.x2;
                    currentConnector.y3 = 12 + yOffset(selection.otherItem) + (row(selection.otherItem) * 18); 
                    currentConnector.x4 = 26 + (column(selection.otherItem) * 28) + (column(selection.otherItem) == 0 ? 4 : 0);
                    currentConnector.y4 = currentConnector.y3; 
                  }
                  break;

                }

              }
              break;

            case CONNECTOR_OUTPUT:
              {
                if (arduboy.justPressed(UP_BUTTON))     { moveSelectedOtherUp(&selection, level.items, MOVEMENT_NO_RESTRICTIONS); }
                if (arduboy.justPressed(DOWN_BUTTON))   { moveSelectedOtherDown(&selection, level.items, MOVEMENT_NO_RESTRICTIONS); }
                if (arduboy.justPressed(RIGHT_BUTTON))  { moveSelectedOtherRight(&selection, level.items, MOVEMENT_PREVENT_LEFT | MOVEMENT_ALLOW_RIGHT); }
                if (arduboy.justPressed(LEFT_BUTTON))   { moveSelectedOtherLeft(&selection, level.items, MOVEMENT_PREVENT_LEFT | MOVEMENT_ALLOW_RIGHT); }

                if (selection.otherItem != NO_GATE_SELECTED) {

                  int8_t colDiff = column(selection.otherItem) - column(selection.item);


                  // Draw connector ..

                  currentConnector.x1 = 30 + (column(selection.item) * 28); 
                  currentConnector.y1 = 12 + yOffset(selection.item) + (row(selection.item) * 18); 
                  currentConnector.x2 = currentConnector.x1 + ((colDiff - 1) * 28) + 2 + (row(selection.item) * 2); 
                  currentConnector.y2 = currentConnector.y1;
                  currentConnector.x3 = currentConnector.x2;
                  currentConnector.y3 = 8 + yOffset(selection.otherItem) + (row(selection.otherItem) * 18) + (level.items[selection.otherItem].type == ItemType::NOT ? 4 : 0) + (selection.otherConnector == CONNECTOR_INPUT_A ? 0 : 8);
                  currentConnector.x4 = 2 + (column(selection.item) * 28) + ((colDiff + 0) * 28) + 11;
                  currentConnector.y4 = currentConnector.y3;

                }
                else {

                  currentConnector.reset();

                }
                
              }
              break;

          }

          break;

        case 9 ... 11: // LEDs
          {
            if (arduboy.justPressed(UP_BUTTON))     { moveSelectedOtherUp(&selection, level.items, MOVEMENT_NO_RESTRICTIONS); }
            if (arduboy.justPressed(DOWN_BUTTON))   { moveSelectedOtherDown(&selection, level.items, MOVEMENT_NO_RESTRICTIONS); }
            if (arduboy.justPressed(RIGHT_BUTTON))  { moveSelectedOtherRight(&selection, level.items, MOVEMENT_ALLOW_LEFT | MOVEMENT_PREVENT_RIGHT); }
            if (arduboy.justPressed(LEFT_BUTTON))   { moveSelectedOtherLeft(&selection, level.items, MOVEMENT_ALLOW_LEFT | MOVEMENT_PREVENT_RIGHT); }

            currentConnector.reset();

            int8_t colDiff = column(selection.item) - column(selection.otherItem);

            if (selection.otherItem != NO_GATE_SELECTED) {

              currentConnector.x1 = 13 + (column(selection.item) * 28);
              currentConnector.y1 = 8 + yOffset(selection.item) + (row(selection.item) * 18) + (selection.connector == 0 ? 0 : 8); 
              currentConnector.x2 = 20 + currentConnector.x1 - (colDiff * 28) + (row(selection.otherItem) * 2);
              currentConnector.y2 = currentConnector.y1; 
              currentConnector.x3 = currentConnector.x2;
              currentConnector.y3 = 12 + yOffset(selection.otherItem) + (row(selection.otherItem) * 18); 
              currentConnector.x4 = 26 + (column(selection.otherItem) * 28) + (column(selection.otherItem) == 0 ? 4 : 0);
              currentConnector.y4 = currentConnector.y3; 

            }

          }

          break;

      }

    }

  }


  //----------------------------------------------------------------------------------------------------------------------
  // Create the new connection ..

  else if (arduboy.justReleased(A_BUTTON)) {

    createOrDeleteConnection(&selection, &currentConnector, connectors);
    updateResults(level.items, connectors);

  }


  //----------------------------------------------------------------------------------------------------------------------
  // Handle change of gate ..

  else if (arduboy.pressed(B_BUTTON)) {

    if (arduboy.justPressed(B_BUTTON)) {
      oldType = level.items[selection.item].type;
    }
    
    switch (column(selection.item)) {

      case 0 ... 2:
        {

          Gate *gate = &level.items[selection.item];

          if (gate->editable) {

            if (arduboy.justPressed(UP_BUTTON)) {
              
              gate->type = ++gate->type;
              updateResults(level.items, connectors);

            }

            if (arduboy.justPressed(DOWN_BUTTON)) {
              
              gate->type = --gate->type;
              updateResults(level.items, connectors);

            }

            displayGateSelector = 1;

          }
          else {
      
            displayGateSelector = 2;
            
          }

        }  
         
        break;

      case 3:          
        displayGateSelector = 2;
        break;
      
    }

  }


  //----------------------------------------------------------------------------------------------------------------------
  // If the gate has been changed then delete the connections ..

  else if (arduboy.justReleased(B_BUTTON)) {

    if (selection.item != SELECTED_MENU && selection.item != NO_GATE_SELECTED && oldType != ItemType::BLANK && oldType != level.items[selection.item].type && (
         (oldType <= ItemType::XNOR && level.items[selection.item].type > ItemType::XNOR) ||
         (oldType > ItemType::XNOR && level.items[selection.item].type <= ItemType::XNOR) ||
         (level.items[selection.item].type == ItemType::BLANK) ||
         (level.items[selection.item].type == ItemType::NOT)
       )) {

         clearConnectors(connectors, selection.item);

    }

  }

  render();

}