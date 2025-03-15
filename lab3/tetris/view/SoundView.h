#pragma once
#include "../model/Model.h"
#include <SFML/Audio.hpp>

class SoundView
{
public:
	explicit SoundView(Model& model)
		: m_model(model)
	{
		m_lineClearedConnection = m_model.DoOnLineCleared([&] { LineCleared(); });
		m_newLevelConnection = m_model.DoOnNewLevel([&] { NewLevel(); });
		m_blockFallenConnection = m_model.DoOnBlockFallen([&] { BlockFallen(); });
		m_gameOverConnection = m_model.DoOnGameOver([&] { GameOver(); });

		if (!m_lineClearedBuffer.loadFromFile("./res/line_cleared.wav"))
		{
			throw std::runtime_error("Failed to load line_cleared.wav");
		}
		m_lineClearedSound.setBuffer(m_lineClearedBuffer);

		if (!m_newLevelBuffer.loadFromFile("./res/new_level.wav"))
		{
			throw std::runtime_error("Failed to load new_level.wav");
		}
		m_newLevelSound.setBuffer(m_newLevelBuffer);

		if (!m_blockFallenBuffer.loadFromFile("./res/block_fallen.wav"))
		{
			throw std::runtime_error("Failed to load block_fallen.wav");
		}
		m_blockFallenSound.setBuffer(m_blockFallenBuffer);

		if (!m_gameOverBuffer.loadFromFile("./res/game_over.wav"))
		{
			throw std::runtime_error("Failed to load game_over.wav");
		}
		m_gameOverSound.setBuffer(m_gameOverBuffer);
		if (!m_backgroundMusic.openFromFile("./res/music.wav"))
		{
			throw std::runtime_error("Failed to load music.wav");
		}

		m_backgroundMusic.setLoop(true);
		m_backgroundMusic.setVolume(30);
		m_backgroundMusic.play();
	}

	void LineCleared()
	{
		m_lineClearedSound.play();
	}

	void NewLevel()
	{
		m_newLevelSound.play();
	}

	void BlockFallen()
	{
		m_blockFallenSound.play();
	}

	void GameOver()
	{
		m_gameOverSound.play();
		m_backgroundMusic.stop();
	}

private:
	Model& m_model;

	sf::SoundBuffer m_lineClearedBuffer;
	sf::Sound m_lineClearedSound;

	sf::SoundBuffer m_newLevelBuffer;
	sf::Sound m_newLevelSound;

	sf::SoundBuffer m_blockFallenBuffer;
	sf::Sound m_blockFallenSound;

	sf::SoundBuffer m_gameOverBuffer;
	sf::Sound m_gameOverSound;

	sf::Music m_backgroundMusic;

	ScopedConnection m_lineClearedConnection;
	ScopedConnection m_newLevelConnection;
	ScopedConnection m_blockFallenConnection;
	ScopedConnection m_gameOverConnection;
};