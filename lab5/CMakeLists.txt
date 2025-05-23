set(SOIL_SOURCES
        lib/SOIL/SOIL.c
        lib/SOIL/image_DXT.c
        lib/SOIL/image_helper.c
        lib/SOIL/stb_image_aug.c
        memoryTrainer/View/TileView.h
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

add_executable(memory-trainer
        memoryTrainer/main.cpp
        lib/TextureLoader.cpp
        lib/TextureLoader.h
        lib/Texture.h
        lib/GLFWInitializer.cpp
        lib/BaseWindow.cpp
        memoryTrainer/main.cpp
        memoryTrainer/Window.h
        memoryTrainer/model/Model.h
        memoryTrainer/model/Scheme.h
        memoryTrainer/Window.cpp
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