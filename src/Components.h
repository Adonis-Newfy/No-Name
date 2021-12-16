///\/\/\\\\//\/\/\\///\//\////\\\/\/\/\\\\//\/\/\\///\//\////\\\
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2021FALL
//  File Name:       Components.h
// 
//  Student Name:    Nicholas Anstey
//  Student User:    nmcanstey
//  Student Email:   nmcanstey@mun.ca
//  Student ID:      201824968
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\\//\/\/\\///\//\////\\\/\/\/\\\\//\/\/\\///\//\////\\\

#pragma once
                                      
#include "Animation.h"
#include "Assets.h"
#include "Items.h"
#include "Weapons.h"
#include "Common.h"

class Item;

class Component
{
public:
    bool has = false;
};
                                      
class CTransform : public Component
{
public:
    Vec2 pos = { 0.0, 0.0 };
    Vec2 prevPos = { 0.0, 0.0 };
    Vec2 scale = { 1.0, 1.0 };
    Vec2 velocity = { 0.0, 0.0 };
    Vec2 facing = { 0.0, 1.0 };
    float angle = 0;

    CTransform() {}
    CTransform(const Vec2& p)
        : pos(p) {}
    CTransform(const Vec2& p, const Vec2& sp, const Vec2& sc, float a)
        : pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) {}

};

class CLifeSpan : public Component
{
public:
    int lifespan = 0;
    int frameCreated = 0;
    CLifeSpan() {}
    CLifeSpan(int duration, int frame)
        : lifespan(duration), frameCreated(frame) {}
};

class CDamage : public Component
{
public:
    int damage = 1;
    CDamage() {}
    CDamage(int d)
        : damage(d) {}
};

class CInvincibility : public Component
{
public:
    int iframes = 0;
    CInvincibility() {}
    CInvincibility(int f)
        : iframes(f) {}
};

class CHealth : public Component
{
public:
    int max = 1;
    int current = 1;
    CHealth() {}
    CHealth(int m, int c)
        : max(m), current(c) {}
};

class CInput : public Component
{
public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool attack = false;
    bool shoot = false;
    bool canShoot = true;
    bool canJump = true;

    CInput() {}
};

class CGravity : public Component
{
public:
    float gravity = 0;
    CGravity() {}
    CGravity(float g) : gravity(g) {}
};

class CBoundingBox : public Component
{
public:
    Vec2 size;
    Vec2 halfSize;
    bool blockMove = false;
    bool blockVision = false;
    CBoundingBox() {}
    CBoundingBox(const Vec2& s)
        : size(s), halfSize(s.x / 2, s.y / 2) {}
    CBoundingBox(const Vec2& s, bool m, bool v)
        : size(s), blockMove(m), blockVision(v), halfSize(s.x / 2, s.y / 2) {}
};
                                      
class CAnimation : public Component
{
public:
    Animation animation;
    bool repeat = false;
    CAnimation() {}
    CAnimation(const Animation& animation, bool r)
        : animation(animation), repeat(r) {}
};

class CState : public Component
{
public:
    std::string state = "stand";
    CState() {}
    CState(const std::string& s) : state(s) {}
};

class CFollowPlayer : public Component
{
public:
    Vec2 home = { 0, 0 };
    float speed = 0;
    CFollowPlayer() {}
    CFollowPlayer(Vec2 p, float s)
        : home(p), speed(s) {}

};
                                      
class CPatrol : public Component
{
public:
    std::vector<Vec2> positions;
    size_t currentPosition = 0;
    float speed = 0;
    CPatrol() {}
    CPatrol(std::vector<Vec2>& pos, float s) : positions(pos), speed(s) {}
};


class CInventory : public Component
{
public:
    std::vector<std::shared_ptr<Item>> items;
    size_t maxSize = 3;
    size_t currentSize = 0;
    CInventory() {}
    CInventory(size_t s, size_t c) : maxSize(s), currentSize(c) {}
};

class CWeapons : public Component
{
public:
    size_t selected = 0;
    std::vector<Weapon> weapons;
    std::vector<bool> unlocked;

    CWeapons() 
    {
        Basic basic;
        Warrior warrior;
        Ranger ranger;
        Mage mage;

        weapons.push_back(basic);           // 0
        weapons.push_back(warrior);         // 1
        weapons.push_back(ranger);          // 2
        weapons.push_back(mage);            // 3

        unlocked.push_back(true);

        for (int i = 1; i < 4; i++)
        {
            unlocked.push_back(true);
        }
    }

    void selectWeapon(int i)
    {
        if (unlocked[i] == true)
        {
            selected = i;
        }
    }

    void unlockWeapon(int i)
    {
        if (i < weapons.size())
        {
            unlocked[i] = true;
        }
    }

    Weapon getWeapon()
    {
        return weapons[selected];
    }
};

class CBuffed : public Component
{
public:
    int multiplier = 1;
    int duration = 0;
    CBuffed() {}
    CBuffed(int m, int duration)
        : multiplier(m), duration(duration) {}
};

class CCooldown : public Component
{
public:
    bool ready = true;
    int curTimer = 0;
    int maxTimer = 0;

    CCooldown() {}
    CCooldown(int c) : curTimer(c), maxTimer(c)
    {
        ready = false;
    }
};

class CLayer : public Component
{
public:
    int layer;
    CLayer() {}
    CLayer(int l) : layer(l) {}
};
                                      
// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2021FALL - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
