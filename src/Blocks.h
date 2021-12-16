#pragma once

#include "Common.h"

class Block
{
public:
	Block(const std::string& tag, const std::string& anim, const bool move, const bool vision) : tag(tag), move(move), vision(vision), animationName(anim) {}

	std::string tag = "MissingNo";
	std::string animationName = "MissingNo";
	bool move = false;
	bool vision = false;
};

class Player : public Block
{
public:
	Player(const std::string& tag = "player", const std::string& anim = "IdleMan", const bool move = true, const bool vision = false) : Block(tag, anim, move, vision) {}

	std::string tag = "MissingNo";
	std::string animationName = "StandDown";
	bool move = false;
	bool vision = false;
};

class Tile : public Block
{
public:
	Tile(const std::string& tag = "tile", const std::string& anim = "Floor1", const bool move = true, const bool vision = true) : Block(tag, anim, move, vision) {}

	std::string tag = "MissingNo";
	std::string animationName = "Bush";
	bool move = false;
	bool vision = false;
};

class Enemy : public Block
{
public:
	Enemy(const std::string& tag = "enemy", const std::string& anim = "Mushroom", const bool move = false, const bool vision = false) : Block(tag, anim, move, vision) {}

	std::string tag = "MissingNo";
	std::string animationName = "Bush";
	bool move = false;
	bool vision = false;
};

class Dec1 : public Block
{
public:
	Dec1(const std::string& tag = "dec", const std::string& anim = "ScaffoldB", const bool move = false, const bool vision = false) : Block(tag, anim, move, vision) {}

	std::string tag = "MissingNo";
	std::string animationName = "Bush";
	bool move = false;
	bool vision = false;
};

class Dec2 : public Block
{
public:
	Dec2(const std::string& tag = "dec", const std::string& anim = "Grass", const bool move = false, const bool vision = true) : Block(tag, anim, move, vision) {}

	std::string tag = "MissingNo";
	std::string animationName = "Bush";
	bool move = false;
	bool vision = false;
};