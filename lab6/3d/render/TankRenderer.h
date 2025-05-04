#pragma once
#include "../model/Model.h"
#include "../modelLoader/ModelLoader.h"
#include "../../model/Tank.h"
#include "../model/ModelRenderer.h"

#include <iostream>

class TankRenderer
{
public:
	void Render(TankModel const& tank)
	{
		if (!m_loaded)
		{
			Load();
		}

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		const auto [x, y] = tank.GetPosition();
		const auto rotation = tank.GetRotation();

		glTranslatef(x, 0, y);
		const auto angle = GetAngle(rotation);
		glRotatef(angle, 0, 1, 0);

		glScalef(m_scale, m_scale, m_scale);

		m_renderer.RenderModel(m_model);

		glPopMatrix();
	}

	[[nodiscard]] float GetLength()
	{
		if (!m_loaded)
		{
			Load();
		}
		const auto bb = m_model.GetBoundingBox();
		const auto length = bb.GetMaxCoord().z - bb.GetMinCoord().z;
		return length * m_scale;
	}

private:
	void Load()
	{
		const ModelLoader loader;
		loader.LoadObjFile("tank.obj", m_model);
		const auto bb = m_model.GetBoundingBox();
		const auto size = bb.GetSize();
		const auto width = std::min(size.x, size.z);
		m_scale = (1.0f / width) * 0.8f;
		m_loaded = true;
	}

	static float GetAngle(const TankRotation rotation)
	{
		switch (rotation)
		{
		case TankRotation::UP:
			return 0.0f;
		case TankRotation::RIGHT:
			return 90.0f;
		case TankRotation::DOWN:
			return 180.0f;
		case TankRotation::LEFT:
			return 270.0f;
		}
		return 0;
	}

private:
	Model m_model;
	bool m_loaded = false;
	ModelRenderer m_renderer;
	float m_scale = 1.0f;
};