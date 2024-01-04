#pragma once
#include "raylib.h"

struct Textures
{
	Texture2D spaceShip;
	Texture2D rocks;
	Texture2D coin;
	Texture2D laser;
};

struct Sounds
{
	Sound coinCollect;
	Sound die;
};

class ResourceManager
{
public:
	static Textures textures;
	static Sounds sounds;
	static void LoadResources();
	static void UnloadResources(); //to prevent memory leak
};