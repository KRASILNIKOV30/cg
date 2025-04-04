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

add_executable(mesh
        mesh/main.cpp
        mesh/BaseWindow.cpp
        mesh/BaseWindow.h
        mesh/GLFWInitializer.cpp
        mesh/GLFWInitializer.h
        mesh/Mesh.cpp
        mesh/Mesh.h
        mesh/Window.cpp
        mesh/Window.h
        mesh/Surface.cpp
        mesh/Surface.h
        mesh/Frame.cpp
        mesh/Frame.h
        mesh/SincSurface.cpp
        mesh/SincSurface.h
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
        GLU
        ${GLUT_LIBRARIES}
)

target_link_libraries(mesh
        glfw
        ${OPENGL_LIBRARIES}
        GLU
        ${GLUT_LIBRARIES}
)