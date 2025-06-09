set(SOIL_SOURCES
        ../lib/texture/Texture.h
        ../lib/texture/TextureUtils.cpp
)

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

add_executable(glow
        ${SHADERS_SOURCES}
        ${GLFW_WINDOW_SOURCES}
        ${SOIL_SOURCES}
        window/Window.cpp
        window/Window.h
        main.cpp
        Gauss.h
        buffer/FrameBuffer.h
        buffer/RenderBuffer.h
        vector/Vector2.h
        vector/Vector3.h
        vector/Vector4.h
        matrix/Matrix3.h
        matrix/Matrix4.h
        ../lib/texture/TextureUtils.cpp
        window/Planets.h
        random/Random.h
)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

target_link_libraries(glow
        glfw
        ${OPENGL_LIBRARIES}
        GLEW
        ${GLUT_LIBRARIES}
)