cmake_minimum_required(VERSION 3.10)

project(Presentation)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(SFML REQUIRED COMPONENTS graphics system)

add_executable(Presentation
  main.cpp
  game.cpp
  solution.cpp
  helper.cpp
 

  engine/engine.cpp
  engine/object.cpp
  engine/texture.cpp
  engine/framebuffer.cpp
  engine/instanceObject.cpp
  engine/font.cpp
  engine/text.cpp
  engine/uiTexture.cpp
  engine/uiObject.cpp
  engine/common/loadShader.cpp
  engine/common/loadPNG.cpp
  engine/common/loadOBJ.cpp
  engine/shapes/circle.cpp
  engine/shapes/uiCircle.cpp
)

target_include_directories(Presentation PRIVATE
  ${OPENGL_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  ${SFML_INCLUDE_DIR}
)

target_link_libraries(Presentation
  ${OPENGL_LIBRARY}
  ${FREETYPE_LIBRARY}
  ${SFML_LIBRARIES}
  glfw
  GLEW
)
