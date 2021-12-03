#include "Items.h"

void Item::use(std::shared_ptr<Entity> entity)
{
}

void HealthPotion::use(std::shared_ptr<Entity> entity)
{
	if (entity->hasComponent<CHealth>())
	{
		entity->getComponent<CHealth>().current = entity->getComponent<CHealth>().max;
	}
}

void InvulnPotion::use(std::shared_ptr<Entity> entity)
{
	entity->addComponent<CInvincibility>(600);
}

void StrengthPotion::use(std::shared_ptr<Entity> entity)
{
	if (entity->hasComponent<CDamage>())
	{
		entity->getComponent<CDamage>().damage = 2;
	}
}
