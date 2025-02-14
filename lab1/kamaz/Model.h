#pragma once
#include "../signals/SignallingValue.h"

struct Point
{
	int x;
	int y;
};

class Model
{
public:
	using UpdateSignal = EmptySignal;
	using UpdateSlot = EmptySignal::slot_type;

	[[nodiscard]] ScopedConnection DoOnUpdate(UpdateSlot const& slot)
	{
		return m_updateSignal.connect(slot);
	}

	void SetPosition(Point const& position)
	{
		m_position = position;
		m_updateSignal();
	}

	[[nodiscard]] Point GetPosition() const
	{
		return m_position;
	}

private:
	Point m_position = { 300, 300 };
	UpdateSignal m_updateSignal;
};
