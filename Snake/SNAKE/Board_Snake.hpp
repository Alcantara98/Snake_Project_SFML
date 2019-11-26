#pragma once
#include <SFML/Graphics.hpp>

namespace SNAKY {
	class Board_Snake {
	public:
		void setNextBodyx(int x);
		void setNextBodyy(int y);
		void setIsSnake(bool isSnake);
		void setIsFood(bool isFood);

		int getNextBodyx();
		int getNextBodyy();
		bool isSnake();
		bool isFood();
	private:
		int nextBodyx, nextBodyy;
		bool isSnakeBody, isTileFood;
	};
}