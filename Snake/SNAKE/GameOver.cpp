#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "GameState.hpp"
#include "GameOver.hpp"
#include <stdlib.h>
#include <iostream>

namespace SNAKY {
	GameOver::GameOver(GameDataRef data) : _data(data), event() {
		changeState = false;

		gameOverPic.loadFromFile(RESTART_PICTURE_LOC);
		gameOverSpr.setTexture(gameOverPic);

		sf::Vector2f targetSize(800.0f / 2.0f, 800.0f / 2.0f);
		gameOverSpr.setScale(targetSize.x / gameOverSpr.getLocalBounds().width, targetSize.y / gameOverSpr.getLocalBounds().height);
		gameOverSpr.setPosition(0, 0);
	}
	void GameOver::Update() {
		while (_data->window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {//If close button clicked, closes window.
				_data->window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed) {
				changeState = true;
			}
		}
		if (changeState == true) {
			_data->machine.ReplaceState(std::unique_ptr<State>(new GameState(this->_data)));
		}
	}
	void GameOver::Draw() {
		_data->window.clear();
		_data->window.draw(gameOverSpr);
		_data->window.display();
	}
}