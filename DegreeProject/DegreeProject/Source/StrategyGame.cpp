#include <SFML/Graphics.hpp>
#include "Engine/AssetHandler.h"
#include "Engine/Time.h"
#include "Engine/Window.h"
#include "Engine/Log.h"
#include "ECS/EntityManager.h"
#include "Game/MapDrawer.h"
#include "Engine/InputHandler.h"
#include "Game/Systems/PlayerSystem.h"

// For ECS demo
#include "Game/Systems/ECSExampleSystem.h"
#include "Game/Systems/SpriteRenderSystem.h"
#include "Game/Components/MovingCircle.h"
#include <Game/Game.h>

int main()
{
	// Init log
	Log::Init();
	LOG_CORE_INFO("Initialized Log");

	// Create and run game
	Game game;
	game.Init();
	game.Run();

	return 0;
}
