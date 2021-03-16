#include "Engine/Log.h"
#include "Game/Game.h"

int main()
{
	// Init log
	Log::Init();
	LOG_INFO("Initialized Log");

	// Create and run game
	Game game;
	game.Init();
	game.Run();

	return 0;
}
