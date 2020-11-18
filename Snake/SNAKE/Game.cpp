#include "Game.hpp"
#include "GameState.hpp"
#include "SFML/Window.hpp"

namespace SNAKY {
	Game::Game(int width, int height, std::string title) {
		_data->window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);
		sf::Image icon;
		icon.loadFromFile(ICON); // File/Image/Pixel
		unsigned int widthx = 225;
		unsigned int heighty = 225;
		_data->window.setIcon(widthx, heighty, icon.getPixelsPtr());
		_data->machine.ReplaceState(std::unique_ptr<State>((new GameState(this->_data)))); // Here we use 'new' because the parameter asks for a pointer. 'new ...' is a pointer to object.
	}

	void Game::Run() {

		float newTime, timeElapsed;

		newTime = _data->clock.getElapsedTime().asSeconds();

		while (this->_data->window.isOpen()) {
			timeElapsed = _data->clock.getElapsedTime().asSeconds() - newTime;
			if (timeElapsed > 0.03f) { //Controls Speed of Snake
				this->_data->machine.GetActiveState()->Update();
				this->_data->machine.GetActiveState()->Draw();
				timeElapsed = 0;
				newTime = _data->clock.getElapsedTime().asSeconds();
			}
		}
	}
}