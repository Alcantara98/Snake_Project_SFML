#include <sstream>
#include <iostream>
#include <time.h>

#include "DEFINITIONS.hpp"
#include "GameState.hpp"



namespace SNAKY {
	GameState::GameState(GameDataRef data) : _data(data) {
		/*
		  Setting up initial conditions of board.
		  Where initial food is, and where snake
		  is initially.

		  1 = food
		  2 = snakebody
		  3 = snakehead
		  4 = snaketail
		*/
		for (int i = 0; i < BOARD_WIDTH; i++) {
			for (int j = 0; j < BOARD_HEIGHT; j++) {
				board[i][j] = 0;
			}
		}

		board[16][25] = 3; //Head Initial Position
		int randomDirection = rand() % 4;

		//going down
		if (randomDirection == 0) {
			for (int i = 0; i < 4; i++) {
				board[16][26 + i] = 2;
			}
			board[16][30] = 4;
		}
		//going up
		if (randomDirection == 1) {
			for (int i = 0; i < 4; i++) {
				board[16][24 - i] = 2;
			}
			board[16][20] = 4;
		}
		//going left
		if (randomDirection == 2) {
			for (int i = 0; i < 4; i++) {
				board[17 + i][25] = 2;
			}
			board[21][25] = 4;
		}
		//going right
		if (randomDirection == 1) {
			for (int i = 0; i < 4; i++) {
				board[15 - i][25] = 2;
			}
			board[11][20] = 4;
		}
		
	}

	void GameState::Update(float dt) {
		pos = sf::Mouse::getPosition(this->_data->window);
	}

	void GameState::Draw(float dt) {
		Shape.setPoint((std::size_t)0, sf::Vector2f((float)0, (float)0));
		Shape.setPoint((std::size_t)1, sf::Vector2f((float)TILE_EDGE_LENGTH, (float)0));
		Shape.setPoint((std::size_t)2, sf::Vector2f((float)TILE_EDGE_LENGTH, (float)TILE_EDGE_LENGTH));
		Shape.setPoint((std::size_t)3, sf::Vector2f((float)0, (float)TILE_EDGE_LENGTH));
		for (int i = 1; i <= BOARD_WIDTH; i++) {
			for (int j = 1; j <= BOARD_HEIGHT; j++) {
				Shape.setPosition((float)(i * TILE_EDGE_LENGTH), (float)(j * TILE_EDGE_LENGTH));
				if (board[i][j] != 0) {
					Shape.setFillColor(sf::Color(0, 0, 0, 200));
				}
				else {
					Shape.setFillColor(sf::Color(255, 255, 255, 200));
				}
				_data->window.draw(Shape);
			}
		}
	}
}