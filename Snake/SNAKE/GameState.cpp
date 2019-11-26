#include <sstream>
#include <time.h>
#include "DEFINITIONS.hpp"
#include "GameState.hpp"
#include "Board_Snake.hpp"
#include "Game.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <ctime>

namespace SNAKY {
	GameState::GameState(GameDataRef data) : _data(data), headx(), heady(), tailx(), taily(), event(), snake(), movementWait(), headInitPosx(), headInitPosy() {
		headInitPosx = 0;
		headInitPosy = 0;

		posx = 0;
		posy = 0;

		foodx = 0;
		foody = 0;

		foodEatenAmount = 0;

		ateSomething = false;

		newDirection = 0; //Snake Moving up initially 
		currentDirection = 0;

		snakeLength = SNAKE_INITIAL_LENGTH;

		srand(static_cast<unsigned int>(time(NULL)));

		//Finding initial head position
		bool foundInitialHeadY = false;
		bool foundInitialHeadX = false;

		while (foundInitialHeadX == false) {
			headInitPosx = rand() % BOARD_WIDTH;
			if (headInitPosx < BOARD_WIDTH - SNAKE_THICKNESS - 1) {
				foundInitialHeadX = true;
			}
		}
		while (foundInitialHeadY == false) {
			headInitPosy = rand() % BOARD_HEIGHT;
			if (headInitPosy > snakeLength / 2 && headInitPosy < BOARD_HEIGHT - snakeLength - 1) {
				foundInitialHeadY = true;
			}
		}

		gameOver = false;

		for (int i = 0; i < 4; i++) {
			movementWait[i] = 0;
		}

		/*
		  Setting up Background.
		*/
		backgroundTex.loadFromFile(BACKGROUND_PICTURE);
		backgroundSpr.setTexture(backgroundTex);

		sf::Vector2f targetSize(960.0f / 1.0f, 640.0f / 1.0f);
		backgroundSpr.setScale(targetSize.x / backgroundSpr.getLocalBounds().width, targetSize.y / backgroundSpr.getLocalBounds().height);
		backgroundSpr.setPosition(SCREEN_WIDTH / 2 - (backgroundSpr.getGlobalBounds().width / 2), 0);


		/*
		  Setting up initial conditions of board.
		  Where initial food is, and where snake
		  is initially.
		*/
		for (int i = 0; i < BOARD_WIDTH; i++) {
			for (int j = 0; j < BOARD_HEIGHT; j++) {
				Shape[i][j].setPointCount(4);
				snake[i][j].setIsSnake(false);
				snake[i][j].setIsFood(false);
				snake[i][j].setNextBodyx(0);
				snake[i][j].setNextBodyy(0);

			}
		}

		//SNAKE Initial Position, from initial head position, we make tiles down part of snake until snake length 
		//equals SNAKE_INITIAL_LENGTH.
		//Initially pointing up, change this part of code to change initial direction of snake, or add to it to give it random options.
		//Can add left right or down.
		for (int i = 0; i < SNAKE_THICKNESS; i++) {// 0:Left going to right, SNAK_THICKNESS - 1 == Right most head.
			//Head Initial Position
			snake[headInitPosx + i][headInitPosy].setIsSnake(true);
			headx[i] = headInitPosx + i;
			heady[i] = headInitPosy;
			//Body Initial Position
			for (int j = 0; j < SNAKE_INITIAL_LENGTH - 2; j++) {
				snake[headInitPosx + i][headInitPosy + 1 + j].setIsSnake(true);
				this->snake[headInitPosx + i][headInitPosy + 1 + j].setNextBodyx(headInitPosx + i);
				this->snake[headInitPosx + i][headInitPosy + 1 + j].setNextBodyy(headInitPosy + j);
			}
			//Tail Initial Position
			snake[headInitPosx + i][headInitPosy + SNAKE_INITIAL_LENGTH - 1].setIsSnake(false);
			this->snake[headInitPosx + i][headInitPosy + SNAKE_INITIAL_LENGTH - 1].setNextBodyx(headInitPosx + i);
			this->snake[headInitPosx + i][headInitPosy + SNAKE_INITIAL_LENGTH - 1].setNextBodyy(headInitPosy + SNAKE_INITIAL_LENGTH - 2);
			tailx[i] = headInitPosx + i;
			taily[i] = headInitPosy + SNAKE_INITIAL_LENGTH - 1;
		}

		//Initialize tiles
		for (int i = 0; i < BOARD_WIDTH; i++) {
			for (int j = 0; j < BOARD_HEIGHT; j++) {
				Shape[i][j].setPoint((std::size_t)0, sf::Vector2f((float)0, (float)0));
				Shape[i][j].setPoint((std::size_t)1, sf::Vector2f((float)TILE_EDGE_LENGTH, (float)0));
				Shape[i][j].setPoint((std::size_t)2, sf::Vector2f((float)TILE_EDGE_LENGTH, (float)TILE_EDGE_LENGTH));
				Shape[i][j].setPoint((std::size_t)3, sf::Vector2f((float)0, (float)TILE_EDGE_LENGTH));
				Shape[i][j].setPosition((float)(i * TILE_EDGE_LENGTH), (float)(j * TILE_EDGE_LENGTH));
			}
		}

		//GenerateFood
		GenerateFood();
	}
	void GameState::MoveSnake() {
		//Head Movement Section
		//Move up
		if (newDirection == 0) {//Move up
			//initial going up
			if (currentDirection == 0) {
				for (int i = 0; i < SNAKE_THICKNESS; i++) {
					//Redirecting Array Chain so when becomes tail, flows naturally.
					this->snake[headx[i]][heady[i]].setNextBodyx(headx[i]);
					this->snake[headx[i]][heady[i]].setNextBodyy(heady[i] - 1);
					//Moving head at designated direction.
					heady[i]--;
					//If Snake already, game over.
					if (snake[headx[i]][heady[i]].isSnake() == true) {
						gameOver = true;
					}
					//Else make new head Snake.
					snake[headx[i]][heady[i]].setIsSnake(true);
					//If new head is food, has to grow.
					if (snake[headx[i]][heady[i]].isFood() == true) {
						ateSomething = true;
					}
				}
				MoveTail();
			}

			//initially going left
			else if (currentDirection == 2) {
				for (int i = 0; i < SNAKE_THICKNESS; i++) {
					//Redirecting Array Chain so when becomes tail, flows naturally.
					this->snake[headx[i] + SNAKE_THICKNESS][heady[i]].setNextBodyx(headx[i] + i);
					this->snake[headx[i] + SNAKE_THICKNESS][heady[i]].setNextBodyy(heady[i] + i);

					for (int j = 0; j < SNAKE_THICKNESS; j++) {
						this->snake[headx[i] + (i)][heady[i] + (i) - j].setNextBodyx(headx[i] + (i));
						this->snake[headx[i] + (i)][heady[i] + (i) - j].setNextBodyy(heady[i] + (i) - 1 - j);
					}
					//Moving head at designated direction.
					headx[i] = headx[i] + i;
					heady[i] = heady[i] - SNAKE_THICKNESS + i;
					//If Snake already, game over.
					if (snake[headx[i]][heady[i]].isSnake() == true) {
						gameOver = true;
					}
					//Else make new head Snake.
					snake[headx[i]][heady[i]].setIsSnake(true);
					//If new head is food, has to grow.
					if (snake[headx[i]][heady[i]].isFood() == true) {
						ateSomething = true;
					}
				}
				//Ensure tail gets allocated correctly if width length is equal or less than thickness.
				if (snakeLength <= SNAKE_THICKNESS) {
					for (int i = 0; i < SNAKE_THICKNESS; i++) {
						tailx[i] = headx[i];
						taily[i] = heady[i] + snakeLength - 1;
						snake[tailx[i]][taily[i]].setIsSnake(false);
					}
					for (int i = 0; i < snakeLength; i++) {
						for (int j = 0; j < SNAKE_THICKNESS - snakeLength + 1; j++) {
							snake[headx[i]][heady[i] + snakeLength + j].setIsSnake(false);
						}
					}
				}
				else { MoveTail(); }

				movementWait[3] = SNAKE_THICKNESS;
			}

			//initially going right
			else if (currentDirection == 3) {
				for (int i = 0; i < SNAKE_THICKNESS; i++) {
					//Redirecting Array Chain so when becomes tail, flows naturally.
					this->snake[headx[i] - SNAKE_THICKNESS][heady[i]].setNextBodyx(headx[i] - SNAKE_THICKNESS + 1 + i);
					this->snake[headx[i] - SNAKE_THICKNESS][heady[i]].setNextBodyy(heady[i] + SNAKE_THICKNESS - 1 - i);

					for (int j = 0; j < SNAKE_THICKNESS; j++) {
						this->snake[headx[i] + (-SNAKE_THICKNESS + 1 + i)][heady[i] + (SNAKE_THICKNESS - 1 - i) - j].setNextBodyx(headx[i] + (-SNAKE_THICKNESS + 1 + i));
						this->snake[headx[i] + (-SNAKE_THICKNESS + 1 + i)][heady[i] + (SNAKE_THICKNESS - 1 - i) - j].setNextBodyy(heady[i] + (SNAKE_THICKNESS - 1 - i) - 1 - j);
					}
					//Moving head at designated direction.
					headx[i] = headx[i] - SNAKE_THICKNESS + 1 + i;
					heady[i] = heady[i] - 1 - i;
					//If Snake already, game over.
					if (snake[headx[i]][heady[i]].isSnake() == true) {
						gameOver = true;
					}
					//Else make new head Snake.
					snake[headx[i]][heady[i]].setIsSnake(true);
					//If new head is food, has to grow.
					if (snake[headx[i]][heady[i]].isFood() == true) {
						ateSomething = true;
					}
				}
				//Ensure tail gets allocated correctly if width length is equal or less than thickness.
				if (snakeLength <= SNAKE_THICKNESS) {
					for (int i = 0; i < SNAKE_THICKNESS; i++) {
						tailx[i] = headx[i];
						taily[i] = heady[i] + snakeLength - 1;
						snake[tailx[i]][taily[i]].setIsSnake(false);
					}
					for (int i = 0; i < snakeLength; i++) {
						for (int j = 0; j < SNAKE_THICKNESS - snakeLength + 1; j++) {
							snake[headx[0] + SNAKE_THICKNESS - 1 - i][heady[i] + snakeLength + j].setIsSnake(false);
						}
					}
				}
				else { MoveTail(); }

				movementWait[2] = SNAKE_THICKNESS;
			}
			currentDirection = 0;
		}

		//Move down
		else if (newDirection == 1) {
			//initially going down
			if (currentDirection == 1) {//initially going down
				for (int i = 0; i < SNAKE_THICKNESS; i++) {
					//Redirecting Array Chain so when becomes tail, flows naturally.
					this->snake[headx[i]][heady[i]].setNextBodyx(headx[i]);
					this->snake[headx[i]][heady[i]].setNextBodyy(heady[i] + 1);
					//Moving head at designated direction.
					heady[i]++;
					//If Snake already, game over.
					if (snake[headx[i]][heady[i]].isSnake() == true) {
						gameOver = true;
					}
					//Else make new head Snake.
					snake[headx[i]][heady[i]].setIsSnake(true);
					//If new head is food, has to grow.
					if (snake[headx[i]][heady[i]].isFood() == true) {
						ateSomething = true;
					}
				}
				MoveTail();
			}

			//initially going left
			else if (currentDirection == 2) {//initially going left
				for (int i = 0; i < SNAKE_THICKNESS; i++) {
					//Redirecting Array Chain so when becomes tail, flows naturally.
					snake[headx[i] + SNAKE_THICKNESS][heady[i]].setNextBodyx(headx[i] + SNAKE_THICKNESS - 1 - i);
					snake[headx[i] + SNAKE_THICKNESS][heady[i]].setNextBodyy(heady[i] - SNAKE_THICKNESS + 1 + i);

					for (int j = 0; j < SNAKE_THICKNESS; j++) {
						snake[headx[i] + (SNAKE_THICKNESS - 1 - i)][heady[i] + (-SNAKE_THICKNESS + 1 + i) + j].setNextBodyx(headx[i] + (SNAKE_THICKNESS - 1 - i));
						snake[headx[i] + (SNAKE_THICKNESS - 1 - i)][heady[i] + (-SNAKE_THICKNESS + 1 + i) + j].setNextBodyy(heady[i] + (-SNAKE_THICKNESS + 1 + i) + 1 + j);
					}
					//Moving head at designated direction.
					headx[i] = headx[i] + SNAKE_THICKNESS - 1 - i;
					heady[i] = heady[i] + 1 + i;
					//If Snake already, game over.
					if (snake[headx[i]][heady[i]].isSnake() == true) {
						gameOver = true;
					}
					//Else make new head Snake.
					snake[headx[i]][heady[i]].setIsSnake(true);
					//If new head is food, has to grow.
					if (snake[headx[i]][heady[i]].isFood() == true) {
						ateSomething = true;
					}
				}
				//Ensure tail gets allocated correctly if width length is equal or less than thickness.
				if (snakeLength <= SNAKE_THICKNESS) {
					for (int i = 0; i < SNAKE_THICKNESS; i++) {
						tailx[i] = headx[i];
						taily[i] = heady[i] - snakeLength + 1;
						snake[tailx[i]][taily[i]].setIsSnake(false);
					}
					for (int i = 0; i < snakeLength; i++) {
						for (int j = 0; j < SNAKE_THICKNESS - snakeLength + 1; j++) {
							snake[headx[0] - SNAKE_THICKNESS + 1 + i][heady[i] - snakeLength - j].setIsSnake(false);
						}
					}
				}
				else { MoveTail(); }

				movementWait[3] = SNAKE_THICKNESS;
			}

			//initially going right
			else if (currentDirection == 3) {
				for (int i = 0; i < SNAKE_THICKNESS; i++) {
					//Redirecting Array Chain so when becomes tail, flows naturally.
					snake[headx[i] - SNAKE_THICKNESS][heady[i]].setNextBodyx(headx[i] - i);
					snake[headx[i] - SNAKE_THICKNESS][heady[i]].setNextBodyy(heady[i] - i);

					for (int j = 0; j < SNAKE_THICKNESS; j++) {
						snake[headx[i] + (-i)][heady[i] + (-i) + j].setNextBodyx(headx[i] + (-i));
						snake[headx[i] + (-i)][heady[i] + (-i) + j].setNextBodyy(heady[i] + (-i) + 1 + j);
					}
					//Moving head at designated direction.
					headx[i] = headx[i] - i;
					heady[i] = heady[i] + SNAKE_THICKNESS - i;
					//If Snake already, game over.
					if (snake[headx[i]][heady[i]].isSnake() == true) {
						gameOver = true;
					}
					//Else make new head Snake.
					snake[headx[i]][heady[i]].setIsSnake(true);
					//If new head is food, has to grow.
					if (snake[headx[i]][heady[i]].isFood() == true) {
						ateSomething = true;
					}
				}
				//Ensure tail gets allocated correctly if width length is equal or less than thickness.
				if (snakeLength <= SNAKE_THICKNESS) {
					for (int i = 0; i < SNAKE_THICKNESS; i++) {
						tailx[i] = headx[i];
						taily[i] = heady[i] - snakeLength + 1;
						snake[tailx[i]][taily[i]].setIsSnake(false);
					}
					for (int i = 0; i < snakeLength; i++) {
						for (int j = 0; j < SNAKE_THICKNESS - snakeLength + 1; j++) {
							snake[headx[i]][heady[i] - snakeLength - j].setIsSnake(false);
						}
					}
				}
				else { MoveTail(); }

				movementWait[2] = SNAKE_THICKNESS;
			}
			currentDirection = 1;
		}

		//Move left
		else if (newDirection == 2) {
			//initially going left
			if (currentDirection == 2) {//initially going left
				for (int i = 0; i < SNAKE_THICKNESS; i++) {
					//Redirecting Array Chain so when becomes tail, flows naturally.
					this->snake[headx[i]][heady[i]].setNextBodyx(headx[i] - 1);
					this->snake[headx[i]][heady[i]].setNextBodyy(heady[i]);
					//Moving head at designated direction.
					headx[i]--;
					//If Snake already, game over.
					if (snake[headx[i]][heady[i]].isSnake() == true) {
						gameOver = true;
					}
					//Else make new head Snake.
					snake[headx[i]][heady[i]].setIsSnake(true);
					//If new head is food, has to grow.
					if (snake[headx[i]][heady[i]].isFood() == true) {
						ateSomething = true;
					}
				}
				MoveTail();
			}

			//initially going up
			else if (currentDirection == 0) {
				for (int i = 0; i < SNAKE_THICKNESS; i++) {
					//Redirecting Array Chain so when becomes tail, flows naturally.
					snake[headx[i]][heady[i] + SNAKE_THICKNESS].setNextBodyx(headx[i] + SNAKE_THICKNESS - 1 - i);
					snake[headx[i]][heady[i] + SNAKE_THICKNESS].setNextBodyy(heady[i] + SNAKE_THICKNESS - 1 - i);

					for (int j = 0; j < SNAKE_THICKNESS; j++) {
						snake[headx[i] + (SNAKE_THICKNESS - 1 - i) - j][heady[i] + (SNAKE_THICKNESS - 1 - i)].setNextBodyx(headx[i] + (SNAKE_THICKNESS - 1 - i) - j - 1);
						snake[headx[i] + (SNAKE_THICKNESS - 1 - i) - j][heady[i] + (SNAKE_THICKNESS - 1 - i)].setNextBodyy(heady[i] + (SNAKE_THICKNESS - 1 - i));
					}
					//Moving head at designated direction.
					headx[i] = headx[i] - 1 - i;
					heady[i] = heady[i] + SNAKE_THICKNESS - 1 - i;
					//If Snake already, game over.
					if (snake[headx[i]][heady[i]].isSnake() == true) {
						gameOver = true;
					}
					//Else make new head Snake.
					snake[headx[i]][heady[i]].setIsSnake(true);
					//If new head is food, has to grow.
					if (snake[headx[i]][heady[i]].isFood() == true) {
						ateSomething = true;
					}
				}
				//Ensure tail gets allocated correctly if width length is equal or less than thickness.
				if (snakeLength <= SNAKE_THICKNESS) {
					for (int i = 0; i < SNAKE_THICKNESS; i++) {
						tailx[i] = headx[i] + snakeLength - 1;
						taily[i] = heady[i];
						snake[tailx[i]][taily[i]].setIsSnake(false);
					}
					for (int i = 0; i < snakeLength; i++) {
						for (int j = 0; j < SNAKE_THICKNESS - snakeLength + 1; j++) {
							snake[headx[i] + snakeLength + j][heady[SNAKE_THICKNESS - 1 - i]].setIsSnake(false);
						}
					}
				}
				else { MoveTail(); }

				movementWait[1] = SNAKE_THICKNESS;
			}

			//initially going down
			else if (currentDirection == 1) {
				for (int i = 0; i < SNAKE_THICKNESS; i++) {
					//Redirecting Array Chain so when becomes tail, flows naturally.
					snake[headx[i]][heady[i] - SNAKE_THICKNESS].setNextBodyx(headx[i] + i);
					snake[headx[i]][heady[i] - SNAKE_THICKNESS].setNextBodyy(heady[i] - i);

					for (int j = 0; j < SNAKE_THICKNESS; j++) {
						snake[headx[i] + (i) - j][heady[i] + (-i)].setNextBodyx(headx[i] + (i) - j - 1);
						snake[headx[i] + (i) - j][heady[i] + (-i)].setNextBodyy(heady[i] + (-i));
					}
					//Moving head at designated direction.
					headx[i] = headx[i] - SNAKE_THICKNESS + i;
					heady[i] = heady[i] - i;
					//If Snake already, game over.
					if (snake[headx[i]][heady[i]].isSnake() == true) {
						gameOver = true;
					}
					//Else make new head Snake.
					snake[headx[i]][heady[i]].setIsSnake(true);
					//If new head is food, has to grow.
					if (snake[headx[i]][heady[i]].isFood() == true) {
						ateSomething = true;
					}
				}
				//Ensure tail gets allocated correctly if width length is equal or less than thickness.
				if (snakeLength <= SNAKE_THICKNESS) {
					for (int i = 0; i < SNAKE_THICKNESS; i++) {
						tailx[i] = headx[i] + snakeLength - 1;
						taily[i] = heady[i];
						snake[tailx[i]][taily[i]].setIsSnake(false);
					}
					for (int i = 0; i < snakeLength; i++) {
						for (int j = 0; j < SNAKE_THICKNESS - snakeLength + 1; j++) {
							snake[headx[i] + snakeLength + j][heady[i]].setIsSnake(false);
						}
					}
				}
				else { MoveTail(); }

				movementWait[0] = SNAKE_THICKNESS;
			}
			currentDirection = 2;
		}

		//Move right
		else if (newDirection == 3) {
			//initially going right
			if (currentDirection == 3) {//initially going right
				for (int i = 0; i < SNAKE_THICKNESS; i++) {
					//Redirecting Array Chain so when becomes tail, flows naturally.
					snake[headx[i]][heady[i]].setNextBodyx(headx[i] + 1);
					snake[headx[i]][heady[i]].setNextBodyy(heady[i]);
					//Moving head at designated direction.
					headx[i]++;
					//If Snake already, game over.
					if (snake[headx[i]][heady[i]].isSnake() == true) {
						gameOver = true;
					}
					//Else make new head Snake.
					snake[headx[i]][heady[i]].setIsSnake(true);
					//If new head is food, has to grow.
					if (snake[headx[i]][heady[i]].isFood() == true) {
						ateSomething = true;
					}
				}
				MoveTail();
			}

			//initially going up
			else if (currentDirection == 0) {
				for (int i = 0; i < SNAKE_THICKNESS; i++) {
					//Redirecting Array Chain so when becomes tail, flows naturally.
					snake[headx[i]][heady[i] + SNAKE_THICKNESS].setNextBodyx(headx[i] - i);
					snake[headx[i]][heady[i] + SNAKE_THICKNESS].setNextBodyy(heady[i] + i);

					for (int j = 0; j < SNAKE_THICKNESS; j++) {
						snake[headx[i] + (-i) + j][heady[i] + (i)].setNextBodyx(headx[i] + (-i) + j + 1);
						snake[headx[i] + (-i) + j][heady[i] + (i)].setNextBodyy(heady[i] + (i));
					}
					//Moving head at designated direction.
					headx[i] = headx[i] + SNAKE_THICKNESS - i;
					heady[i] = heady[i] + i;
					//If Snake already, game over.
					if (snake[headx[i]][heady[i]].isSnake() == true) {
						gameOver = true;
					}
					//Else make new head Snake.
					snake[headx[i]][heady[i]].setIsSnake(true);
					//If new head is food, has to grow.
					if (snake[headx[i]][heady[i]].isFood() == true) {
						ateSomething = true;
					}
				}
				//Ensure tail gets allocated correctly if width length is equal or less than thickness.
				if (snakeLength <= SNAKE_THICKNESS) {
					for (int i = 0; i < SNAKE_THICKNESS; i++) {
						tailx[i] = headx[i] - snakeLength + 1;
						taily[i] = heady[i];
						snake[tailx[i]][taily[i]].setIsSnake(false);
					}
					for (int i = 0; i < snakeLength; i++) {
						for (int j = 0; j < SNAKE_THICKNESS - snakeLength + 1; j++) {
							snake[headx[i] - snakeLength - j][heady[i]].setIsSnake(false);
						}
					}
				}
				else { MoveTail(); }

				movementWait[1] = SNAKE_THICKNESS;
			}

			//initially going down
			else if (currentDirection == 1) {
				for (int i = 0; i < SNAKE_THICKNESS; i++) {
					//Redirecting Array Chain so when becomes tail, flows naturally.
					this->snake[headx[i]][heady[i] - SNAKE_THICKNESS].setNextBodyx(headx[i] - SNAKE_THICKNESS + 1 + i);
					this->snake[headx[i]][heady[i] - SNAKE_THICKNESS].setNextBodyy(heady[i] - SNAKE_THICKNESS + 1 + i);

					for (int j = 0; j < SNAKE_THICKNESS; j++) {
						this->snake[headx[i] + (-SNAKE_THICKNESS + 1 + i) + j][heady[i] + (-SNAKE_THICKNESS + 1 + i)].setNextBodyx(headx[i] + (-SNAKE_THICKNESS + 1 + i) + j + 1);
						this->snake[headx[i] + (-SNAKE_THICKNESS + 1 + i) + j][heady[i] + (-SNAKE_THICKNESS + 1 + i)].setNextBodyy(heady[i] + (-SNAKE_THICKNESS + 1 + i));
					}
					//Moving head at designated direction.
					headx[i] = headx[i] + 1 + i;
					heady[i] = heady[i] - SNAKE_THICKNESS + 1 + i;
					//If Snake already, game over.
					if (snake[headx[i]][heady[i]].isSnake() == true) {
						gameOver = true;
					}
					//Else make new head Snake.
					snake[headx[i]][heady[i]].setIsSnake(true);
					//If new head is food, has to grow.
					if (snake[headx[i]][heady[i]].isFood() == true) {
						ateSomething = true;
					}
				}
				//Ensure tail gets allocated correctly if width length is equal or less than thickness.
				if (snakeLength <= SNAKE_THICKNESS) {
					for (int i = 0; i < SNAKE_THICKNESS; i++) {
						tailx[i] = headx[i] - snakeLength + 1;
						taily[i] = heady[i];
						snake[tailx[i]][taily[i]].setIsSnake(false);
					}
					for (int i = 0; i < snakeLength; i++) {
						for (int j = 0; j < SNAKE_THICKNESS - snakeLength + 1; j++) {
							snake[headx[i] - SNAKE_THICKNESS - j][heady[0] + SNAKE_THICKNESS - 1 - i].setIsSnake(false);
						}
					}
				}
				else { MoveTail(); }

				movementWait[0] = SNAKE_THICKNESS;
			}
			currentDirection = 3;
		}
		if (ateSomething == true) {
			foodEatenAmount = foodEatenAmount + 2;
			//Make current food disappear
			for (int i = foodx; i < foodx + FOOD_SIZE; i++) {
				for (int j = foody; j < foody + FOOD_SIZE; j++) {
					snake[i][j].setIsFood(false);
				}
			}

			GenerateFood();
			ateSomething = false;
		}
		if (foodEatenAmount > 0) {
			EatExtend(1);
			foodEatenAmount--;
		}
		if (headx[0] < 0 || heady[0] < 0 || headx[0] > BOARD_WIDTH - 1 || heady[0] > BOARD_HEIGHT - 1) {
			gameOver = true;
		}
	}

