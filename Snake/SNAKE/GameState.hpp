#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game.hpp"

namespace SNAKY {
	class GameState : public State {
	public:
		GameState(GameDataRef data);
		void Update(float dt);
		void Draw(float dt);

	private:
		GameDataRef _data;
		sf::ConvexShape Shape;
		sf::Event event;
		sf::Vector2i pos;
		sf::Sprite restart; 
		int board[BOARD_WIDTH][BOARD_HEIGHT];
	};
}