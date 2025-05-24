#include "RaytraceView.h"
#include "../shader/CheckerShader.h"
#include "../objects/Plane.h"
#include "../material/SimpleMaterial.h"
#include "../objects/Sphere.h"
#include "../light/OmniLightSource.h"
#include "../objects/Paraboloid.h"
#include "../objects/Torus.h"
#include "../shader/PhongLightShader.h"

RaytraceView::RaytraceView(int width, int height)
	: Window(width, height, "Raytracer"),
	  m_pFrameBuffer(std::make_unique<FrameBuffer>(width, height))
{
	InitializeScene();
	m_renderer.Render(m_scene, m_context, *m_pFrameBuffer); // Запуск рендеринга
}

RaytraceView::~RaytraceView()
{
	m_renderer.Stop();
}

void RaytraceView::OnUpdate()
{
}

void RaytraceView::OnRender()
{
	// Очищаем экран
	SDL_SetRenderDrawColor(GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(GetRenderer());

	if (m_pFrameBuffer)
	{
		UpdateTextureFromPixels(
			m_pFrameBuffer->GetPixels(),
			GetWidth() * static_cast<int>(sizeof(uint32_t))
			);
	}
}

void RaytraceView::InitializeScene()
{
	m_scene.SetBackdropColor(Vector4f(0.8, 0.1, 0.8, 1));

	AddSomePlane();
	AddSomeLight();

	//AddSomeSpheres();
	//AddSomeParaboloid();
	AddSomeTorus();

	const auto width = GetWidth();
	const auto height = GetHeight();

	m_context.SetViewPort(ViewPort(0, 0, width, height));
	Matrix4d proj;
	proj.LoadPerspective(60, static_cast<double>(width) / height, 0.1, 10);
	m_context.SetProjectionMatrix(proj);

	Matrix4d modelView;
	modelView.LoadLookAtRH(0, 4, 5, 0, 1.5, 0, 0, 1, 0);
	m_context.SetModelViewMatrix(modelView);
}

void RaytraceView::AddSomePlane()
{
	Matrix4d floorTransform;
	floorTransform.Scale(0.2, 0.2, 0.2);
	floorTransform.Translate(0.25, 0.25, 0.25);
	SimpleMaterial floorMaterial(
		{ 0.8, 0.8, 0.8, 1 },
		{ 0.2, 0.2, 0.2, 1 },
		{ 0.1, 0.1, 0.1, 1 },
		0
		);
	AddPlane(std::make_shared<PhongLightShader>(floorMaterial), 0, 1, 0, 0);
}

void RaytraceView::AddSomeSpheres()
{
	SimpleMaterial yellow({ 0.8, 0.8, 0, 1 }, { 0.2, 0.2, 0.2, 1 }, { 0.4, 0.4, 0.4, 1 }, 100);
	const auto phongShader = std::make_shared<PhongLightShader>(yellow);
	AddSphere(phongShader, 1.5, Vector3d(-1, 1.5, 2));
	AddSphere(phongShader, 1, Vector3d(2.5, 0, 0));
}

// Создаем и добавляем в сцену точечный источник света
void RaytraceView::AddSomeLight()
{
	const OmniLightPtr pLight(new OmniLightSource(Vector3d(-3, 5, 5)));
	pLight->SetDiffuseIntensity({ 1, 1, 1, 1 });
	pLight->SetAmbientIntensity({ 0.5, 0.5, 0.5, 1 });
	pLight->SetSpecularIntensity({ 1, 1, 1, 1 });
	pLight->SetAttenuation(1, 0, 0.0005);
	m_scene.AddLightSource(pLight);
}

void RaytraceView::AddSomeParaboloid()
{
	Matrix4d transform;
	transform.Rotate(-90, 1.0, 0, 0);
	SimpleMaterial green({ 0.2, 0.8, 0.3, 1 }, { 0.2, 0.4, 0.2, 1 }, { 0.8, 0.8, 0.8, 1 }, 100);
	SimpleMaterial red({ 0.8, 0.2, 0.3, 1 }, { 0.3, 0.4, 0.2, 1 }, { 0.8, 0.8, 0.8, 1 }, 100);
	SimpleMaterial blue({ 0.5, 0.2, 0.8, 1 }, { 0.4, 0.3, 0.4, 1 }, { 0.8, 0.8, 0.8, 1 }, 100);
	const auto greenShader = std::make_shared<PhongLightShader>(green);
	const auto redShader = std::make_shared<PhongLightShader>(red);
	const auto blueShader = std::make_shared<PhongLightShader>(blue);
	AddParaboloid(greenShader, 1.5, Vector3d(0, 0, 0), transform);
	AddParaboloid(redShader, 1.0, Vector3d(0, 0, 1.5), transform);
	AddParaboloid(blueShader, 0.5, Vector3d(0, 0, 2.5), transform);
}

void RaytraceView::AddSomeTorus()
{
	const Matrix4d transform;
	SimpleMaterial green({ 0.2, 0.8, 0.3, 1 }, { 0.2, 0.4, 0.2, 1 }, { 0.8, 0.8, 0.8, 1 }, 100);
	SimpleMaterial red({ 0.8, 0.2, 0.3, 1 }, { 0.3, 0.4, 0.2, 1 }, { 0.8, 0.8, 0.8, 1 }, 100);
	SimpleMaterial blue({ 0.3, 0.2, 0.8, 1 }, { 0.3, 0.3, 0.4, 1 }, { 0.8, 0.8, 0.8, 1 }, 100);
	SimpleMaterial yellow({ 0.8, 0.8, 0, 1 }, { 0.2, 0.2, 0.2, 1 }, { 0.4, 0.4, 0.4, 1 }, 100);

	const auto greenShader = std::make_shared<PhongLightShader>(green);
	const auto redShader = std::make_shared<PhongLightShader>(red);
	const auto blueShader = std::make_shared<PhongLightShader>(blue);
	const auto yellowShader = std::make_shared<PhongLightShader>(yellow);

	AddTorus(blueShader, 1.5, 0.4, Vector3d(0, 0, 0), transform);
	AddTorus(redShader, 1.2, 0.35, Vector3d(0, 0.5, 0), transform);
	AddTorus(greenShader, 1.0, 0.3, Vector3d(0, 1.0, 0), transform);
	AddSphere(yellowShader, 0.9, { 0, 1.5, 0 }, transform);
}

SceneObject& RaytraceView::AddPlane(const std::shared_ptr<IShader const>& shader, double a, double b, double c, double d, Matrix4d const& transform)
{
	return AddSceneObject(std::make_shared<Plane>(a, b, c, d, transform), shader);
}

SceneObject& RaytraceView::AddSceneObject(std::shared_ptr<IGeometryObject const> object, std::shared_ptr<IShader const> shader)
{
	const auto obj = std::make_shared<SceneObject>(std::move(object), std::move(shader));
	m_scene.AddObject(obj);
	return *obj;
}

SceneObject& RaytraceView::AddParaboloid(std::shared_ptr<IShader const> const& shader, double scale, Vector3d const& center, Matrix4d const& transform)
{
	return AddSceneObject(std::make_shared<Paraboloid>(scale, center, transform), shader);
}

SceneObject& RaytraceView::AddTorus(std::shared_ptr<IShader const> const& shader, double majorRadius, double minorRadius, Vector3d const& center, Matrix4d const& transform)
{
	return AddSceneObject(std::make_shared<Torus>(majorRadius, minorRadius, center, transform), shader);
}

SceneObject& RaytraceView::AddSphere(std::shared_ptr<IShader const> const& shader, double radius, Vector3d const& center, Matrix4d const& transform)
{
	return AddSceneObject(std::make_shared<Sphere>(radius, center, transform), shader);
}
