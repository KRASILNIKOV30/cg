find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(SFML 2.5 COMPONENTS audio REQUIRED)

add_executable(tetris
        tetris/main.cpp
        tetris/model/Model.h
        tetris/controller/Controller.h
        tetris/view/View.h
        tetris/BaseWindow.h
        tetris/model/Tetromino.h
        tetris/view/SoundView.h
)

target_link_libraries(tetris glfw)
target_link_libraries(tetris OpenGL::GL GLUT::GLUT)
target_link_libraries(tetris sfml-audio)
