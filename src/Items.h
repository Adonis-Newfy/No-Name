#pragma once

#include "Entity.h"

class Entity;

class Item
{
public:
	Item() {}

	int itemID = 0;
	int value = 0;
	std::string tag = "MissingNo";

	virtual void use(std::shared_ptr<Entity> entity);
};


class HealthPotion : public Item
{
public:
	HealthPotion() {}

	int itemID = 1;
	int value = 5;
	std::string tag = "Health Potion";

	//Using a HP Potion restores HP to max.
	void use(std::shared_ptr<Entity> entity);
};


class InvulnPotion : public Item
{
public:
	InvulnPotion() {}

	int itemID = 2;
	int value = 5;
	std::string tag = "Invuln Potion";

	//Using an InvulnPotion grants a short spurt of invincibility
	void use(std::shared_ptr<Entity> e);
};

class StrengthPotion : public Item
{
public:
	StrengthPotion() {}

	int itemID = 3;
	int value = 5;
	std::string tag = "Strength Potion";

	//Using a StrengthPotion doubles the players damage for a small amount of time.
	void use(std::shared_ptr<Entity> e);
};