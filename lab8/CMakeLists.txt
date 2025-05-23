set(GEOMETRY_SOURCES
        buffer/FrameBuffer.h
        buffer/IColorBuffer.h
        buffer/ColorBuffer.h
        geometry/vector/Vector2.h
        geometry/vector/Vector3.h
        geometry/IGeometryObject.h
        geometry/vector/Vector4.h
        geometry/matrix/Matrix3.h
        geometry/matrix/Matrix4.h
        geometry/vector/VectorMath.h
        geometry/intersection/HitInfo.h
        geometry/intersection/Intersection.h
        geometry/GeometryObjectImpl.h
        geometry/GeometryObjectWithInitialTransformImpl.h
        shader/IShader.h
        shader/ShadeContext.h
        light/ILightSource.h
        light/LightSourceImpl.h
        scene/sceneObject/SceneObject.cpp
        scene/sceneObject/SceneObject.h
        scene/Scene.cpp
        scene/Scene.h
        render/ViewPort.h
        render/RenderContext.cpp
        render/RenderContext.h
        render/Renderer.cpp
        render/Renderer.h
        view/RaytraceView.cpp
        view/RaytraceView.h
        window/Window.cpp
        window/Window.h
        shader/CheckerShader.h
        objects/Plane.h
        objects/Sphere.h
        material/SimpleMaterial.h
        shader/SimpleDiffuseShader.h
        light/OmniLightSource.h
        shader/PhongLightShader.h
        random/Random.h
)

find_package(SDL2 REQUIRED)

add_executable(phong
        ${GEOMETRY_SOURCES}
        main.cpp
)

target_link_libraries(phong PRIVATE SDL2::SDL2)