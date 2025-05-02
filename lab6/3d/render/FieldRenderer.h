#pragma once
#include "../../model/Field.h"
#include "../model/Model.h"
#include "lab6/3d/model/ModelRenderer.h"
#include "lab6/3d/modelLoader/ModelLoader.h"

class FieldRenderer
{
public:
	void Render(Field const& model)
	{
		if (!m_loaded)
		{
			Load();
		}

		m_renderer.RenderModel(m_brick);
	}

private:
	void Load()
	{
		const ModelLoader loader;
		loader.LoadObjFile("cube.obj", m_brick);
		m_loaded = true;
	}

private:
	bool m_loaded = false;
	Model m_brick;
	Model m_water;
	Model m_armor;
	ModelRenderer m_renderer;
};