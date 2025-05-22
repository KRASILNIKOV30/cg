#pragma once
#include "IShader.h"
#include "ShadeContext.h"
#include "../geometry/matrix/Matrix4.h"
#include "../geometry/vector/VectorMath.h"

/*
Шейдер шахматной доски
*/
class CheckerShader final : public IShader
{
public:
	// Инициализация шейдера матрицей преобразования текстурных координат
	explicit CheckerShader(Matrix4d const& textureTransform = Matrix4d())
		: m_textureTransform(textureTransform)
	{
	}

	void SetTextureTransform(Matrix4d const& textureTransform)
	{
		m_textureTransform = textureTransform;
	}

	[[nodiscard]] Vector4f Shade(ShadeContext const& shadeContext) const override
	{
		/*
			Шейдер шахматной доски подвергает точку, заданную в системе координат объекта,
			преобразованию, заданному матрицей трансформации.
			У полученной точки определяется принадлежность к черной или белой клетке трехмерного шахматного пространства
			*/

		// Представление точки в виде 4-мерного вектора
		const Vector4d pt(shadeContext.GetSurfacePointInObjectSpace(), 1.0);
		// Трансформируем координаты матрицей трансформации текстурных координат
		const Vector3d transformedPoint = (m_textureTransform * pt).Project();
		/*
		Вычисляем дробную часть координат точки в систем координат объекта
		*/
		const Vector3d fract = Fract(transformedPoint);
		/*
		Координаты, превышающие 0.5, будут иметь значение 1, а не превышающие - 0
		*/
		Vector3d s = Step(0.5, fract);

		/*
		Применяем операцию XOR для определения принадлежности точки либо к черному, либо к белому кубу
		*/
		if (static_cast<int>(s.x) ^ static_cast<int>(s.y) ^ static_cast<int>(s.z))
		{
			return { 0.9f, 0.9f, 0.9f, 1 };
		}

		return { 0.1f, 0.1f, 0.1f, 1 };

	}

private:
	Matrix4d m_textureTransform;
};