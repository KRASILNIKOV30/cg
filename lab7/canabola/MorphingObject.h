#pragma once
#include "../../lib/shaders/Program.h"
#include <chrono>

using namespace std::chrono;
constexpr float ANIMATION_SPEED = 0.2f;

class MorphingObject
{
public:
	MorphingObject()
		: m_program{ R"(
uniform float phase;

vec3 mobiusStripFn(vec4 originPoint)
{
	vec3 resultPoint;
	float x = originPoint.x * 2.0 - 1.0;
	float y = originPoint.y * 2.0 * 3.14159;

	resultPoint.x = cos(y) * (2.0 + (x / 2.0) * cos(y / 2.0));
	resultPoint.y = sin(y) * (2.0 + (x / 2.0) * cos(y / 2.0));
	resultPoint.z = (x / 2.0) * sin(y / 2.0);

	return resultPoint;
}

vec3 kleinBottleFn(vec4 originPoint)
{
        float u = originPoint.x * 2.0 * 3.14159;
    float v = originPoint.y * 2.0 * 3.14159;
    float r = 1.0;

    vec3 resultPoint;

    if (u <= 3.14159) {
        resultPoint.x = 3.0 * cos(u) * (1.0 + sin(u)) + (2.0 * (1.0 - cos(u) / 2.0)) * cos(u) * cos(v);
        resultPoint.y = -8.0 * sin(u) - 2.0 * (1.0 - cos(u) / 2.0) * sin(u) * cos(v);
    }
    else {
        resultPoint.x = 3.0 * cos(u) * (1.0 + sin(u)) + (2.0 * (1.0 - cos(u) / 2.0)) * cos(v + 3.1415926);
        resultPoint.y = -8.0 * sin(u);
    }
	resultPoint.z = -2.0 * (1.0 - cos(u) / 2.0) * sin(v);

    return resultPoint * 0.25;
}

void main()
{
	vec3 mobiusPoint = mobiusStripFn(gl_Vertex);
	vec3 kleinPoint = kleinBottleFn(gl_Vertex);
	vec3 vertex = mix(mobiusPoint, kleinPoint, phase);

	gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex, 1.0);
	gl_FrontColor = gl_Color;
}
)",
		             R"(
void main()
{
	gl_FragColor = gl_Color;
}
)" }
	{
		m_phaseLocation = m_program.GetUniformLocation("phase");
	}

	void Render() const
	{

		glUseProgram(m_program.GetId());

		glUniform1f(m_phaseLocation, m_phase);

		const float z = 0;
		float u = 0;
		float v = 0;

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBegin(GL_TRIANGLE_STRIP);
		for (v = 0; v <= 1; v += 0.01f)
		{
			for (u = 0; u <= 1; u += 0.01f)
			{
				glVertex3f(u, v, z);
				glColor3f(u, v, z);
			}
		}
		glEnd();
		glBegin(GL_TRIANGLE_STRIP);
		for (u = 0; u <= 1; u += 0.01f)
		{
			for (v = 0; v <= 1; v += 0.01f)
			{
				glVertex3f(u, v, z);
				glColor3f(u, v, z);
			}
		}
		glEnd();

		glUseProgram(0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void OnIdle(double deltaTime)
	{
		if (m_phase < 1.0f)
		{
			m_phase += static_cast<float>(deltaTime) * ANIMATION_SPEED;
		}
		else
		{
			m_phase = 1.0f;
		}
	}

private:
	Program m_program;
	float m_phase = 0.0;
	GLint m_phaseLocation;
};