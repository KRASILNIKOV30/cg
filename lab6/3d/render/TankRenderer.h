#pragma once
#include "../model/Model.h"
#include "../modelLoader/ModelLoader.h"
#include "../../model/Tank.h"
#include "../model/ModelRenderer.h"

class TankRenderer
{
public:
	void Render(TankModel const& tank)
	{
		if (!m_loaded)
		{
			Load();
		}

		glPushMatrix();

		const auto [x, y] = tank.GetPosition();
		const auto rotation = tank.GetRotation();

		glTranslatef(x, 0, y);
		const auto angle = GetAngle(rotation);
		glRotatef(angle, 0, 1, 0);

		ModelRenderer::RenderModel(m_model);

		glPopMatrix();
	}

private:
	void Load()
	{
		const ModelLoader loader;
		loader.LoadObjFile("res/model.obj", m_model);
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
};