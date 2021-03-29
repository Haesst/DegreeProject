#include "Engine/Log.h"
#include "Game/Game.h"

int main()
{
	// Init log
	Log::init();
	LOG_INFO("Initialized Log");

	// Create and run game
	Game game;
	game.init();
	game.run();

	return 0;
}
