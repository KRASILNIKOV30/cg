#pragma once
#include "../geometry/Ray.h"
#include "../geometry/intersection/Intersection.h"
#include "../geometry/GeometryObjectImpl.h"

/*
Геометрический объект "бесконечная плоскость"
*/
class Plane final : public GeometryObjectImpl
{
public:
	/*
	Плоскость задается коэффициентами уравнения плоскости ax+by+cz+d=0,
	а также матрицей начального преобразования плоскости
	*/
	Plane(double a, double b, double c, double d, Matrix4d const& transform = Matrix4d())
		: GeometryObjectImpl(transform)
		  , m_planeEquation(a, b, c, d)
	{
	}

	/*
	Нахождение точки пересечения луча с плоскостью
	*/
	bool Hit(Ray const& ray, Intersection& intersection) const override
	{
		// Величина, меньше которой модуль скалярного произведения вектора направления луча и
		// нормали плоскости означает параллельность луча и плоскости
		constexpr double EPSILON = 1e-10;

		// Вместо преобразования плоскости выполняем обратное преобразование луча
		// Результат будет тот же самый
		const Ray invRay = Transform(ray, GetInverseTransform());

		// Нормаль к плоскости в системе координат объекта
		const Vector3d normalInObjectSpace{ m_planeEquation };

		// Скалярное произведение направления луча и нормали к плоскости
		double normalDotDirection = Dot(invRay.GetDirection(), normalInObjectSpace);

		// Если скалярное произведение близко к нулю, луч параллелен плоскости
		if (fabs(normalDotDirection) < EPSILON)
		{
			// Луч параллелен плоскости - точек пересечения нет
			return false;
		}

		/*
		Находим время пересечения луча с плоскостью, подставляя в уравнение плоскости точку испускания луча
		и деление результата на ранее вычисленное скалярное произведение направления луча и нормали к плоскости
		*/
		const double hitTime = -Dot(Vector4d(invRay.GetStart(), 1), m_planeEquation) / normalDotDirection;

		// Нас интересует только пересечение луча с плоскостью в положительный момент времени,
		// поэтому находящуюся "позади" точки испускания луча точку пересечения мы за точку пересечения не считаем
		// Сравнение с величиной EPSILON, а не с 0 нужно для того, чтобы позволить
		// лучам, испущенным с плоскости, оторваться от нее.
		// Это необходимо при обработке вторичных лучей для построения теней и отражений и преломлений
		if (hitTime <= EPSILON)
		{
			return false;
		}

		// Вычисляем точку столкновения с лучом в системе координат сцены в момент столкновения
		const Vector3d hitPoint = ray.GetPointAtTime(hitTime);

		// Вычисляем точку столкновения с лучом в системе координат объекта
		const Vector3d hitPointInObjectSpace = invRay.GetPointAtTime(hitTime);

		// Вычисляем нормаль к плоскости в системе координат объекта
		Vector3d normalInWorldSpace = GetNormalMatrix() * normalInObjectSpace;

		// В список точек пересечения добавляем информацию о найденной точке пересечения
		intersection.AddHit(HitInfo(
			hitTime, // Когда столкнулись
			*this, // С кем
			hitPoint, hitPointInObjectSpace, // Точка соударения луча с поверхностью
			normalInWorldSpace, normalInObjectSpace // Нормаль к поверхности в точке соударения
			));

		// Точка столкновения есть, возвращаем true
		return true;
	}

private:
	// Четырехмерный вектор, хранящий коэффициенты уравнения плоскости
	Vector4d m_planeEquation;
};