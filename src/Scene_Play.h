///\/\/\\\\//\/\/\\///\//\////\\\/\/\/\\\\//\/\/\\///\//\////\\\
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2021FALL
//  File Name:       Scene_Play.h
// 
//  Student Name:    Nicholas Anstey
//  Student User:    nmcanstey
//  Student Email:   nmcanstey@mun.ca
//  Student ID:      201824968
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\\//\/\/\\///\//\////\\\/\/\/\\\\//\/\/\\///\//\////\\\

#pragma once

#include "Common.h"
#include "Scene.h"
#include <map>
#include <memory>
#include <vector>

#include "EntityManager.h"

class Scene_Play : public Scene
{

    struct PlayerConfig
    {
        float X, Y, CX, CY, SPEED, HEALTH, GRAVITY, JUMP;
        std::string WEAPON;
    };

protected:

    std::shared_ptr<Entity> m_player;
    std::string             m_levelPath;
    PlayerConfig            m_playerConfig;
    bool                    m_drawTextures = true;
    bool                    m_drawCollision = false;
    bool                    m_follow = false;

    std::vector<Vec2>       BlackTiles;

    // NEW
    bool                        m_drawInventory = false;    // NEW
    std::vector<std::string>    m_inventoryStrings;         // NEW
    size_t                      m_selectedMenuIndex = 0;    // NEW
    sf::Text                    m_menuText;                 // NEW

    std::vector<std::shared_ptr<Item>> m_items;
    
    void init(const std::string & levelPath);

    void loadLevel(const std::string & filename);
    void saveLevel(const std::string & filename);

    void onEnd();
    void update();
    void spawnPlayer();
    void spawnWeapon(std::shared_ptr<Entity> entity);
    Vec2 getPosition(int sx, int sy, int tx, int ty) const;
    
    void sMovement();
    void sAI();
    void sStatus();
    void sAnimation();
    void sCollision();
    bool sAddItem(std::shared_ptr<Entity> entity, std::shared_ptr<Item> item);
    void sUseItem(std::shared_ptr<Entity> entity, std::shared_ptr<Item> item);
    void sSwapWeapon();
    void sTestValue();
    void sCamera();
    void sLighting();
    

public:

    Scene_Play(GameEngine* gameEngine, const std::string& levelPath);

    void sRender();
    void sDoAction(const Action& action);
    void doCollide(std::shared_ptr<Entity>, std::shared_ptr<Entity>);
};
                                      
// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2021FALL - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
