#pragma once
#include "vector"
#include "myMath.h"
#include "Resources.h"

enum class Entity_type
{
	Spaceship, Laser, Asteroid, Coin
};

class Entities
{
public:
	Vector2i position;
	int width;
	int height;
	bool dead = false;
	Entity_type ET;
};

class Game
{
public:
	std::vector<Entities> Game_Entity;

	bool isCharged = false;
	bool asteroidCollidedWithLaser = false;
	bool comboTimer = false;
	bool isDead = false;

	int laserChargeTimer = 0;
	int score;
	int spawnRockTimer = 0;
	int coinValues = 50;
	int comboTime = 80;
	int shipSpeed = 10;
	int rockSpeed = 8;
	int coinSpeed = 4;
	int laserSpeed = 10;
	int maxRadius = 5;
	int health = 1;

	void UpdateSpaceshipPosition();
	void SpawnSpaceship();
	void SpawnLasers();
	void SpawnAsteroids();
	void SpawnCoins(Vector2i coinPos);
	void WorkWithAllSpaceshipCollisions();
	void WorkWithLasers_AsteroidsCollisions();
	void MoveAsteroids_Lasers_Coins();
	void DestroyEntity();
	void ResetGame();
	void ManageCombo();
	void Render(Textures& texture);
	void Update();
};