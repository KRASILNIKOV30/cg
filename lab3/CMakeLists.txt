find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(tetris
        tetris/main.cpp
        tetris/model/Model.h
        tetris/controller/Controller.h
        tetris/view/View.h
        tetris/BaseWindow.h
        tetris/model/Tetromino.h
)

target_link_libraries(tetris glfw)
target_link_libraries(tetris OpenGL)