	//Standard way of moving the tail. Used when snake is longer than width, or is moving straight.
	void GameState::MoveTail() {
		for (int i = 0; i < SNAKE_THICKNESS; i++) {
			//Tail Movement Section
			int new_Tailx = this->snake[tailx[i]][taily[i]].getNextBodyx();
			int new_Taily = this->snake[tailx[i]][taily[i]].getNextBodyy();

			this->snake[tailx[i]][taily[i]].setNextBodyx(0);
			this->snake[tailx[i]][taily[i]].setNextBodyy(0);

			tailx[i] = new_Tailx;
			taily[i] = new_Taily;

			snake[tailx[i]][taily[i]].setIsSnake(false);
		}
	}

	void GameState::GenerateFood() {
		bool foodGenerated = false;
		while (foodGenerated == false) {
			int x = (rand() % BOARD_WIDTH);
			int y = (rand() % BOARD_HEIGHT);

			if (x < 2) {
				x = 2;
			}
			if (y < 2) {
				y = 2;
			}
			if (y > BOARD_HEIGHT - 4) {
				y = BOARD_HEIGHT - 4;
			}
			if (x > BOARD_WIDTH - 4) {
				x = BOARD_WIDTH - 4;
			}

			if (snake[x][y].isSnake() == false) {
				if (snake[x + 1][y].isSnake() == false) {
					if (snake[x][y + 1].isSnake() == false) {
						if (snake[x + 1][y + 1].isSnake() == false) {
							for (int i = x; i < x + FOOD_SIZE; i++) {
								for (int j = y; j < y + FOOD_SIZE; j++) {
									snake[i][j].setIsFood(true);
								}
							}
							foodx = x;
							foody = y;
							foodGenerated = true;
						}
					}
				}
			}
		}
	}

