#pragma once

/*
Êëàññ "Êîîðäèíàòíûé ôðåéì" äëÿ âèçóàëèçàöèè îñåé êîîðäèíàò
*/
class Frame
{
public:
	// Èíèöèàëèçàöèÿ êîîðäèíàòíîãî ôðåéìà
	// size - äëèíà ñòðåëêè,
	Frame(float size = 1, float endSize = 0.1);

	// Ðèñóåì êîîðäèíàòíûå îñè
	void Draw(void) const;

private:
	float m_size;
	float m_endSize;
};