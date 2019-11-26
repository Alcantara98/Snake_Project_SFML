#include <iostream>
#include "Game.hpp"
#include "DEFINITIONS.hpp"
#include <Windows.h>

int main() {
		SNAKY::Game game = SNAKY::Game(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake");
		game.Run();
		return EXIT_SUCCESS;
}
