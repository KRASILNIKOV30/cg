#pragma once
#include "../window/Window.h"
#include "../render/Renderer.h"
#include "../scene/Scene.h"


class RaytraceView final : public Window
{
public:
	RaytraceView(int width, int height);
	~RaytraceView() override;

	SceneObject& AddSphere(std::shared_ptr<IShader const> const& shader, double radius = 1, Vector3d const& center = Vector3d(), Matrix4d const& transform = {});
	//SceneObject& AddConicCylinder(std::shared_ptr<IShader const> shader, double height = 1, double baseRadius = 1, double capRadius = 0, CMatrix4d const& transform = {});
	SceneObject& AddPlane(const std::shared_ptr<IShader const>& shader, double a, double b, double c, double d, Matrix4d const& transform = Matrix4d());
	SceneObject& AddSceneObject(std::shared_ptr<IGeometryObject const> object, std::shared_ptr<IShader const> shader);
	SceneObject& AddParaboloid(std::shared_ptr<IShader const> const& shader, double scale, Vector3d const& center, Matrix4d const& transform = {});

private:
	void OnUpdate() override;
	void OnRender() override;

	void InitializeScene();
	void AddSomePlane();
	void AddSomeSpheres();
	void AddSomeLight();
	void AddSomeParaboloid();
	// void AddSomeConicCylinders();

	RenderContext m_context;
	Renderer m_renderer;
	Scene m_scene;
	std::unique_ptr<FrameBuffer> m_pFrameBuffer;
};