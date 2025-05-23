#include "RaytraceView.h"
#include "../shader/CheckerShader.h"
#include "../objects/Plane.h"
#include "../material/SimpleMaterial.h"
#include "../shader/SimpleDiffuseShader.h"
#include "../objects/Sphere.h"
#include "../light/OmniLightSource.h"

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
	unsigned renderedChunks, totalChunks;
	if (m_renderer.GetProgress(renderedChunks, totalChunks))
	{
		// Рендеринг завершен
	}
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
	m_scene.SetBackdropColor(Vector4f(1, 0, 1, 1));

	AddSomePlane();
	AddSomeSpheres();
	// AddSomeConicCylinders();
	AddSomeLight();

	const auto width = GetWidth();
	const auto height = GetHeight();

	m_context.SetViewPort(ViewPort(0, 0, width, height));
	Matrix4d proj;
	proj.LoadPerspective(60, static_cast<double>(width) / height, 0.1, 10);
	m_context.SetProjectionMatrix(proj);

	Matrix4d modelView;
	modelView.LoadLookAtRH(0, 3, 7, 0, 0, 0, 0, 1, 0);
	m_context.SetModelViewMatrix(modelView);
}

void RaytraceView::AddSomePlane()
{
	Matrix4d checkerShaderTransform;
	checkerShaderTransform.Scale(0.2, 0.2, 0.2);
	checkerShaderTransform.Translate(0.25, 0.25, 0.25);
	AddPlane(std::make_shared<CheckerShader>(checkerShaderTransform), 0, 1, 0, 0);
}

void RaytraceView::AddSomeSpheres()
{
	SimpleMaterial yellow({ 1, 1, 0, 1 });
	auto shader = std::make_shared<SimpleDiffuseShader>(yellow);
	AddSphere(shader, 1, Vector3d(0, 1, 0));
	AddSphere(std::move(shader), 0.5, Vector3d(2, 0, 0));
}

// Создаем и добавляем в сцену точечный источник света
void RaytraceView::AddSomeLight()
{
	OmniLightPtr pLight(new OmniLightSource(Vector3d(-5, 5, 10)));
	pLight->SetDiffuseIntensity(Vector4f(1, 1, 1, 1));
	pLight->SetAttenuation(1, 0, 0.0005);
	m_scene.AddLightSource(pLight);
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

SceneObject& RaytraceView::AddSphere(std::shared_ptr<IShader const> const& shader, double radius, Vector3d const& center, Matrix4d const& transform)
{
	return AddSceneObject(std::make_shared<Sphere>(radius, center, transform), shader);
}

// Остальные методы (AddSphere, InitializeScene и т. д.) остаются без изменений,
// как в предыдущей реализации, но без зависимостей от SDL2.