#include <Arduboy2.h>

Arduboy2 arduboy;


void setup() {

  arduboy.begin();
  arduboy.setFrameRate(15);

}


void loop() {

  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();

  switch (state) {


  }

  arduboy.display();

}


