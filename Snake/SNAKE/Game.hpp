#pragma once

#include <sstream>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "StateMachine.hpp"
#include "DEFINITIONS.hpp"

namespace SNAKY {
	struct GameData {
		StateMachine machine;
		sf::RenderWindow window;
		sf::Clock clock;
	};

	typedef std::shared_ptr<GameData> GameDataRef;

	class Game {
	public:
		Game(int width, int height, std::string title);
		void Run();

	private:
		const float dt = 1.0f / 60.0f;
		GameDataRef _data = std::make_shared<GameData>(); 
	};
}