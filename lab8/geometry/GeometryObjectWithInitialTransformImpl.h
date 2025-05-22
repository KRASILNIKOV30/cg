#pragma once
#include "GeometryObjectImpl.h"

class GeometryObjectWithInitialTransformImpl : public GeometryObjectImpl
{
public:
	explicit GeometryObjectWithInitialTransformImpl(Matrix4d const& transform = Matrix4d())
		: GeometryObjectImpl(transform)
	{
		UpdateInverseTransform();
	}

	/*
	Перегруженный метод получения матрицы обратной трансформации объекта
	*/
	[[nodiscard]] Matrix4d const& GetInverseTransform() const override
	{
		// При последующих вызовах метода GetInverseTransform() будет возвращено ранее вычисленное преобразование
		return m_inverseTransform;
	}

protected:
	void SetInitialTransform(Matrix4d const& initialTransform)
	{
		m_initialTransform = initialTransform;
		UpdateInverseTransform();
	}

	void OnUpdateTransform() override
	{
		GeometryObjectImpl::OnUpdateTransform();

		UpdateInverseTransform();
	}

private:
	void UpdateInverseTransform()
	{
		// Инвертируем матрицу начального преобразования
		const Matrix4d inverseInitialTransform = m_initialTransform.GetInverseMatrix();

		// Получаем обратную матрицу преобразования геометрического объект в целом
		Matrix4d const& inverseGeomObjectTransform = GeometryObjectImpl::GetInverseTransform();

		// Вычисляем результирующую матрицу преобразования, перемножив обратные матрицы в обратном порядке
		m_inverseTransform = inverseInitialTransform * inverseGeomObjectTransform;
	}

private:
	/*
	Параметры обратной матрицы трансформации, построенной с учетом начального
	преобразования базовой сферы и матрицы трансформации сферы
	*/
	Matrix4d m_inverseTransform;

	/*
	Начальная трансформация, выполняющая преобразование базовой сферы единичного радиуса с центром
	в начале координат в сферу заданного радиуса с центром в указанной точке
	*/
	Matrix4d m_initialTransform;
};