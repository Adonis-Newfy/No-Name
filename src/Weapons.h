#pragma once

#include "Entity.h"

class Weapon
{
public:
	Weapon(const int& id, const std::string& t, const std::string& a, const int &life, const int &d, const int& l, const bool& r) 
		: weaponID(id), tag(t), animationName(a), lifespan(life), damage(d), delay(l), ranged(r) {}

	//Basic Information
	int weaponID;
	std::string tag = "MissingNo";
	
	//Entity Components
	std::string animationName;					// Animation the weapon should use.
	int lifespan;								// Time the weapon should stay on screen.

	//Functional Components
	int damage;									// Amount of base damage the weapon should use.
	int delay;
	bool ranged;
};

class Basic : public Weapon
{
public:
	Basic(const int& id = 1, const std::string& t = "Basic", const std::string& a = "SwordRight", const int& life = 10, const int& d = 1, const int& l = 0, const bool& r = false) 
		: Weapon(id, t, a, life, d, l, r) {}

	//Basic Information
	int weaponID = 1;
	std::string tag = "Basic";

	//Entity Components
	std::string animationName = "SwordRight";   // Animation the weapon should use.
	int lifespan = 10;								// Time the weapon should stay on screen.

	//Functional Components
	int damage = 1;								// Amount of base damage the weapon should use.
	int delay = 0;
	bool ranged = false;
};

class Warrior : public Weapon
{
public:
	Warrior(const int& id = 2, const std::string& t = "Warrior", const std::string& a = "RockTR", const int& life = 10, const int& d = 2, const int& l = 60, const bool& r = false) 
		: Weapon(id, t, a, life, d, l, r) {}

	//Basic Information
	int weaponID = 2;
	std::string tag = "Warrior";

	//Entity Components
	std::string animationName = "WarriorWeaponRight";   // Animation the weapon should use.
	int lifespan = 10;								    // Time the weapon should stay on screen.

	//Functional Components
	int damage = 2;										// Amount of base damage the weapon should use.
	int delay = 60;
	bool ranged = false;
};

class Ranger : public Weapon
{
public:
	Ranger(const int& id = 3, const std::string& t = "Ranger", const std::string& a = "TreeTL", const int& life = 10, const int& d = 1, const int& l = 0, const bool& r = true) 
		: Weapon(id, t, a, life, d, l, r) {}

	//Basic Information
	int weaponID = 3;
	std::string tag = "Ranger";

	//Entity Components
	std::string animationName = "RangerWeaponRight";   // Animation the weapon should use.
	int lifespan = 10;								    // Time the weapon should stay on screen.

	//Functional Components
	int damage = 1;										// Amount of base damage the weapon should use.
	int delay = 0;
	bool ranged = true;
};

class Mage : public Weapon
{
public:
	Mage(const int& id = 4, const std::string& t = "Mage", const std::string& a = "Black", const int& life = 10, const int& d = 3, const int& l = 120, const bool& r = true) 
		: Weapon(id, t, a, life, d, l, r) {}

	//Basic Information
	int weaponID = 3;
	std::string tag = "Mage";

	//Entity Components
	std::string animationName = "RangerWeaponRight";   // Animation the weapon should use.
	int lifespan = 10;								    // Time the weapon should stay on screen.

	//Functional Components
	int damage = 3;										// Amount of base damage the weapon should use.
	int delay = 0;
	bool ranged = true;
};