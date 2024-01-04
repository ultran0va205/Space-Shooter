#include "Game.h"
#include "Resources.h"

void Game::UpdateSpaceshipPosition()
{
	if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
	{
		Game_Entity[0].position.y -= shipSpeed;
		if (Game_Entity[0].position.y < 0)
		{
			Game_Entity[0].position.y += shipSpeed;
		}
	}
	if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
	{
		Game_Entity[0].position.y += shipSpeed;
		if (Game_Entity[0].position.y + 40 > GetScreenHeight())
		{
			Game_Entity[0].position.y -= shipSpeed;
		}
	}
	if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
	{
		Game_Entity[0].position.x -= shipSpeed;
		if (Game_Entity[0].position.x < 0)
		{
			Game_Entity[0].position.x += shipSpeed;
		}
	}
	if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
	{
		Game_Entity[0].position.x += shipSpeed;
		if (Game_Entity[0].position.x + 40 > GetScreenWidth())
		{
			Game_Entity[0].position.x -= shipSpeed;
		}
	}
}

/*----------------------------------------------------- Spawn Functions --------------------------------------------------------------------*/
void Game::SpawnSpaceship() 
{
	Entities SpaceShip;
	SpaceShip.position = { 400,650 };
	SpaceShip.ET = Entity_type::Spaceship;
	SpaceShip.width = 50;
	SpaceShip.height = 50;
	
	Game_Entity.push_back(SpaceShip);
}

void Game::SpawnAsteroids() //since multiple rocks spawn, using a vector (Game_Entity) seemed to be the easiest way
{
	spawnRockTimer++; //this is for spawning rocks continuously after a certain period of time
	if (spawnRockTimer == 60)
	{
		Entities Rocks;
		Rocks.position = { GetRandomValue(0,700) , 0 }; //rocks spawn from a random x position on the screen, y pos being 0
		Rocks.ET = Entity_type::Asteroid;
		Rocks.width = 40;
		Rocks.height = 40;
		Game_Entity.push_back(Rocks);
		spawnRockTimer = 0;
	}
	if (spawnRockTimer == 25) //more rocks spawn to make the game challenging
	{
		Entities Rocks;
		Rocks.position = { GetRandomValue(0,700) , 0 };
		Rocks.ET = Entity_type::Asteroid;
		Rocks.width = 40;
		Rocks.height = 40;
		Game_Entity.push_back(Rocks);
	}
}

void Game::SpawnLasers()
{	
	if (IsKeyDown(KEY_SPACE)) //for charging the laser
	{
		laserChargeTimer++;
		if (laserChargeTimer == 30)
		{
			isCharged = true;
		}
	}

	if (IsKeyReleased(KEY_SPACE) && isCharged == true) //same as rocks, multiple lasers are required
	{
		Entities Lasers;
		Lasers.position = Game_Entity[0].position;
		Lasers.ET = Entity_type::Laser;
		Lasers.width = 20;
		Lasers.height = 20;
		Game_Entity.push_back(Lasers);
		laserChargeTimer = 0;
		isCharged = false;
	}
}

void Game::SpawnCoins(Vector2i coinPos) 
{
	Entities coin;
	for (int i = 0; i < 5; i++) //spawns a fixed number of coins everytime the player shoots down a rock
	{
		coin.ET = Entity_type::Coin;
		coin.position.x = coinPos.x + GetRandomValue(-20, 80); //makes the coin float around
		coin.position.y = coinPos.y + GetRandomValue(-20, 80);
		coin.height = 20;
		coin.width = 15;
		Game_Entity.push_back(coin);
	}
}

