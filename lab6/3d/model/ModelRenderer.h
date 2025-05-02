#pragma once
#include <GL/glew.h>

class Model;
class Mesh;

class ModelRenderer
{
public:
	ModelRenderer();

	ModelRenderer(const ModelRenderer& other) = delete;
	ModelRenderer& operator=(const ModelRenderer& other) = delete;

	void RenderModel(Model const& model) const;

private:
	// Визуализируем подсетку, грани которой используют материал
	// materialIndex
	// Возвращает true, если при вызове метода был активирован материал
	bool RenderMaterialSubMesh(
		Model const& model, // модель
		unsigned meshIndex, // индекс сетки
		int materialIndex, // индекс материала
		bool activateMaterial // нужно ли активировать материал?
		) const;

	// Инициализируем указатели OpenGL адресами массивов вершин, нормалей
	// и текстурных координат полигональной сетки
	void SetupMeshVertexPointers(
		Mesh const& mesh, // Сетка
		GLubyte const* pVertexBufferData // Адрес буфера
		) const;

	// Визуализация граней подсетки
	// Предполагается, что адреса массивов вершин, нормалей и текстурных
	// координат уже настроены на текущую сетки
	void RenderSubMeshFaces(
		Mesh const& mesh, // сетка
		unsigned subMeshIndex, // индекс рисуемой подсетки
		GLubyte const* pMeshIndices // адрес массива индексов сетки
		) const;

	// Был ли включен массив текстурных координат?
	mutable bool m_texCoordsEnabled;

	// Был ли включен режим отбраковки граней?
	mutable bool m_cullFace;
};