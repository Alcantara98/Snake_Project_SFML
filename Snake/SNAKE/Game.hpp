#pragma once

#include <sstream>
#include <memory>
#include <string>
#include "StateMachine.hpp"
#include "DEFINITIONS.hpp"

namespace SNAKY {
	struct GameData {
		StateMachine machine;
		sf::RenderWindow window;
	};

	typedef std::shared_ptr<GameData> GameDataRef;

	class Game {
	public:
		Game(int width, int height, std::string title);

	private:
		const float dt = 1.0f / 60.0f;
		sf::Clock clock;
		GameDataRef _data = std::make_shared<GameData>(); 
		void Run();
	};
}