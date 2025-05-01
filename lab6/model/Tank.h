#pragma once
#include "Point.h"
#include "../../lib/signals/SignallingValue.h"

enum class TankType
{
	PLAYER,
	ENEMY
};

enum class TankRotation
{
	UP, // 0°
	RIGHT, // 90°
	DOWN, // 180°
	LEFT // 270°
};

class TankModel
{
public:
	using PositionChangedSignal = EmptySignal;
	using RotationChangedSignal = EmptySignal;
	using HealthChangedSignal = EmptySignal;
	using FiredSignal = EmptySignal;

	TankModel(TankType type, const Point& initialPosition, TankRotation initialRotation)
		: m_type(type),
		  m_position(initialPosition),
		  m_rotation(initialRotation),
		  m_fireRate(30)
	{
	}

	[[nodiscard]] ScopedConnection DoOnPositionChanged(PositionChangedSignal::slot_type const& slot)
	{
		return m_positionChangedSignal.connect(slot);
	}

	[[nodiscard]] ScopedConnection DoOnRotationChanged(RotationChangedSignal::slot_type const& slot)
	{
		return m_rotationChangedSignal.connect(slot);
	}

	[[nodiscard]] ScopedConnection DoOnHealthChanged(HealthChangedSignal::slot_type const& slot)
	{
		return m_healthChangedSignal.connect(slot);
	}

	[[nodiscard]] ScopedConnection DoOnFired(FiredSignal::slot_type const& slot)
	{
		return m_firedSignal.connect(slot);
	}

	[[nodiscard]] TankType GetType() const
	{
		return m_type;
	}

	[[nodiscard]] Point GetPosition() const
	{
		return m_position;
	}

	[[nodiscard]] TankRotation GetRotation() const
	{
		return m_rotation;
	}

	[[nodiscard]] int GetHealth() const
	{
		return m_health;
	}

	[[nodiscard]] int GetMaxHealth() const
	{
		return m_maxHealth;
	}

	[[nodiscard]] int GetSpeed() const
	{
		return m_speed;
	}

	[[nodiscard]] bool CanFire() const
	{
		return m_fireCooldown <= 0;
	}

	void MoveForward()
	{
		switch (m_rotation)
		{
		case TankRotation::UP:
			m_position.y -= m_speed;
			break;
		case TankRotation::RIGHT:
			m_position.x += m_speed;
			break;
		case TankRotation::DOWN:
			m_position.y += m_speed;
			break;
		case TankRotation::LEFT:
			m_position.x -= m_speed;
			break;
		}
		m_positionChangedSignal();
	}

	void Rotate(const bool clockwise)
	{
		switch (m_rotation)
		{
		case TankRotation::UP:
			m_rotation = clockwise ? TankRotation::RIGHT : TankRotation::LEFT;
			break;
		case TankRotation::RIGHT:
			m_rotation = clockwise ? TankRotation::DOWN : TankRotation::UP;
			break;
		case TankRotation::DOWN:
			m_rotation = clockwise ? TankRotation::LEFT : TankRotation::RIGHT;
			break;
		case TankRotation::LEFT:
			m_rotation = clockwise ? TankRotation::UP : TankRotation::DOWN;
			break;
		}
		m_rotationChangedSignal();
	}

	void Fire()
	{
		if (!CanFire())
			return;

		m_fireCooldown = m_fireRate;
		m_firedSignal();
	}

	void TakeDamage(const int damage)
	{
		m_health -= damage;
		if (m_health < 0)
			m_health = 0;
		m_healthChangedSignal();
	}

	void Update()
	{
		if (m_fireCooldown > 0)
		{
			m_fireCooldown--;
		}
	}

private:
	TankType m_type;
	Point m_position;
	TankRotation m_rotation;
	int m_health = 1;
	int m_maxHealth = 1;
	int m_speed = 2;
	int m_fireCooldown = 0;
	int m_fireRate;

	PositionChangedSignal m_positionChangedSignal;
	RotationChangedSignal m_rotationChangedSignal;
	HealthChangedSignal m_healthChangedSignal;
	FiredSignal m_firedSignal;
};