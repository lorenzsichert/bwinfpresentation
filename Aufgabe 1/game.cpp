#include "game.h"
#include "solution.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>


Game::Game() {
  system = new System;
  system->unit = 1000;
  engine.start(system, 900, 900, "Aufgabe 1");
  engine.setClearColor(glm::vec4(1,1,1,1));
}

void Game::run() {
  Solution solution;
  //solution.init(system, "../../../a1/res/wenigerkrumm4.txt");
  solution.initRandom(system, 200, 10); // 5, 6, 27, 31
  // 8: 6, 31, 45
  // 16: 10, 32, 34

  bool space_pressed = false;
  bool right_pressed = false;
  int state = 0;

  do {
    engine.update();
    if (glfwGetKey(engine.window, GLFW_KEY_SPACE) == GLFW_PRESS && space_pressed == false) {
      space_pressed = true;
      solution.solve();  
      std::cout << "Done" << std::endl;
    }
    if (glfwGetKey(engine.window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
      space_pressed = false;
    }
    if (glfwGetKey(engine.window, GLFW_KEY_RIGHT) == GLFW_PRESS && right_pressed == false) {
      right_pressed = true;
      if (state == 0) {
        solution.solveDFS();  
        state++;
      } else {
        solution.solve3Opt();
      }

    }
    if (glfwGetKey(engine.window, GLFW_KEY_RIGHT) == GLFW_RELEASE) {
      right_pressed = false;
    }

    // Calc View Matrices
    system->projectionMatrix = glm::orthoRH(-(float)system->unit * system->aspectRatio, (float)system->unit * system->aspectRatio, -(float)system->unit, (float)system->unit, -1000.f, 1000.f);
    system->viewMatrix = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0)) * glm::scale(glm::vec3(system->camera.zoom, system->camera.zoom,1)) * glm::translate(-system->camera.position);
    system->screenMatrix = glm::orthoRH(-(float)system->unit * system->aspectRatio, (float)system->unit * system->aspectRatio, -(float)system->unit, (float)system->unit, -1000.f, 1000.f);

    solution.draw();

    glfwPollEvents();
    glfwSwapBuffers(engine.window);
  } while (glfwGetKey(engine.window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(engine.window));
}
