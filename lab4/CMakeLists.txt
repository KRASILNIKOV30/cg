add_executable(shape
        shape/main.cpp
        shape/BaseWindow.cpp
        shape/BaseWindow.h
        shape/pch.cpp
        shape/pch.h
        shape/GLFWInitializer.cpp
        shape/GLFWInitializer.h
        shape/DirectLight.cpp
        shape/DirectLight.h
        shape/Window.cpp
        shape/Window.h
        shape/Cube.cpp
        shape/Cube.h
        shape/GreatDodecahedron.cpp
        shape/GreatDodecahedron.h
)

add_executable(maze
        maze/main.cpp
)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(shape glfw)
target_link_libraries(shape OpenGL::GL)

target_link_libraries(maze
        glfw
        ${OPENGL_LIBRARIES}
        GLU # Добавляем GLU
        ${GLUT_LIBRARIES}
)