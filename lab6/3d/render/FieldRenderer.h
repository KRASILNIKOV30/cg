#pragma once
#include "../../model/Field.h"
#include "../model/Model.h"
#include "lab6/3d/model/ModelRenderer.h"
#include "lab6/3d/modelLoader/ModelLoader.h"

class FieldRenderer
{
public:
	void Render(Field const& field)
	{
		if (!m_loaded)
		{
			Load();
		}

		glMatrixMode(GL_MODELVIEW);
		for (int y = 0; y < FIELD_HEIGHT; y++)
		{
			for (int x = 0; x < FIELD_WIDTH; x++)
			{
				glPushMatrix();
				glTranslatef(x, 0, y);
				switch (field.GetBlock(x, y))
				{
				case BRICK:
					m_renderer.RenderModel(m_brick);
					break;
				case EARTH:
					m_renderer.RenderModel(m_ground);
					break;
				case WATER:
					m_renderer.RenderModel(m_water);
					break;
				case ARMOR:
					m_renderer.RenderModel(m_armor);
					break;
				default: ;
				}

				glPopMatrix();
			}
		}
	}

private:
	void Load()
	{
		const ModelLoader loader;
		loader.LoadObjFile("brick.obj", m_brick);
		loader.LoadObjFile("ground.obj", m_ground);
		loader.LoadObjFile("water.obj", m_water);
		loader.LoadObjFile("armor.obj", m_armor);
		m_loaded = true;
	}

private:
	bool m_loaded = false;
	Model m_brick;
	Model m_water;
	Model m_armor;
	Model m_ground;
	ModelRenderer m_renderer;
};