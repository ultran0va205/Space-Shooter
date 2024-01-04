#include "Resources.h"
Textures ResourceManager::textures;
Sounds ResourceManager::sounds;

void ResourceManager::LoadResources()
{
	textures.rocks = LoadTexture("./Assets/rock.png");
	textures.spaceShip = LoadTexture("./Assets/spaceship.png");
	textures.coin = LoadTexture("./Assets/coin.png");
	textures.laser = LoadTexture("./Assets/laser.png");
	sounds.coinCollect = LoadSound("./Assets/win.wav");
	sounds.die = LoadSound("./Assets/die.wav");
}

void ResourceManager::UnloadResources()
{
	UnloadTexture(textures.rocks);
	UnloadTexture(textures.spaceShip);
	UnloadTexture(textures.coin);
	UnloadTexture(textures.laser);
	UnloadSound(sounds.coinCollect);
}