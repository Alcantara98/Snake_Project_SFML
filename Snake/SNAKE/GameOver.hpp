#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game.hpp"
#include "GameState.hpp"
#include "GameOver.hpp"
#include "StateMachine.hpp"
#include <stdlib.h>

namespace SNAKY {
	class GameOver : public State {
	public:
		GameOver(GameDataRef data);
		void Update();
		void Draw();

	private:
		GameDataRef _data;
		sf::Event event;
		sf::Texture gameOverPic;
		sf::Sprite gameOverSpr;
		bool changeState;
	};
}