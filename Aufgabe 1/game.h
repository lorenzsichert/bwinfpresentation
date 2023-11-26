#ifndef GAME_H
#define GAME_H

#include "engine/system.h"
#include "engine/engine.h"

class Game {
private:
  System* system;
  Engine engine;
public:
  Game();
  void run();
};

#endif