	void GameState::EatExtend(int lengthExtend) {
		int tailDirection = 0; // 0:up 1:down 2:left 3:right

		if (snake[tailx[0] + 1][taily[0]].isSnake() == true) {
			tailDirection = 3;
		}
		else if (snake[tailx[0] - 1][taily[0]].isSnake() == true) {
			tailDirection = 2;
		}
		else if (snake[tailx[0]][taily[0] + 1].isSnake() == true) {
			tailDirection = 1;
		}
		else if (snake[tailx[0]][taily[0] - 1].isSnake() == true) {
			tailDirection = 0;
		}
		else if (snakeLength == 1) {
			tailDirection = currentDirection;
		}

		if (tailDirection == 0) {//going up, extend down
			for (int i = 0; i < SNAKE_THICKNESS; i++) {
				for (int j = 0; j < lengthExtend; j++) {
					snake[tailx[i]][taily[i]].setIsSnake(true);
					taily[i]++;
					snake[tailx[i]][taily[i]].setNextBodyx(tailx[i]);
					snake[tailx[i]][taily[i]].setNextBodyy(taily[i] - 1);
				}
			}
		}
		else if (tailDirection == 1) {//going down, extend up
			for (int i = 0; i < SNAKE_THICKNESS; i++) {
				for (int j = 0; j < lengthExtend; j++) {
					snake[tailx[i]][taily[i]].setIsSnake(true);
					taily[i]--;
					snake[tailx[i]][taily[i]].setNextBodyx(tailx[i]);
					snake[tailx[i]][taily[i]].setNextBodyy(taily[i] + 1);
				}
			}
		}
		else if (tailDirection == 2) {//going left, extend right
			for (int i = 0; i < SNAKE_THICKNESS; i++) {
				for (int j = 0; j < lengthExtend; j++) {
					snake[tailx[i]][taily[i]].setIsSnake(true);
					tailx[i]++;
					snake[tailx[i]][taily[i]].setNextBodyx(tailx[i] - 1);
					snake[tailx[i]][taily[i]].setNextBodyy(taily[i]);
				}
			}
		}
		else if (tailDirection == 3) {//going right, extend left
			for (int i = 0; i < SNAKE_THICKNESS; i++) {
				for (int j = 0; j < lengthExtend; j++) {
					snake[tailx[i]][taily[i]].setIsSnake(true);
					tailx[i]--;
					snake[tailx[i]][taily[i]].setNextBodyx(tailx[i] + 1);
					snake[tailx[i]][taily[i]].setNextBodyy(taily[i]);
				}
			}
		}
		snakeLength = snakeLength + lengthExtend;
	}

