set(SOIL_SOURCES
        ../lib/SOIL/SOIL.c
        ../lib/SOIL/image_DXT.c
        ../lib/SOIL/image_helper.c
        ../lib/SOIL/stb_image_aug.c
)

set(TEXTURE_SOURCES
        ../lib/texture/Texture.h
        ../lib/texture/TextureLoader.h
        ../lib/texture/TextureLoader.cpp
)

set(SIGNALS_SOURCES
        ../lib/signals/SignallingValue.h
)

set(GLFW_WINDOW_SOURCES
        ../lib/glfwWindow/BaseWindow.h
        ../lib/glfwWindow/BaseWindow.cpp
        ../lib/glfwWindow/GLFWInitializer.h
        ../lib/glfwWindow/GLFWInitializer.cpp
)

add_executable(battle-city
        ${SOIL_SOURCES}
        ${SIGNALS_SOURCES}
        ${TEXTURE_SOURCES}
        ${GLFW_WINDOW_SOURCES}
        main.cpp
        model/Tank.h
        model/Point.h
        model/FieldScheme.h
        model/Field.h
        model/Field.cpp
        3d/Vector3.h
        3d/Vertex.h
        3d/boundingBox/BoundingBox.h
        3d/boundingBox/BoundingBox.cpp
        3d/mesh/Mesh.h
        3d/mesh/Mesh.cpp
        3d/buffer/BufferObject.h
        3d/buffer/Buffer.h
        3d/model/Model.h
        3d/model/Model.cpp
        3d/textureMap/TextureMap.h
        3d/textureMap/TextureMap.cpp
        3d/material/Material.h
        3d/material/Material.cpp
        3d/material/ModelMaterial.h
        3d/material/ModelMaterial.cpp
        3d/model/ModelRenderer.h
        3d/model/ModelRenderer.cpp
        3d/modelLoader/ModelLoader.h
        3d/modelLoader/ModelLoader.cpp
        window/Window.h
        window/Window.cpp
        3d/render/TankRenderer.h
        3d/render/TankRenderer.h
        3d/render/FieldRenderer.h
)


find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(battle-city
        glfw
        ${OPENGL_LIBRARIES}
        GLEW
        tinyobjloader
)