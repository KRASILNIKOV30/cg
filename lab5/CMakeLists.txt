set(SOIL_SOURCES
        lib/SOIL/src/SOIL.c
        lib/SOIL/src/image_DXT.c
        lib/SOIL/src/image_helper.c
        lib/SOIL/src/stb_image_aug.c
)

add_executable(maze-with-textures
        maze/main.cpp
        lib/TextureLoader.cpp
        lib/TextureLoader.h
        lib/Texture.h
        lib/GLFWInitializer.cpp
        lib/BaseWindow.cpp
        maze/Window.cpp
        maze/Maze.cpp
        maze/Maze.h
        maze/Maze.cpp
        maze/Maze.h
        maze/MazeScheme.h
        ${SOIL_SOURCES}
)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(maze-with-textures
        glfw
        ${OPENGL_LIBRARIES}
        GLU
        ${GLUT_LIBRARIES}
)