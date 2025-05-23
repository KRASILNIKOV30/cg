#pragma once
#include "ILightSource.h"
#include "../geometry/matrix/Matrix4.h"
#include "../geometry/vector/Vector4.h"

/*
Реализация методов, общих для различных типов источников света
*/
class LightSourceImpl : public ILightSource
{
public:
	/*
	Получение матрицы трансформации источника света
	*/
	[[nodiscard]] Matrix4d const& GetTransform() const override
	{
		return m_transform;
	}

	/*
	Установка матрицы трансформации источника света
	*/
	void SetTransform(Matrix4d const& transform) override
	{
		m_transform = transform;
	}

	/*
	Установка интенсивности диффузной составляющей света
	*/
	void SetDiffuseIntensity(Vector4f const& diffuseIntensity) override
	{
		m_diffuseIntensity = diffuseIntensity;
	}

	/*
	Получение интенсивности диффузной составляющей источника света
	*/
	[[nodiscard]] Vector4f const& GetDiffuseIntensity() const override
	{
		return m_diffuseIntensity;
	}

	void SetAmbientIntensity(Vector4f const& ambientIntensity) override
	{
		m_ambientIntensity = ambientIntensity;
	}

	[[nodiscard]] Vector4f const& GetAmbientIntensity() const override
	{
		return m_ambientIntensity;
	}

	void SetSpecularIntensity(Vector4f const& specularIntensity) override
	{
		m_specularIntensity = specularIntensity;
	}

	[[nodiscard]] Vector4f const& GetSpecularIntensity() const override
	{
		return m_specularIntensity;
	}

protected:
	explicit LightSourceImpl(Matrix4d const& transform = Matrix4d())
		: m_transform(transform)
	{
	}

private:
	Vector4f m_diffuseIntensity;
	Vector4f m_ambientIntensity;
	Vector4f m_specularIntensity;
	Matrix4d m_transform;
};