#include "Game.hpp"
#include "GameState.hpp"

namespace SNAKY {
	Game::Game(int width, int height, std::string title) {
		_data->window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);
		_data->machine.ReplaceState(StateRef(new GameState(this->_data)));
		this->Run();
	}

	void Game::Run() {

		float newTime, currentTime;

		while (this->_data->window.isOpen()) {

			newTime = this->clock.getElapsedTime().asSeconds();

			this->_data->machine.GetActiveState()->Update(dt);
			this->_data->machine.GetActiveState()->Draw(dt);
		}
	}
}