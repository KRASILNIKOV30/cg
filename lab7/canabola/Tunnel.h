#pragma once
#include "../../lib/texture/Texture.h"
#include "../../lib/shaders/Program.h"

class Tunnel
{
public:
	Tunnel()
		: m_program{ R"(
varying vec2 texCoord;

void main()
{
	texCoord = gl_MultiTexCoord0.st;
	gl_Position = ftransform();
}
)",
		             R"(
varying vec2 texCoord;
uniform float time;
uniform sampler2D tunnelTexture;

void main()
{
    // Нормализуем координаты в диапазон [-1, 1]
    vec2 uv = texCoord * 2.0 - 1.0;

    // Параметры туннеля
    float tunnelRadius = 1.0;
    float movementSpeed = 0.5;
    float textureScale = 0.1;

    // Позиция камеры (движемся вперед по оси Z)
    vec3 camPos = vec3(0.0, 0.0, time * movementSpeed);

    // Направление луча (ортографическая проекция)
    vec3 rayDir = normalize(vec3(uv, 1.0));

    // Находим пересечение с цилиндром (x² + y² = r²)
    float a = dot(rayDir.xy, rayDir.xy);
    float b = 2.0 * dot(camPos.xy, rayDir.xy);
    float c = dot(camPos.xy, camPos.xy) - tunnelRadius*tunnelRadius;

    float discriminant = b*b - 4.0*a*c;
    if (discriminant < 0.0) {
        gl_FragColor = vec4(0.0, 0.0, 0.1, 1.0); // Цвет фона
        return;
    }

    float t = (-b - sqrt(discriminant)) / (2.0*a);
    vec3 hitPos = camPos + rayDir * t;

    // Текстурные координаты
    float angle = atan(hitPos.y, hitPos.x);
    vec2 texCoords = vec2(
        angle / (2.0 * 3.1415926) + 0.5,  // s: 0..1 по углу
        fract(hitPos.z * textureScale)     // t: вдоль туннеля (с зацикливанием)
    );

    // Эффект перспективы
    float perspective = 1.0 / (1.0 + hitPos.z * 0.1);
    texCoords = (texCoords - 0.5) * perspective + 0.5;

    // Чтение текстуры с эффектом глубины
    float fog = exp(-0.05 * abs(hitPos.z));
    gl_FragColor = texture2D(tunnelTexture, texCoords) * fog;
}
)" }
	{
		m_colorBufferTexture.Create();
		m_colorBufferTexture.Bind();
	}

	void Render() const
	{
		glUseProgram(m_program.GetId());

		glEnable(GL_TEXTURE_2D);
		m_colorBufferTexture.Bind();

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0, 0);
			glVertex2f(-2, -1.5);

			glTexCoord2f(1, 0);
			glVertex2f(2, -1.5);

			glTexCoord2f(1, 1);
			glVertex2f(2, 1.5);

			glTexCoord2f(0, 1);
			glVertex2f(-2, 1.5);
		}
		glEnd();

		glUseProgram(0);
	}

private:
	Program m_program;
	Texture2D m_colorBufferTexture;
};