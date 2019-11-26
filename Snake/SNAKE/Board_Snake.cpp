#include "Board_Snake.hpp"

namespace SNAKY {
	void Board_Snake::setNextBodyx(int x) {
		this->nextBodyx = x;
	}

	void Board_Snake::setNextBodyy(int y) {
		this->nextBodyy = y;
	}

	void Board_Snake::setIsSnake(bool isSnake) {
		this->isSnakeBody = isSnake;
	}
	
	void Board_Snake::setIsFood(bool isFood) {
		this->isTileFood = isFood;
	}

	int Board_Snake::getNextBodyx() {
		return this->nextBodyx;
	}

	int Board_Snake::getNextBodyy() {
		return this->nextBodyy;
	}

	bool Board_Snake::isSnake() {
		return this->isSnakeBody;
	}

	bool Board_Snake::isFood() {
		return this->isTileFood;
	}
}