#pragma once
#include "Game.hpp"
#include "Board_Snake.hpp"
#include "GameOver.hpp"
#include "DEFINITIONS.hpp"

namespace SNAKY {
	class GameState : public State {
	public:
		GameState(GameDataRef data);
		void Update();
		void Draw();
		void MoveSnake();
		void MoveTail();
		void GenerateFood();
		void EatExtend(int lengthExtend);

	private:
		GameDataRef _data;
		sf::ConvexShape Shape[BOARD_WIDTH][BOARD_HEIGHT];
		sf::Event event;
		sf::Vector2i pos;
		int posx, posy;
		int headx[SNAKE_THICKNESS], heady[SNAKE_THICKNESS], tailx[SNAKE_THICKNESS], taily[SNAKE_THICKNESS];
		int newDirection, currentDirection; // 0:up 1:down 2:left 3:right
		int snakeLength;
		int movementWait[4]; // 0:up 1:down 2:left 3:right
		bool gameOver;
		int foodx, foody;
		int foodEatenAmount;
		bool ateSomething;
		int headInitPosx, headInitPosy;
		Board_Snake snake[BOARD_WIDTH][BOARD_HEIGHT];
		sf::Texture backgroundTex;
		sf::Sprite backgroundSpr;
	};
}