/*------------------------------------------------------- Collision Functions --------------------------------------------------------------------*/
void Game::WorkWithAllSpaceshipCollisions()
{
	for (auto& iterate : Game_Entity)
	{
		switch (iterate.ET)
		{
		case (Entity_type::Asteroid):
			for (auto& iterator : Game_Entity)
			{
				if (iterator.ET == Entity_type::Spaceship)
				{
					if (iterator.position.x <= iterate.position.x + iterate.width && //this is the logic i could come up with for collisions
						iterator.position.x + iterator.width >= iterate.position.x &&
						iterator.position.y <= iterate.position.y + iterate.height &&
						iterator.position.y + iterator.height >= iterate.position.y)
					{
						iterator.dead = true;
						PlaySoundMulti(ResourceManager::sounds.die);
						isDead = true;
					}
				}
			}
			break;

		case(Entity_type::Coin):
			for (auto& iterator : Game_Entity)
			{
				if (iterator.ET == Entity_type::Spaceship)
				{
					if (iterator.position.x <= iterate.position.x + iterate.width &&
						iterator.position.x + iterator.width >= iterate.position.x &&
						iterator.position.y <= iterate.position.y + iterate.height &&
						iterator.position.y + iterator.height >= iterate.position.y)
					{
						PlaySoundMulti(ResourceManager::sounds.coinCollect);
						DrawText(TextFormat("%i", coinValues), iterate.position.x + 40, iterate.position.y - 40, 20 , YELLOW ); //this shows the score beside the spaceship everytime you collect a coin
						if (comboTime > 0)
						{
							coinValues += 10;
						}
						comboTimer = true;
						score += coinValues;
						iterate.dead = true;
					}
				}
			}
		}	
	}
}

void Game::WorkWithLasers_AsteroidsCollisions()
{
	for (auto& iterate : Game_Entity)
	{
		if (iterate.ET == Entity_type::Asteroid)
		{
			for (auto& iteration : Game_Entity)
			{
				if (iteration.ET == Entity_type::Laser)
				{
					if (iteration.position.x <= iterate.position.x + iterate.width &&
						iteration.position.x + iteration.width >= iterate.position.x &&
						iteration.position.y <= iterate.position.y + iterate.height &&
						iteration.position.y + iteration.height >= iterate.position.y)
					{
						iterate.dead = true;
						SpawnCoins(iterate.position);
					}
				}
			}
		}
	}
}

void Game::MoveAsteroids_Lasers_Coins()
{
	for (auto& iterate : Game_Entity)
	{
		switch (iterate.ET)
		{
		case Entity_type::Laser:
			iterate.position.y-= laserSpeed;
			break;
		case Entity_type::Asteroid:
			iterate.position.x++;
			iterate.position.y += rockSpeed;
			break;
		case Entity_type::Coin:
			iterate.position.y += coinSpeed;
			break;
		}
	}
}

/*---------------------------------------------------------------- Other Misc. Functions ------------------------------------------------------------------------*/
void Game::DestroyEntity()
{
	auto last_entity = std::remove_if(Game_Entity.begin(), Game_Entity.end(), [](const Entities& e)->bool {return e.dead; });
	Game_Entity.erase(last_entity, Game_Entity.end()); //this code destroys any entity that's needed to be destroyed from the vector list
}

void Game::ResetGame()
{
	score = 0;
	Game_Entity.clear();
	SpawnSpaceship();
	isDead = false;
}

void Game::ManageCombo()
{
	if (comboTimer == true)
	{
		comboTime--;
	}

	if (comboTime <= 0)
	{
		comboTime = 60;
		comboTimer = false;
		coinValues = 50;
	}
}

void Game::Update()
{
	SpawnAsteroids();
	SpawnLasers();
	UpdateSpaceshipPosition();
	MoveAsteroids_Lasers_Coins();
	WorkWithAllSpaceshipCollisions();
	WorkWithLasers_AsteroidsCollisions();
	DestroyEntity();
	ManageCombo();
}

void Game::Render(Textures &texture)
{
	for (auto& iterate:Game_Entity)
	{
		switch (iterate.ET)
		{
		case Entity_type::Spaceship:
			DrawTexture(texture.spaceShip, iterate.position.x, iterate.position.y, RAYWHITE);
			break;
		case Entity_type::Asteroid:
			DrawTexture(texture.rocks, iterate.position.x, iterate.position.y, RAYWHITE);
			break;
		case Entity_type::Laser:
			DrawTexture(texture.laser, iterate.position.x+10, iterate.position.y, RAYWHITE);
			break;
		case Entity_type::Coin:
			DrawTexture(texture.coin, iterate.position.x, iterate.position.y, RAYWHITE);
		}
	}
}