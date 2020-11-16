#pragma once
#include "GameState.hpp"

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