	void GameState::Update() {
		pos = sf::Mouse::getPosition(this->_data->window);
		posx = pos.x / TILE_EDGE_LENGTH;
		posy = pos.y / TILE_EDGE_LENGTH;

		while (_data->window.pollEvent(event)) {
			if (event.type ==  sf::Event::Closed) {//If close button clicked, closes window.
				_data->window.close();
			}

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::W && currentDirection != 1 && movementWait[0] == 0) {
					newDirection = 0; //Go up
				}
				if (event.key.code == sf::Keyboard::S && currentDirection != 0 && movementWait[1] == 0) {
					newDirection = 1; //Go down
				}
				if (event.key.code == sf::Keyboard::A && currentDirection != 3 && movementWait[2] == 0) {
					newDirection = 2; //Go left
				}
				if (event.key.code == sf::Keyboard::D && currentDirection != 2 && movementWait[3] == 0) {
					newDirection = 3; //Go right
				}
			}
		}

		this->MoveSnake();
		//Ensure we don't turn while still turning else we hit snake body.
		for (int i = 0; i < 4; i++) {
			if (snakeLength <= movementWait[i] + 1) {
				movementWait[i] = 0;
			}
			if (movementWait[i] != 0) {
				movementWait[i]--;
			}
		}

		//changeState becomes true if new head was already snake, hence gameover.
		if (gameOver == true) {
			_data->machine.ReplaceState(StateRef(new GameOver(this->_data)));
		}
	}

	void GameState::Draw() {
		_data->window.clear();
		
		//Draw Background
		_data->window.draw(backgroundSpr);

		for (int i = 0; i < SNAKE_THICKNESS; i++) {
			int currentx = tailx[i];
			int currenty = taily[i];
			for (int j = 0; j < snakeLength; j++) {
				int x; //For changing currentx
				int y; //For changing currenty
				if (j == 0) {
					Shape[currentx][currenty].setFillColor(sf::Color(0, 0, 250, 200));
				}
				else if (j == snakeLength - 1) {
					Shape[currentx][currenty].setFillColor(sf::Color(250, 0, 0, 200));
				}
				else {
					Shape[currentx][currenty].setFillColor(sf::Color(100, 250, 0, 130));
				}
				_data->window.draw(Shape[currentx][currenty]);
				x = this->snake[currentx][currenty].getNextBodyx();
				y = this->snake[currentx][currenty].getNextBodyy();
				currentx = x;
				currenty = y;
			}
		}

		for (int i = foodx; i < foodx + FOOD_SIZE; i++) {
			for (int j = foody; j < foody + FOOD_SIZE; j++) {
				Shape[i][j].setFillColor(sf::Color(100, 100, 250, 200));
				_data->window.draw(Shape[i][j]);
			}
		}

		/*for (int i = 0; i < SNAKE_THICKNESS; i++) {
			for (int j = 0; j < BOARD_WIDTH; j++) {
				for (int k = 0; k < BOARD_HEIGHT; k++) {
					if (snake[j][k].isSnake() == true) {
						Shape[j][k].setFillColor(sf::Color(0, 200, 0, 200));
						_data->window.draw(Shape[j][k]);
					}
				}
			}
		}*/
		_data->window.display();
	}
}