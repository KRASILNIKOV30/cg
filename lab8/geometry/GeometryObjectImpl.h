#pragma once
#include "IGeometryObject.h"
#include "matrix/Matrix4.h"

/*
Реализация функционала геометрических объектов, не зависящего от типа объекта
*/
class GeometryObjectImpl : public IGeometryObject
{
public:
	/*
	Конструктор, принимающий матрицу, задающую трансформацию объекта
	*/
	explicit GeometryObjectImpl(Matrix4d const& transform = Matrix4d())
	{
		GeometryObjectImpl::SetTransform(transform);
	}

	/*
	Возвращает матрицу трансформации объекта
	*/
	[[nodiscard]] Matrix4d const& GetTransform() const override
	{
		return m_transform;
	}

	/*
	Задает матрицу трансформации объекта
	*/
	void SetTransform(Matrix4d const& transform) override
	{
		m_transform = transform;

		// Также вычисляем матрицу обратного преобразования
		m_invTransform = transform.GetInverseMatrix();

		// Матрица нормали - обратная транспонированная матрица моделирования-вида
		m_normalMatrix.SetColumn(0, Vector3d(m_invTransform.GetRow(0)));
		m_normalMatrix.SetColumn(1, Vector3d(m_invTransform.GetRow(1)));
		m_normalMatrix.SetColumn(2, Vector3d(m_invTransform.GetRow(2)));

		// Уведомляем наследников об изменении матрицы преобразования
		OnUpdateTransform();
	}

	/*
	Возвращает матрицу, обратную матрице трансформации.
	Как правило, гораздо проще применить обратную трансформацию к лучу и найти
	точку его пересечения с базовой формой геометрического объекта, нежели выполнять
	прямую трансформацию объекта и искать пересечени луча с ним
	*/
	[[nodiscard]] virtual Matrix4d const& GetInverseTransform() const
	{
		return m_invTransform;
	}

	// Матрица нормали
	[[nodiscard]] Matrix3d const& GetNormalMatrix() const
	{
		return m_normalMatrix;
	}

protected:
	/*
	Вызывается всякий раз, когда у объекта изменяется матрица трансформации
	Может быть перегружен в классах-наследниках для выполнения связанных с этим операций
	*/
	virtual void OnUpdateTransform()
	{
		// Здесь мы ничего не делаем, но классы-наследники будут перегружать данный метод
	}

private:
	// Матрица трансформации объекта
	Matrix4d m_transform;

	// Матрица нормалей
	Matrix3d m_normalMatrix;

	// Обратная матрица
	Matrix4d m_invTransform;
};