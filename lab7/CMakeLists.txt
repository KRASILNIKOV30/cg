set(GLFW_WINDOW_SOURCES
        ../lib/glfwWindow/BaseWindow.h
        ../lib/glfwWindow/BaseWindow.cpp
        ../lib/glfwWindow/GLFWInitializer.h
        ../lib/glfwWindow/GLFWInitializer.cpp
)

set(SHADERS_SOURCES
        ../lib/shaders/Shader.h
        ../lib/shaders/ShaderProgram.h
        ../lib/shaders/Program.h
        ../lib/shaders/Program.cpp
)

add_executable(canabola
        ${GLFW_WINDOW_SOURCES}
        ${SHADERS_SOURCES}
        canabola/Canabola.h
        canabola/Window.h
        canabola/Window.cpp
        canabola/main.cpp
        canabola/Flag.h
        canabola/MorphingObject.h
)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(canabola
        glfw
        ${OPENGL_LIBRARIES}
        GLEW
)