#pragma once

#include "Entity.h"

class Entity;

class Item
{
public:
	Item(const int& id, const int& val, const std::string& t) : itemID(id), value(val), tag(t) {}

	int itemID;
	int value;
	std::string tag = "MissingNo";

	virtual void use(std::shared_ptr<Entity> entity);
};


class HealthPotion : public Item
{
public:
	HealthPotion(const int& id = 1, const int val = 5, const std::string t = "Health Potion") : Item(id, val, t) {}

	int itemID = 1;
	int value = 5;
	std::string tag = "Health Potion";

	//Using a HP Potion restores HP to max.
	void use(std::shared_ptr<Entity> entity);
};


class InvulnPotion : public Item
{
public:
	InvulnPotion(const int& id = 2, const int val = 10, const std::string t = "Invuln Potion") : Item(id, val, t) {}

	int itemID = 2;
	int value = 10;
	std::string tag = "Invuln Potion";

	//Using an InvulnPotion grants a short spurt of invincibility
	void use(std::shared_ptr<Entity> e);
};

class StrengthPotion : public Item
{
public:
	StrengthPotion(const int& id = 3, const int val = 10, const std::string t = "Strength Potion") : Item(id, val, t) {}

	int itemID = 3;
	int value = 10;
	std::string tag = "Strength Potion";

	//Using a StrengthPotion doubles the players damage for a small amount of time.
	void use(std::shared_ptr<Entity> e);
};