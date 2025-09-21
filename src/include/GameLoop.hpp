#ifndef GAMPLOOP_HPP
#define GAMELOOP_HPP

#include "Plant.hpp"//include 的Head.hpp包含<iostream>
#include "Zombie.hpp"
#include "Map.hpp"
#include "ZombiePool.hpp"
#include "BulletPool.hpp"

#include "Bullet.hpp"
#include "UI.hpp"

void GameLoop();
void Clear();
void Init();
void Load(bool reset = 0);
void Save();


extern sf::Texture Background;
extern sf::Texture Surface;
#endif