///\/\/\\\\//\/\/\\///\//\////\\\/\/\/\\\\//\/\/\\///\//\////\\\
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2021FALL
//  File Name:       Scene_Play.cpp
// 
//  Student Name:    Nicholas Anstey
//  Student User:    nmcanstey
//  Student Email:   nmcanstey@mun.ca
//  Student ID:      201824968
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\\//\/\/\\///\//\////\\\/\/\/\\\\//\/\/\\///\//\////\\\
                                      
#include "Scene_Play.h"
#include "Common.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Scene_Menu.h"
#include "Scene_Levels.h"
#include "Scene_GameOver.h"
#include "Scene_Victory.h"

int totalMoney = 0;
int levelMoney = 0;

Scene_Play::Scene_Play(GameEngine* game, const std::string& levelPath)
    : Scene(game)
    , m_levelPath(levelPath)
{
    init(m_levelPath);
}

void Scene_Play::init(const std::string& levelPath)
{

    loadParameters(m_parameters);

    //Register Default Keybinds
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Y, "TOGGLE_FOLLOW");       // Toggle follow camera
    registerAction(sf::Keyboard::B, "TOGGLE_BOX");          // Toggle follow camera
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");      // Toggle drawing (T)extures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");    // Toggle drawing (C)ollision Boxes
    registerAction(sf::Keyboard::W, "JUMP");                // Jump
    registerAction(sf::Keyboard::A, "MOVE_LEFT");           // Move Left
    registerAction(sf::Keyboard::D, "MOVE_RIGHT");          // Move Right    

    registerAction(sf::Keyboard::Space, "ATTACK");          // Attack
    registerAction(sf::Keyboard::Tab, "SWAP");              // Swap Currently Equipped Weapon
    registerAction(sf::Keyboard::V, "TEST");                // For testing purposes, prints a value to console.

    registerAction(sf::Keyboard::Up, "MENU_UP");             // NEW: Move menu option selection up
    registerAction(sf::Keyboard::Down, "MENU_DOWN");           // NEW: Move menu option selection down
    registerAction(sf::Keyboard::Right, "MENU_POSITIVE");       // NEW: Select menu option
    registerAction(sf::Keyboard::Left, "MENU_NEGATIVE");       // NEW: Select menu option

    registerAction(sf::Keyboard::I, "OPEN_INVENTORY");      // NEW: Open Inventory

    loadLevel(levelPath);

    unlocked.push_back(true);

    for (int i = 1; i < 5; i++)
    {
        unlocked.push_back(false);
    }


    

    m_inventoryStrings.push_back("Health Potion");
    m_inventoryStrings.push_back("Defense Potion");
    m_inventoryStrings.push_back("Damage Potion");
    m_menuText.setFont(m_game->assets().getFont("Golem"));
    m_menuText.setCharacterSize(32);

    auto HP = std::shared_ptr<HealthPotion>(new HealthPotion());
    auto IP = std::shared_ptr<InvulnPotion>(new InvulnPotion());
    auto SP = std::shared_ptr<StrengthPotion>(new StrengthPotion());
    m_items.push_back(HP);
    m_items.push_back(IP);
    m_items.push_back(SP);

    loadParameters(m_parameters);

    loadData(m_saveData);

    std::srand(static_cast<unsigned int>(std::time(NULL)));

    m_game->window().setView(m_game->window().getDefaultView());

    std::cout << m_difficulty << std::endl;
}


void Scene_Play::loadLevel(const std::string& filename)
{
    m_entityManager = EntityManager();

    std::ifstream fin(filename);

    std::string text = "";

    int count = 1;

    while (fin >> text)
    {
        std::cout << text << "\n";

        if (text == "Player")
        {
            int spawnX;
            int spawnY;
            int boxX;
            int boxY;
            int speed;
            int health;
            float gravity;
            float jump;

            fin >> spawnX >> spawnY >> boxX >> boxY >> speed >> health >> gravity >> jump;

            m_playerConfig.X = spawnX;
            m_playerConfig.Y = spawnY;
            m_playerConfig.CX = boxX;
            m_playerConfig.CY = boxY;
            m_playerConfig.SPEED = speed;
            m_playerConfig.HEALTH = health / m_difficulty;
            m_playerConfig.GRAVITY = gravity;
            m_playerConfig.JUMP = jump;

        }

        if (text == "Tile")
        {
            std::string animationName = "";
            int roomX;
            int roomY;
            int tileX;
            int tileY;
            int blockV;
            int blockM;

            std::string AI = "";

            fin >> animationName >> roomX >> roomY >> tileX >> tileY >> blockM >> blockV >> AI;

            if (AI == "Patrol")
            {
                float speed;
                int patrolCount;
                std::vector<Vec2> patrolPositions;

                fin >> speed >> patrolCount;

                for (int i = 0; i < patrolCount; i++)
                {
                    int x;
                    int y;

                    fin >> x >> y;

                    Vec2 position = getPosition(roomX, roomY, x, y);

                    patrolPositions.push_back(position);
                }

                auto tile = m_entityManager.addEntity("tile");
                tile->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
                tile->addComponent<CTransform>(getPosition(roomX, roomY, tileX, tileY));
                tile->addComponent<CBoundingBox>(m_game->assets().getAnimation(animationName).getSize(), blockM, blockV);
                tile->addComponent<CPatrol>(patrolPositions, speed);
            }

            if (AI == "Static")
            {
                auto tile = m_entityManager.addEntity("tile");
                tile->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
                tile->addComponent<CTransform>(getPosition(roomX, roomY, tileX, tileY));
                tile->addComponent<CBoundingBox>(m_game->assets().getAnimation(animationName).getSize(), blockM, blockV);
            }
        }

        if (text == "KillTile")
        {
            std::string animationName = "";
            int roomX;
            int roomY;
            int tileX;
            int tileY;
            int blockV;
            int blockM;

            fin >> animationName >> roomX >> roomY >> tileX >> tileY >> blockM >> blockV;

            auto tile = m_entityManager.addEntity("killtile");
            tile->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
            tile->addComponent<CTransform>(getPosition(roomX, roomY, tileX, tileY));
            tile->addComponent<CBoundingBox>(m_game->assets().getAnimation(animationName).getSize(), blockM, blockV);
        }

        if (text == "Button")
        {
            std::string animationName = "";
            int roomX;
            int roomY;
            int tileX;
            int tileY;
            int blockV;
            int blockM;
            int unlock; //1 = warrior, 2 = mage, 3 = ranger

            fin >> animationName >> roomX >> roomY >> tileX >> tileY >> blockM >> blockV >> unlock;

            auto tile = m_entityManager.addEntity("button");
            tile->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
            tile->addComponent<CTransform>(getPosition(roomX, roomY, tileX, tileY));
            tile->addComponent<CBoundingBox>(m_game->assets().getAnimation(animationName).getSize(), blockM, blockV);
            tile->addComponent<CWeapons>();

            tile->getComponent<CWeapons>().unlockWeapon(unlock);
            tile->getComponent<CWeapons>().selectWeapon(unlock);
        }

        if (text == "Dec")
        {
            std::string animationName = "";
            int roomX;
            int roomY;
            int tileX;
            int tileY;
            int blockV;
            int blockM;
            int layer;

            fin >> animationName >> roomX >> roomY >> tileX >> tileY >> blockM >> blockV >> layer;

            auto dec = m_entityManager.addEntity("dec");
            dec->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
            dec->addComponent<CTransform>(getPosition(roomX, roomY, tileX, tileY));
            dec->addComponent<CBoundingBox>(m_game->assets().getAnimation(animationName).getSize(), blockM, blockV);
            dec->addComponent<CLayer>(layer);
        }

        if (text == "NPC")
        {
            std::string animationName = "";
            int roomX;
            int roomY;
            int tileX;
            int tileY;
            int blockV;
            int blockM;
            int maxHealth;
            int damage;
            float gravity;

            std::string AI = "";

            fin >> animationName >> roomX >> roomY >> tileX >> tileY >> blockM >> blockV >> maxHealth >> damage >> gravity >> AI;

            if (AI == "Patrol")
            {
                float speed;
                int patrolCount;
                std::vector<Vec2> patrolPositions;

                fin >> speed >> patrolCount;

                for (int i = 0; i < patrolCount; i++)
                {
                    int x;
                    int y;

                    fin >> x >> y;

                    Vec2 position = getPosition(roomX, roomY, x, y);

                    patrolPositions.push_back(position);
                }

                auto npc = m_entityManager.addEntity("npc");
                npc->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
                npc->addComponent<CTransform>(getPosition(roomX, roomY, tileX, tileY));
                npc->addComponent<CBoundingBox>(m_game->assets().getAnimation(animationName).getSize(), blockM, blockV);
                npc->addComponent<CHealth>(maxHealth, maxHealth);
                npc->addComponent<CDamage>(damage * m_difficulty);
                npc->addComponent<CPatrol>(patrolPositions, speed);
                npc->addComponent<CGravity>(gravity);
            }

            if (AI == "Follow")
            {
                float speed;

                fin >> speed;

                auto npc = m_entityManager.addEntity("npc");
                npc->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
                npc->addComponent<CTransform>(getPosition(roomX, roomY, tileX, tileY));
                npc->addComponent<CBoundingBox>(m_game->assets().getAnimation(animationName).getSize(), blockM, blockV);
                npc->addComponent<CHealth>(maxHealth, maxHealth);
                npc->addComponent<CDamage>(damage * m_difficulty);
                npc->addComponent<CFollowPlayer>(getPosition(roomX, roomY, tileX, tileY), speed);
                npc->addComponent<CGravity>(gravity);
            }

            if (AI == "Boss")
            {
                auto npc = m_entityManager.addEntity("boss");
                npc->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
                npc->addComponent<CTransform>(getPosition(roomX, roomY, tileX, tileY));
                npc->addComponent<CBoundingBox>(m_game->assets().getAnimation(animationName).getSize(), blockM, blockV);
                npc->addComponent<CHealth>(maxHealth, maxHealth);
                npc->addComponent<CDamage>(damage * m_difficulty);
                npc->addComponent<CBoss>();
                npc->addComponent<CGravity>(gravity);
            }

            if (AI == "BossArm")
            {
                float speed;
                int patrolCount;
                std::vector<Vec2> patrolPositions;

                fin >> speed >> patrolCount;

                for (int i = 0; i < patrolCount; i++)
                {
                    int x;
                    int y;

                    fin >> x >> y;

                    Vec2 position = getPosition(roomX, roomY, x, y);

                    patrolPositions.push_back(position);
                }

                auto npc = m_entityManager.addEntity("bossarm");
                npc->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
                npc->addComponent<CTransform>(getPosition(roomX, roomY, tileX, tileY));
                npc->addComponent<CBoundingBox>(m_game->assets().getAnimation(animationName).getSize(), blockM, blockV);
                npc->addComponent<CDamage>(damage * m_difficulty);
                npc->addComponent<CPatrol>(patrolPositions, speed);
                npc->addComponent<CGravity>(gravity);
            }
        }

        if (text == "Item")
        {
            std::string animationName = "";
            int roomX;
            int roomY;
            int tileX;
            int tileY;
            int blockV;
            int blockM;
            std::string itemName = "";

            fin >> animationName >> roomX >> roomY >> tileX >> tileY >> blockM >> blockV >> itemName;

            auto item = m_entityManager.addEntity("item");
            item->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
            item->addComponent<CTransform>(getPosition(roomX, roomY, tileX, tileY));
            item->addComponent<CBoundingBox>(m_game->assets().getAnimation(animationName).getSize(), blockM, blockV);
            item->addComponent<CInventory>(1, 0);

            std::cout << itemName << std::endl;

            if (itemName == "HP")
            {
                auto potion = std::shared_ptr<HealthPotion>(new HealthPotion());
                sAddItem(item, potion);
                std::cout << potion->itemID << "\n";
                std::cout << potion->tag << "\n";
            }
            else if (itemName == "IP")
            {
                auto potion = std::shared_ptr<InvulnPotion>(new InvulnPotion());
                sAddItem(item, potion);
            }
            else if (itemName == "SP")
            {
                auto potion = std::shared_ptr<StrengthPotion>(new StrengthPotion());
                sAddItem(item, potion);
            }
            else
            {
                std::cout << "Invalid item name from file!\n";
            }
        }
    }
    
    // Load the level file and put all entities in the manager
    // Use the getPosition() function below to convert room-tile coords to game world coords
                                      
    spawnPlayer();
    
}

void Scene_Play::saveData(const std::string& filename)
{
    std::ofstream file(filename);

    auto weapons = m_player->getComponent<CWeapons>().unlocked;

    file << "BasicData " << weapons[0] << " " << weapons[1] << " " << weapons[2] << " " << weapons[3] << " " << float(m_player->getComponent<CHealth>().current) / float(m_player->getComponent<CHealth>().max)
    << " " << (totalMoney + levelMoney) << std::endl;

    file << "UnlockedLevels " << unlocked[0] << " " << unlocked[1] << " " << unlocked[2] << " " << unlocked[3] << " " << unlocked[4] << std::endl;

for (int i = 0; i < m_player->getComponent<CInventory>().currentSize; i++)
{
    int item = 0;
    bool flag = false;

    if (m_player->getComponent<CInventory>().items[i]->itemID == m_items[0]->itemID)
    {
        item = 0;
        flag = true;
    }

    else if (m_player->getComponent<CInventory>().items[i]->itemID == m_items[1]->itemID)
    {
        item = 1;
        flag = true;
    }

    else if (m_player->getComponent<CInventory>().items[i]->itemID == m_items[2]->itemID)
    {
        item = 2;
        flag = true;
    }

    if (flag == true)
    {
        file << "Inventory " << item << std::endl;
        flag = false;
    }

}

/*
* BasicData Base War Ran Mag HP Gold
* UnlockedLevels One Two Three Final
* Inventory ............................
*/
}

void Scene_Play::loadParameters(const std::string& filename)
{
    std::ifstream fin(filename);
    std::string text;

    std::string actionToBind;
    int actionKeyCode;

    while (fin >> text)
    {
        if (text == "Difficulty")
        {
            fin >> m_difficulty;
        }
        if (text == "Keybind")
        {
            fin >> actionToBind >> actionKeyCode;

            registerAction(actionKeyCode, actionToBind);
        }
    }
}

void Scene_Play::loadData(const std::string& filename)
{
    std::ifstream fin(filename);
    std::string text;

    while (fin >> text)
    {
        if (text == "BasicData")
        {
            int unlockedB;
            int unlockedW;
            int unlockedR;
            int unlockedM;

            float hp;

            int money;

            fin >> unlockedB >> unlockedW >> unlockedR >> unlockedM >> hp >> money;

            if (unlockedB)
            {
                m_player->getComponent<CWeapons>().unlockWeapon(0);
            }
            if (unlockedW)
            {
                m_player->getComponent<CWeapons>().unlockWeapon(1);
            }
            if (unlockedR)
            {
                m_player->getComponent<CWeapons>().unlockWeapon(2);
            }
            if (unlockedM)
            {
                m_player->getComponent<CWeapons>().unlockWeapon(3);
            }

            if (int((m_player->getComponent<CHealth>().max) * hp) <= 0)
            {
                m_player->getComponent<CHealth>().current = 1;
            }
            else
            {
                m_player->getComponent<CHealth>().current = (m_player->getComponent<CHealth>().max) * hp;
            }

            totalMoney = money;
        }

        if (text == "UnlockedLevels")
        {
            std::vector<bool> levels;

            int level1;
            int level2;
            int level3;
            int level4;
            int level5;

            fin >> level1 >> level2 >> level3 >> level4 >> level5;

            levels.push_back(level1);
            levels.push_back(level2);
            levels.push_back(level3);
            levels.push_back(level4);
            levels.push_back(level5);

            unlocked = std::move(levels);
        }

        if (text == "Inventory")
        {
            int item;

            fin >> item;

            sAddItem(m_player, m_items[item]);
        }
    }
}
                                      
Vec2 Scene_Play::getPosition(int rx, int ry, int tx, int ty) const
{
    
    // Implement this function, which takes in the room (rx, ry) coordinate
    // as well as the tile (tx, ty) coordinate, and returns the Vec2 game world 
    // position of the center of the entitiy. 
    //
    // All tiles are of size 64x64 for this assignment
    
    
    int xPos = (rx * (1280.0f)) + (64.0f * tx) + 32.0f;
    int yPos = (ry * (768.0f)) + (64.0f * ty) + 32.0f;

    return Vec2(xPos, yPos);

}

void Scene_Play::spawnPlayer()
{
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CTransform>(Vec2(m_playerConfig.X, m_playerConfig.Y));
    m_player->addComponent<CAnimation>(m_game->assets().getAnimation("StandDown"), true);
    m_player->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY), true, false);
    m_player->addComponent<CHealth>(m_playerConfig.HEALTH, m_playerConfig.HEALTH);
    m_player->addComponent<CInput>();
    m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
    m_player->addComponent<CState>();
    m_player->addComponent<CInventory>();
    m_player->addComponent<CDamage>();
    m_player->addComponent<CBuffed>();
    m_player->addComponent<CWeapons>();
    m_player->addComponent<CCooldown>();


    // Implement this function so that it uses the parameters input from the level file
    // Those parameters should be stored in the m_playerConfig variable
}

void Scene_Play::spawnWeapon(std::shared_ptr<Entity> entity)
{
    
    // Implement the spawning of the weapon, which:
    // - should be given the appropriate lifespan
    // - should spawn at the appropriate location based on player's facing direction
    // - be given a damage value of 1
    // - should play the "Slash" sound

    
    auto& pTransform = m_player->getComponent<CTransform>();

    auto& pState = m_player->getComponent<CState>();

    auto& pWeapon = m_player->getComponent<CWeapons>().getWeapon();

    if (entity->getComponent<CInput>().attack == false && m_player->getComponent<CCooldown>().ready == true)
    {
        Vec2 weaponPos = entity->getComponent<CTransform>().pos;


        Vec2 offset = Vec2(64.0f, 64.0f);

        if (pWeapon.ranged == false)
        {
            auto weapon = m_entityManager.addEntity("weapon");
            weapon->addComponent<CTransform>(weaponPos);
            weapon->addComponent<CAnimation>(m_game->assets().getAnimation(pWeapon.animationName), true);
            weapon->addComponent<CBoundingBox>(weapon->getComponent<CAnimation>().animation.getSize());
            weapon->addComponent<CLifeSpan>(10, m_currentFrame);
            weapon->addComponent<CDamage>(pWeapon.damage * m_player->getComponent<CBuffed>().multiplier / m_difficulty);

            m_player->addComponent<CCooldown>(pWeapon.delay);

            m_game->playSound("Attack");
        }
        else if (pWeapon.ranged == true)
        {
            Vec2 bulletPos = entity->getComponent<CTransform>().pos;

            auto bullet = m_entityManager.addEntity("bullet");

            bullet->addComponent<CAnimation>(m_game->assets().getAnimation(pWeapon.animationName), true);
            bullet->addComponent<CTransform>(bulletPos);
            bullet->addComponent<CBoundingBox>(bullet->getComponent<CAnimation>().animation.getSize());
            bullet->addComponent<CLifeSpan>(60, m_currentFrame);
            bullet->addComponent<CDamage>(pWeapon.damage * m_player->getComponent<CBuffed>().multiplier / m_difficulty);


            if (entity->getComponent<CTransform>().scale.x == 1)
            {
                bullet->getComponent<CTransform>().velocity.x = m_playerConfig.SPEED * 2;
            }
            else if (entity->getComponent<CTransform>().scale.x == -1)
            {
                bullet->getComponent<CTransform>().velocity.x = -m_playerConfig.SPEED * 2;
            }

            m_player->addComponent<CCooldown>(pWeapon.delay);
        }

    }
  
}

void Scene_Play::sSwapWeapon()
{
    auto& pWeapon = m_player->getComponent<CWeapons>();

    if ((pWeapon.selected + 1) < pWeapon.unlocked.size())
        if (pWeapon.unlocked[pWeapon.selected + 1] == true)
        {
            pWeapon.selected += 1;
        }
        else
            pWeapon.selected = 0;
    else
        pWeapon.selected = 0;
}

void Scene_Play::sTestValue()
{
    std::cout << levelMoney << std::endl;
}

void Scene_Play::update()
{
    m_entityManager.update();
                                      
    
    // Implement pause functionality
       
    if (!m_paused && !m_drawInventory)
    {
        sAI();
        sMovement();
        sStatus();
        sCollision();
        sAnimation();
        sCamera();
        //sLighting();
        // add dim shader to screen for when paused
    }
    

    m_currentFrame++;
}

void Scene_Play::sMovement()
{
    
    // Implement all player movement functionality here based on 
    // the player's input component variables

    auto& pTransform = m_player->getComponent<CTransform>();
    auto& pInput = m_player->getComponent<CInput>();

    auto& pState = m_player->getComponent<CState>();

    Vec2 playerSpeed = m_player->getComponent<CTransform>().velocity;

    playerSpeed.x = 0;

    if (pInput.right)
    {
        playerSpeed.x += m_playerConfig.SPEED;
        pTransform.facing = Vec2(1.0f, 0.0f);
    }
    if (pInput.left)
    {
        playerSpeed.x -= m_playerConfig.SPEED;
        pTransform.facing = Vec2(-1.0f, 0.0f);
    }
    if (pInput.up && pInput.canJump == true)
    {
        pInput.canJump = false;
        playerSpeed.y += m_playerConfig.JUMP;
    }
    if (!pInput.up && pTransform.velocity.y < 0)
    {
        playerSpeed.y = 0;
    }

    //Update the animation based on state of the player
    if (abs(pTransform.velocity.y) > 0)
    {
        m_player->getComponent<CState>().state = "jumping";
    }
    else if (abs(pTransform.velocity.x) > 0)
    {
        m_player->getComponent<CState>().state = "running";
    }
    else
    {
        m_player->getComponent<CState>().state = "standing";
    }

    /*
    if (pInput.right || pInput.left)
    {
        if (pInput.attack == false)
        {
            pState.state = "runningSide";
        }

        if (pInput.right)
        {
            playerSpeed.x += m_playerConfig.SPEED;
            pTransform.facing = Vec2(1.0f, 0.0f);
        }
        if (pInput.left)
        {
            playerSpeed.x -= m_playerConfig.SPEED;
            pTransform.facing = Vec2(-1.0f, 0.0f);
        }
    }
   
    else if (pInput.up || pInput.down)
    {
        if (pInput.up)
        {
            if (pInput.attack == false)
            {
                pState.state = "runningUp";
            }

            playerSpeed.y -= m_playerConfig.SPEED;
            pTransform.facing = Vec2(0.0f, -1.0f);
        }
        if (pInput.down)
        {
            if (pInput.attack == false)
            {
                pState.state = "runningDown";
            }
            
            playerSpeed.y += m_playerConfig.SPEED;
            pTransform.facing = Vec2(0.0f, 1.0f);
        }
    }
    else
    {
        if (abs(pTransform.facing.x) > 0.0f)
        {
            pState.state = "idleSide";
        }

        else if (pTransform.facing.y < 0.0f)
        {
            pState.state = "idleUp";
        }

        else if (pTransform.facing.y > 0.0f)
        {
            pState.state = "idleDown";
        }
    }
    */
   

    pTransform.velocity = playerSpeed;


    if (pTransform.velocity.x < 0)
    {
        m_player->getComponent<CTransform>().scale.x = -1.0f;
    }
    else if (pTransform.velocity.x > 0)
    {
        m_player->getComponent<CTransform>().scale.x = 1.0f;
    }

    for (auto& e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CGravity>())
        {
            e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity;
        }
        e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
        e->getComponent<CTransform>().pos += e->getComponent<CTransform>().velocity;
    }
}

void Scene_Play::sDoAction(const Action& action)
{
    
    // Implement all actions described for the game here
    // Only the setting of the player's input component variables should be set here
    // Do minimal logic in this function

    if (action.type() == "START")
    {
        
             if (action.name() == "PAUSE") { setPaused(!m_paused); }
        else if (action.name() == "QUIT") { onEnd(); }
        else if (action.name() == "TOGGLE_FOLLOW") { m_follow = !m_follow; } /*if (!m_follow) { sf::View view = m_game->window().getView(); view.zoom(0.8f); m_game->window().setView(view); }
        else { sf::View view = m_game->window().getDefaultView(); m_game->window().setView(view); }*/
        else if (action.name() == "TOGGLE_BOX") { m_box = !m_box; }
        else if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
        else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
        else if (action.name() == "JUMP") { m_player->getComponent<CInput>().up = true; }
        else if (action.name() == "MOVE_LEFT") { m_player->getComponent<CInput>().left = true; }
        else if (action.name() == "MOVE_RIGHT") { m_player->getComponent<CInput>().right = true; }
        else if (action.name() == "ATTACK") { spawnWeapon(m_player);  if (m_player->getComponent<CCooldown>().ready == true) m_player->getComponent<CInput>().attack = true; }
        else if (action.name() == "ITEM") { sUseItem(m_player, m_player->getComponent<CInventory>().items[0]); } //Uses the first item in the player's inventory
        else if (action.name() == "SWAP") { sSwapWeapon(); }
        else if (action.name() == "TEST") { sTestValue(); }
        else if (action.name() == "OPEN_INVENTORY")
             {
                 if (!m_paused)
                 {
                     m_selectedMenuIndex = 0;

                     m_drawInventory = !m_drawInventory;

                 }
             }
        else if (action.name() == "MENU_UP")
             {
                 if (m_drawInventory || m_paused)
                 {
                     if (m_selectedMenuIndex > 0)
                     {
                         m_selectedMenuIndex--;
                     }
                     else if (m_drawInventory)
                     {
                         m_selectedMenuIndex = m_inventoryStrings.size() - 1;
                     }
                 }
             }
        else if (action.name() == "MENU_DOWN")
             {
                 if (m_drawInventory)
                 {
                     m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_inventoryStrings.size();
                 }

             }
        else if (action.name() == "MENU_POSITIVE")
             {
                 if (m_selectedMenuIndex == 0)
                 {
                     sUseItem(m_player, m_items[0]);
                 }

                 if (m_selectedMenuIndex == 1)
                 {
                     sUseItem(m_player, m_items[1]);
                 }

                 if (m_selectedMenuIndex == 2)
                 {
                     sUseItem(m_player, m_items[2]);
                 }
             }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "JUMP") { m_player->getComponent<CInput>().up = false; }
        else if (action.name() == "MOVE_LEFT") { m_player->getComponent<CInput>().left = false; }
        //else if (action.name() == "MOVE_DOWN") { m_player->getComponent<CInput>().down = false; }
        else if (action.name() == "MOVE_RIGHT") { m_player->getComponent<CInput>().right = false; }
    }
}

void Scene_Play::sAI()
{
    
    // Implement Follow behavior
    // Implement Patrol behavior

    for (auto e : m_entityManager.getEntities())
    {
        if (e->tag() == "npc" || e->tag() == "tile" || e->tag() == "bossarm" || e->tag() == "enemybullet")
        {
            if (e->hasComponent<CGravity>())
            {
                if (abs(e->getComponent<CGravity>().gravity) > 0)
                {
                    e->getComponent<CTransform>().velocity.x = 0;
                }
                else
                {
                    e->getComponent<CTransform>().velocity = Vec2(0, 0);
                }
            }
            else
            {
                e->getComponent<CTransform>().velocity = Vec2(0, 0);
            }


            if (e->hasComponent<CPatrol>())
            {
                //Do patrol things
                if ((e->getComponent<CTransform>().pos - e->getComponent<CPatrol>().positions[(e->getComponent<CPatrol>().currentPosition)]).abs() <= Vec2(5, 5))
                {
                    if ((e->getComponent<CPatrol>().currentPosition + 1) >= e->getComponent<CPatrol>().positions.size())
                    {
                        e->getComponent<CPatrol>().currentPosition = 0;
                    }
                    else
                    {
                        e->getComponent<CPatrol>().currentPosition++;
                    }
                }
                else
                {
                    Vec2 desired = e->getComponent<CPatrol>().positions[e->getComponent<CPatrol>().currentPosition] - e->getComponent<CTransform>().pos;
                    float magnitude = sqrt((desired.x * desired.x) + (desired.y * desired.y));
                    desired.x = desired.x / magnitude;
                    desired.y = desired.y / magnitude;
                    desired = desired * e->getComponent<CPatrol>().speed;

                    float scale = 1.0f;

                    Vec2 steer = desired - e->getComponent<CTransform>().velocity;
                    steer = steer * scale;

                    e->getComponent<CTransform>().velocity = e->getComponent<CTransform>().velocity + steer;
                    /*
                    *                     if (e->getComponent<CPatrol>().positions[(e->getComponent<CPatrol>().currentPosition)].x > e->getComponent<CTransform>().pos.x)
                    {
                        e->getComponent<CTransform>().velocity.x = e->getComponent<CPatrol>().speed;
                    }
                    if (e->getComponent<CPatrol>().positions[(e->getComponent<CPatrol>().currentPosition)].x < e->getComponent<CTransform>().pos.x)
                    {
                        e->getComponent<CTransform>().velocity.x = -e->getComponent<CPatrol>().speed;
                    }
                    if (e->getComponent<CPatrol>().positions[(e->getComponent<CPatrol>().currentPosition)].y > e->getComponent<CTransform>().pos.y)
                    {
                        e->getComponent<CTransform>().velocity.y = e->getComponent<CPatrol>().speed;
                    }
                    if (e->getComponent<CPatrol>().positions[(e->getComponent<CPatrol>().currentPosition)].y < e->getComponent<CTransform>().pos.y)
                    {
                        e->getComponent<CTransform>().velocity.y = -e->getComponent<CPatrol>().speed;
                    }
                    */

                }

                if (e->getComponent<CTransform>().velocity.x < 0 && e->tag() != "tile")
                {
                    e->getComponent<CTransform>().scale.x = -1.0f;
                }
                else if (e->getComponent<CTransform>().velocity.x > 0 && e->tag() != "tile")
                {
                    e->getComponent<CTransform>().scale.x = 1.0f;
                }

            }

            if (e->hasComponent<CFollowPlayer>())
            {
                //Do follow things

                Vec2 desired = m_player->getComponent<CTransform>().pos - e->getComponent<CTransform>().pos;
                float magnitude = sqrt((desired.x * desired.x) + (desired.y * desired.y));
                desired.x = desired.x / magnitude;
                desired.y = desired.y / magnitude;
                desired = desired * e->getComponent<CFollowPlayer>().speed;

                float scale = 1.0f;

                Vec2 steer = desired - e->getComponent<CTransform>().velocity;
                steer = steer * scale;

                if (e->hasComponent<CGravity>())
                {
                    if (abs(e->getComponent<CGravity>().gravity) > 0)
                    {
                        int multiplier = 1;
                        if (steer.x < 0)
                            multiplier = -1;
                        e->getComponent<CTransform>().velocity.x += (multiplier * sqrtf(exp2f(steer.x) + exp2f(steer.y)));
                    }
                    else
                    {
                        e->getComponent<CTransform>().velocity = e->getComponent<CTransform>().velocity + steer;
                    }
                }
                else
                {
                    e->getComponent<CTransform>().velocity = e->getComponent<CTransform>().velocity + steer;
                }


                for (auto b : m_entityManager.getEntities())
                {
                    if (b->hasComponent<CBoundingBox>() && (b->getComponent<CBoundingBox>().blockVision == true))
                    {
                        if (Physics::EntityIntersect(m_player->getComponent<CTransform>().pos, e->getComponent<CTransform>().pos, b))
                        {
                            if (e->tag() == "enemybullet")
                            {
                                break;
                            }

                            Vec2 desired = e->getComponent<CFollowPlayer>().home - e->getComponent<CTransform>().pos;
                            if (abs(desired.x) > 0 && abs(desired.y) > 0)
                            {
                                float magnitude = sqrt((desired.x * desired.x) + (desired.y * desired.y));
                                desired.x = desired.x / magnitude;
                                desired.y = desired.y / magnitude;
                                desired = desired * e->getComponent<CFollowPlayer>().speed;

                                float scale = 1.0f;

                                Vec2 steer = desired - e->getComponent<CTransform>().velocity;
                                steer = steer * scale;

                                if (e->hasComponent<CGravity>())
                                {
                                    if (abs(e->getComponent<CGravity>().gravity) > 0)
                                    {
                                        e->getComponent<CTransform>().velocity.x += steer.x;
                                    }
                                    else
                                    {
                                        e->getComponent<CTransform>().velocity = e->getComponent<CTransform>().velocity + steer;
                                    }
                                }
                                else
                                {
                                    e->getComponent<CTransform>().velocity = e->getComponent<CTransform>().velocity + steer;
                                }

                            }
                            else
                            {
                                e->getComponent<CTransform>().velocity = Vec2(0, 0);
                            }
                        }

                    }
                }

                if (e->getComponent<CTransform>().velocity.x < 0)
                {
                    e->getComponent<CTransform>().scale.x = -1.0f;
                }
                else if (e->getComponent<CTransform>().velocity.x > 0)
                {
                    e->getComponent<CTransform>().scale.x = 1.0f;
                }
            }
        }     
    }
}

void Scene_Play::sLighting()
{
    std::vector<Vec2> visibilityPolygon;
    auto& pTransform = m_player->getComponent<CTransform>();
    sf::View view = m_game->window().getView();

    Vec2 winP1 = Vec2(view.getCenter().x - (view.getSize().x / 2.0f), view.getCenter().y - (view.getSize().y / 2.0f)); // 0, 0
    Vec2 winP2 = Vec2(view.getCenter().x + (view.getSize().x / 2.0f), view.getCenter().y - (view.getSize().y / 2.0f)); // 0, 1
    Vec2 winP3 = Vec2(view.getCenter().x - (view.getSize().x / 2.0f), view.getCenter().y + (view.getSize().y / 2.0f)); // 1, 0
    Vec2 winP4 = Vec2(view.getCenter().x + (view.getSize().x / 2.0f), view.getCenter().y + (view.getSize().y / 2.0f)); // 1, 1

    for (auto& e : m_entityManager.getEntities("tile"))
    {
        //Generates 4 points of a tile, which will be targetted by the ray to render LOS
        

        Vec2 P1 = Vec2(e->getComponent<CTransform>().pos.x - e->getComponent<CBoundingBox>().halfSize.x, e->getComponent<CTransform>().pos.y - e->getComponent<CBoundingBox>().halfSize.y);
        Vec2 P2 = Vec2(e->getComponent<CTransform>().pos.x + e->getComponent<CBoundingBox>().halfSize.x, e->getComponent<CTransform>().pos.y - e->getComponent<CBoundingBox>().halfSize.y);
        Vec2 P3 = Vec2(e->getComponent<CTransform>().pos.x + e->getComponent<CBoundingBox>().halfSize.x, e->getComponent<CTransform>().pos.y + e->getComponent<CBoundingBox>().halfSize.y);
        Vec2 P4 = Vec2(e->getComponent<CTransform>().pos.x - e->getComponent<CBoundingBox>().halfSize.x, e->getComponent<CTransform>().pos.y + e->getComponent<CBoundingBox>().halfSize.y);

        Vec2 points[4] = { P1, P2, P3, P4 };
        
        for (auto& b : m_entityManager.getEntities("tile"))
        {

            for (int j = 0; j < 4; j++)
            {
                //If no intersection between player position & target point, extend the line by arbitrarily large value in length
                if (Physics::EntityIntersect(pTransform.pos, points[j], b))
                {
                    /*
                    *                     float cX = pTransform.pos.x - points[j].x;
                    float cY = pTransform.pos.y - points[j].y;

                    float cH = sqrtf(powf(cX, 2.0) + powf(cY, 2.0));

                    float theta = atanf(cY / cX);

                    float nX = cH * cosf(theta);
                    float nY = cH * sinf(theta);

                    Vec2 target = Vec2(nX + pTransform.pos.x, nY + pTransform.pos.y);

                    //Then check collision between the target point, the arbitrarily large value, and the screen's bounding box positions
                    */

                    Vec2 B1 = Vec2(b->getComponent<CTransform>().pos.x - b->getComponent<CBoundingBox>().halfSize.x, b->getComponent<CTransform>().pos.y - b->getComponent<CBoundingBox>().halfSize.y);
                    Vec2 B2 = Vec2(b->getComponent<CTransform>().pos.x + b->getComponent<CBoundingBox>().halfSize.x, b->getComponent<CTransform>().pos.y - b->getComponent<CBoundingBox>().halfSize.y);
                    Vec2 B3 = Vec2(b->getComponent<CTransform>().pos.x + b->getComponent<CBoundingBox>().halfSize.x, b->getComponent<CTransform>().pos.y + b->getComponent<CBoundingBox>().halfSize.y);
                    Vec2 B4 = Vec2(b->getComponent<CTransform>().pos.x - b->getComponent<CBoundingBox>().halfSize.x, b->getComponent<CTransform>().pos.y + b->getComponent<CBoundingBox>().halfSize.y);

                    Intersect intersect;

                    for (int i = 0; i < 4; i++)
                    {
                        intersect = Physics::LineIntersect(points[j], pTransform.pos, B1, B2);
                        if (intersect.result == true)
                        {
                            visibilityPolygon.push_back(intersect.pos);
                            break;
                        }

                        intersect = Physics::LineIntersect(points[j], pTransform.pos, B2, B3);
                        if (intersect.result == true)
                        {
                            visibilityPolygon.push_back(intersect.pos);
                            break;
                        }

                        intersect = Physics::LineIntersect(points[j], pTransform.pos, B3, B4);
                        if (intersect.result == true)
                        {
                            visibilityPolygon.push_back(intersect.pos);
                            break;
                        }

                        intersect = Physics::LineIntersect(points[j], pTransform.pos, B4, B1);
                        if (intersect.result == true)
                        {
                            visibilityPolygon.push_back(intersect.pos);
                            break;
                        }

                    }
                }
            }
        }
    }
}


void Scene_Play::sStatus()
{
    
    // Implement Lifespan Here
    // Implement Invincibility Frames here

    // Check lifespawn of entities that have them, and destroy them if they go over

    auto& pState = m_player->getComponent<CState>();

    for (auto& e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CLifeSpan>())
        {
            // while life is remaining, decrement lifespan and alpha
            if (e->getComponent<CLifeSpan>().lifespan > 0 && e->isActive())
            {
                e->getComponent<CLifeSpan>().lifespan -= 1;
            }
            //when lifespan reaches 0, destroy the entity
            else
            {
                if (e->tag() == "weapon")
                {
                    m_player->getComponent<CInput>().attack = false;
                }

                e->destroy();

            }   
        }

        if (e->hasComponent<CInvincibility>())
        {
            if (e->getComponent<CInvincibility>().iframes > 0)
            {
                e->getComponent<CInvincibility>().iframes -= 1;
            }

            else
            {
                e->removeComponent<CInvincibility>();
            }
        }

        if (e->hasComponent<CBuffed>())
        {
            if (e->getComponent<CBuffed>().duration > 0)
            {
                e->getComponent<CBuffed>().duration--;
            }

            else
            {
                e->getComponent<CBuffed>().multiplier = 1;
            }
        }

        if (e->hasComponent<CCooldown>())
        {
            if (e->getComponent<CCooldown>().curTimer > 0)
            {
                std::cout << e->getComponent<CCooldown>().curTimer << std::endl;
                e->getComponent<CCooldown>().curTimer--;
            }
            else
            {
                e->getComponent<CCooldown>().ready = true;
            }
        }

        if (e->hasComponent<CHealth>())
        {
            if (e->tag() != "player")
            {
                if (e->getComponent<CHealth>().current <= 0)
                {
                    if (e->tag() == "boss")
                    {
                        for (auto b : m_entityManager.getEntities("bossarm"))
                        {
                            b->destroy();
                            levelMoney += b->getComponent<CDamage>().damage;
                        }
                    }
                    e->destroy();
                    levelMoney += e->getComponent<CDamage>().damage;
                    m_game->playSound("EnemyHurt");
                }
            }
            else
            {
                if (e->getComponent<CHealth>().current <= 0)
                {

                    //e->destroy();
                    //spawnPlayer();
                    //onDie(); To be implemented later
                    m_game->playSound("PlayerHurt");
                    onDie();
                }
            }
        }

        if (e->hasComponent<CBoss>())
        {
            if ((e->getComponent<CHealth>().current <= e->getComponent<CHealth>().max / 1))
            {
                e->getComponent<CBoss>().mechanicOccured = true;
                e->getComponent<CBoss>().currentTimer++;

                if (e->getComponent<CBoss>().currentTimer >= e->getComponent<CBoss>().maxTimer)
                {
                    sBossMechanic(e);
                    e->getComponent<CBoss>().currentTimer = 0;
                }
                
            }
        }
    }
}

void Scene_Play::sBossMechanic(std::shared_ptr<Entity> entity)
{
    auto bullet = m_entityManager.addEntity("enemybullet");

    bullet->addComponent<CAnimation>(m_game->assets().getAnimation("Fireball"), true);
    bullet->addComponent<CTransform>(entity->getComponent<CTransform>().pos + Vec2(32, 0));
    bullet->addComponent<CBoundingBox>(bullet->getComponent<CAnimation>().animation.getSize(), 0, 0);
    bullet->addComponent<CLifeSpan>(200, m_currentFrame);
    bullet->addComponent<CDamage>(2 * m_difficulty);
    bullet->addComponent<CFollowPlayer>(entity->getComponent<CTransform>().pos, 2);

    auto bullet2 = m_entityManager.addEntity("enemybullet");

    bullet2->addComponent<CAnimation>(m_game->assets().getAnimation("Fireball"), true);
    bullet2->addComponent<CTransform>(entity->getComponent<CTransform>().pos - Vec2(32, 0));
    bullet2->addComponent<CBoundingBox>(bullet->getComponent<CAnimation>().animation.getSize(), 0, 0);
    bullet2->addComponent<CLifeSpan>(200, m_currentFrame);
    bullet2->addComponent<CDamage>(2 * m_difficulty);
    bullet2->addComponent<CFollowPlayer>(entity->getComponent<CTransform>().pos, 2);
}

void Scene_Play::sCollision()
{
    
    // Implement entity - tile collisions
    // Implement player - enemy collisions with appropriate damage calculations
    // Implement Sword - NPC collisions
    // Implement black tile collisions / 'teleporting'
    // Implement entity - heart collisions and life gain logic
    //
    // You may want to use helper functions for these behaviors or this function will get long 

    for (auto e : m_entityManager.getEntities("tile"))
    {
        for (auto b : m_entityManager.getEntities())
        {
            if (Physics::GetOverlap(e, b).x > 0 && Physics::GetOverlap(e, b).y > 0 && e->hasComponent<CBoundingBox>() && b->tag() != "tile" && b->tag() != "dec" && b->tag() != "bossarm" && b->tag() != "enemybullet")
            { 
                //if movement came from below
                if (Physics::GetPreviousOverlap(e, b).x > 0 && b->getComponent<CTransform>().pos.y > e->getComponent<CTransform>().pos.y)
                {

                    if (b->tag() == "bullet")
                    {
                        b->destroy();
                    }

                    b->getComponent<CTransform>().pos.y += Physics::GetOverlap(e, b).y + 5; // + 5 to deal with a bug where the player was sticking to platforms that were moving down
                    b->getComponent<CTransform>().velocity.y = 0;


                    if (b->hasComponent<CInput>())
                    {
                        b->getComponent<CInput>().canJump = false;
                    }
                }

                //if movement came from above
                else if (Physics::GetPreviousOverlap(e, b).x > 0 && b->getComponent<CTransform>().pos.y < e->getComponent<CTransform>().pos.y)
                {
                    if (b->tag() == "bullet")
                    {
                        b->destroy();
                    }

                    b->getComponent<CTransform>().pos.y -= Physics::GetOverlap(e, b).y;
                    b->getComponent<CTransform>().velocity.y = 0;

                    if (e->hasComponent<CPatrol>())
                    {
                        b->getComponent<CTransform>().pos.x += e->getComponent<CTransform>().velocity.x;
                        b->getComponent<CTransform>().velocity.x = 0;
                        b->getComponent<CTransform>().pos.y += e->getComponent<CTransform>().velocity.y;
                        b->getComponent<CTransform>().velocity.y = 0;
                    }

                    if (b->hasComponent<CInput>())
                    {
                        b->getComponent<CInput>().canJump = true;
                    }
                }

                //if movement came from the side
                else if (Physics::GetPreviousOverlap(e, b).y > 0)
                {
                    if (b->tag() == "bullet")
                    {
                        b->destroy();
                    }

                    if (b->getComponent<CTransform>().pos.x > e->getComponent<CTransform>().pos.x)
                    {
                        b->getComponent<CTransform>().pos.x += Physics::GetOverlap(e, b).x;
                    }
                    else
                    {
                        b->getComponent<CTransform>().pos.x -= Physics::GetOverlap(e, b).x;
                    }
                }
            }
        }
    }

    for (auto i : m_entityManager.getEntities("item"))
    {
        if (Physics::GetOverlap(i, m_player).x > 0 && Physics::GetOverlap(i, m_player).y > 0)
        {
            bool notFull = sAddItem(m_player, i->getComponent<CInventory>().items[0]);

            if (notFull == true)
            {
                i->destroy();
                m_game->playSound("PickupItem");
            }
        }
    }

    for (auto e : m_entityManager.getEntities("npc"))
    {
        for (auto b : m_entityManager.getEntities("weapon"))
        {

            if (Physics::GetOverlap(e, b).x > 0 && Physics::GetOverlap(e, b).y > 0 && b->hasComponent<CBoundingBox>())
            {
                //e->destroy();

                // Deal damage to enemy
                e->getComponent<CHealth>().current -= b->getComponent<CDamage>().damage;
                
                // Temporary solution to a wacky problem. Using removeComponent() didn't actually remove the component for whatever reason, so we just set damage to 0
                b->getComponent<CDamage>().damage = 0;
                
                m_game->playSound("EnemyHurt");
            }
        }

        for (auto b : m_entityManager.getEntities("bullet"))
        {
            if (Physics::GetOverlap(e, b).x > 0 && Physics::GetOverlap(e, b).y > 0 && b->hasComponent<CBoundingBox>())
            {
                if (b->getComponent<CAnimation>().animation.getName() == "Arrow")
                {
                    b->destroy();
                }

                // Deal damage to enemy
                e->getComponent<CHealth>().current -= b->getComponent<CDamage>().damage;

                // Temporary solution to a wacky problem. Using removeComponent() didn't actually remove the component for whatever reason, so we just set damage to 0
                b->getComponent<CDamage>().damage = 0;

                m_game->playSound("EnemyHurt");
            }
        }

        for (auto b : m_entityManager.getEntities("player"))
        {
            if (Physics::GetOverlap(e, b).x > 0 && Physics::GetOverlap(e, b).y > 0 && b->hasComponent<CBoundingBox>() && !m_player->hasComponent<CInvincibility>())
            {
                b->getComponent<CHealth>().current -= e->getComponent<CDamage>().damage;

                b->addComponent<CInvincibility>(30);

                m_game->playSound("PlayerHurt");
            }
        }
    }

    for (auto e : m_entityManager.getEntities("dec"))
    {
        if (e->getComponent<CAnimation>().animation.getName() == "Exit")
        {
            if (Physics::GetOverlap(e, m_player).x > 0 && Physics::GetOverlap(e, m_player).y > 0 && e->hasComponent<CBoundingBox>())
            {
                //save player data
                if (m_levelPath == "level1.txt")
                {
                    unlocked[1] = true;
                }
                else if (m_levelPath == "level2.txt")
                {
                    unlocked[2] = true;
                }
                else if (m_levelPath == "level3.txt")
                {
                    unlocked[3] = true;
                }
                else if (m_levelPath == "level4.txt")
                {
                    unlocked[4] = true;
                }

                onEnd();
            }
        }

        if (e->getComponent<CAnimation>().animation.getName() == "Coin")
        {
            if (Physics::GetOverlap(e, m_player).x > 0 && Physics::GetOverlap(e, m_player).y > 0 && e->hasComponent<CBoundingBox>())
            {
                e->destroy();
                levelMoney++;
            }
        }
    }

    for (auto e : m_entityManager.getEntities("button"))
    {
        if (Physics::GetOverlap(e, m_player).x > 0 && Physics::GetOverlap(e, m_player).y > 0 && e->hasComponent<CBoundingBox>())
        {
            for (int i = 1; i < e->getComponent<CWeapons>().unlocked.size(); i++)
            {
                if (e->getComponent<CWeapons>().unlocked[i] == true)
                {
                    m_player->getComponent<CWeapons>().unlockWeapon(i);
                    e->destroy();
                    break;
                }
            }
        }
    }

    for (auto e : m_entityManager.getEntities("bossarm"))
    {
        for (auto b : m_entityManager.getEntities("weapon"))
        {

            if (Physics::GetOverlap(e, b).x > 0 && Physics::GetOverlap(e, b).y > 0 && b->hasComponent<CBoundingBox>())
            {
                //e->destroy();

                // Deal damage to enemy
                for (auto x : m_entityManager.getEntities("boss"))
                {
                    x->getComponent<CHealth>().current -= b->getComponent<CDamage>().damage;
                }

                // Temporary solution to a wacky problem. Using removeComponent() didn't actually remove the component for whatever reason, so we just set damage to 0
                b->getComponent<CDamage>().damage = 0;

                m_game->playSound("EnemyHurt");
            }
        }

        for (auto b : m_entityManager.getEntities("bullet"))
        {
            if (Physics::GetOverlap(e, b).x > 0 && Physics::GetOverlap(e, b).y > 0 && b->hasComponent<CBoundingBox>())
            {
                b->destroy();

                for (auto x : m_entityManager.getEntities("boss"))
                {
                    x->getComponent<CHealth>().current -= b->getComponent<CDamage>().damage;
                }

                // Temporary solution to a wacky problem. Using removeComponent() didn't actually remove the component for whatever reason, so we just set damage to 0
                b->getComponent<CDamage>().damage = 0;

                m_game->playSound("EnemyHurt");
            }
        }

        for (auto b : m_entityManager.getEntities("player"))
        {
            if (Physics::GetOverlap(e, b).x > 0 && Physics::GetOverlap(e, b).y > 0 && b->hasComponent<CBoundingBox>() && !m_player->hasComponent<CInvincibility>())
            {
                b->getComponent<CHealth>().current -= e->getComponent<CDamage>().damage;

                b->addComponent<CInvincibility>(30);

                m_game->playSound("PlayerHurt");
            }
        }
    }

    for (auto e : m_entityManager.getEntities("killtile"))
    {
        for (auto b : m_entityManager.getEntities("player"))
        {
            if (Physics::GetOverlap(e, b).x > 0 && Physics::GetOverlap(e, b).y > 0 && b->hasComponent<CBoundingBox>() && !m_player->hasComponent<CInvincibility>())
            {
                b->getComponent<CHealth>().current -= 999;

                m_game->playSound("PlayerHurt");
            }
        }
    }

    for (auto e : m_entityManager.getEntities("enemybullet"))
    {
        for (auto b : m_entityManager.getEntities("player"))
        {
            if (Physics::GetOverlap(e, b).x > 0 && Physics::GetOverlap(e, b).y > 0 && b->hasComponent<CBoundingBox>() && !m_player->hasComponent<CInvincibility>())
            {
                b->getComponent<CHealth>().current -= e->getComponent<CDamage>().damage;

                b->addComponent<CInvincibility>(30);

                m_game->playSound("PlayerHurt");
            }
        }

        for (auto b : m_entityManager.getEntities("tile"))
        {
            if (Physics::GetOverlap(e, b).x > 0 && Physics::GetOverlap(e, b).y > 0 && b->hasComponent<CBoundingBox>() && !m_player->hasComponent<CInvincibility>())
            {
                b->destroy();
                e->destroy();
            }
        }

        for (auto b : m_entityManager.getEntities("weapon"))
        {
            if (Physics::GetOverlap(e, b).x > 0 && Physics::GetOverlap(e, b).y > 0 && b->hasComponent<CBoundingBox>() && !m_player->hasComponent<CInvincibility>())
            {
                e->destroy();
            }
        }
    }
}

//Helper function for determining collision between tiles and NPCs OR Players
void Scene_Play::doCollide(std::shared_ptr<Entity> e, std::shared_ptr<Entity> b)
{
    if (Physics::GetOverlap(e, b).x > 0 && Physics::GetOverlap(e, b).y > 0 && e->hasComponent<CBoundingBox>())
    {
        //if movement came from below
        if (Physics::GetPreviousOverlap(e, b).x > 0 && b->getComponent<CTransform>().pos.y > e->getComponent<CTransform>().pos.y)
        {
            b->getComponent<CTransform>().pos.y += Physics::GetOverlap(e, b).y;
            b->getComponent<CTransform>().velocity.y = 0;
        }

        //if movement came from above
        else if (Physics::GetPreviousOverlap(e, b).x > 0 && b->getComponent<CTransform>().pos.y < e->getComponent<CTransform>().pos.y)
        {
            b->getComponent<CTransform>().pos.y -= Physics::GetOverlap(e, b).y;
            b->getComponent<CTransform>().velocity.y = 0;
        }

        //if movement came from the side
        else if (Physics::GetPreviousOverlap(e, b).y > 0)
        {
            if (b->getComponent<CTransform>().pos.x > e->getComponent<CTransform>().pos.x)
            {
                b->getComponent<CTransform>().pos.x += Physics::GetOverlap(e, b).x;
            }
            else
            {
                b->getComponent<CTransform>().pos.x -= Physics::GetOverlap(e, b).x;
            }

        }
    }
}


//Add an item to the entity's inventory.
bool Scene_Play::sAddItem(std::shared_ptr<Entity> entity, std::shared_ptr<Item> item)
{
    if (entity->hasComponent<CInventory>())
    {
        if (entity->getComponent<CInventory>().currentSize != entity->getComponent<CInventory>().maxSize)
        {
            entity->getComponent<CInventory>().items.push_back(item);
            entity->getComponent<CInventory>().currentSize++;
            return true;
        }
    }
    else
    {
        return false;
    }
}

//Uses an item in the entity's inventory.
void Scene_Play::sUseItem(std::shared_ptr<Entity> entity, std::shared_ptr<Item> item)
{
    if (entity->hasComponent<CInventory>())
    {
        for (int i = 0; i < entity->getComponent<CInventory>().items.size(); i++)
        {
            if (entity->getComponent<CInventory>().items[i]->itemID == item->itemID)
            {
                std::cout << "Used Item: " << entity->getComponent<CInventory>().items[i]->itemID << "\n";
                std::cout << "Used item: " << entity->getComponent<CInventory>().items[i]->tag << std::endl;
                item->use(entity);
                entity->getComponent<CInventory>().items.erase(entity->getComponent<CInventory>().items.begin() + i);
                entity->getComponent<CInventory>().currentSize--;
                break;
            }
        }
    }
    else
        std::cout << "Item not found!\n";
}




void Scene_Play::sAnimation()
{
    
    // Implement player facing direction animation
    // Implement weapon animation based on player facing
    //    The weapon should move if the player changes direction mid swing
    // Implement destruction of entities with non-repeating finished animations

    auto& pTransform = m_player->getComponent<CTransform>();
    auto& pState = m_player->getComponent<CState>();
    auto& pInput = m_player->getComponent<CInput>();
    auto& pWeapon = m_player->getComponent<CWeapons>().getWeapon();
    

    if (pWeapon.tag == "Basic")
    {
        // sets the animation of the player based on its CState component
        if (m_player->getComponent<CState>().state == "running" && m_player->getComponent<CAnimation>().animation.getName() != "RunMan")
        {
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("RunMan"), true);
        }
        else if (m_player->getComponent<CState>().state == "jumping")
        {
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("JumpUpMan"), true);
        }
        else if (m_player->getComponent<CState>().state == "standing" && m_player->getComponent<CAnimation>().animation.getName() != "IdleMan")
        {
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("IdleMan"), true);
        }
    }

    if (pWeapon.tag == "Warrior")
    {
        // sets the animation of the player based on its CState component
        if (m_player->getComponent<CState>().state == "running" && m_player->getComponent<CAnimation>().animation.getName() != "RunWar")
        {
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("RunWar"), true);
        }
        else if (m_player->getComponent<CState>().state == "jumping")
        {
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("JumpWar"), true);
        }
        else if (m_player->getComponent<CState>().state == "standing" && m_player->getComponent<CAnimation>().animation.getName() != "IdleWar")
        {
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("IdleWar"), true);
        }
    }

    if (pWeapon.tag == "Ranger")
    {
        // sets the animation of the player based on its CState component
        if (m_player->getComponent<CState>().state == "running" && m_player->getComponent<CAnimation>().animation.getName() != "RunRang")
        {
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("RunRang"), true);
        }
        else if (m_player->getComponent<CState>().state == "jumping")
        {
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("JumpRang"), true);
        }
        else if (m_player->getComponent<CState>().state == "standing" && m_player->getComponent<CAnimation>().animation.getName() != "IdleRang")
        {
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("IdleRang"), true);
        }
    }

    if (pWeapon.tag == "Mage")
    {
        // sets the animation of the player based on its CState component
        if (m_player->getComponent<CState>().state == "running" && m_player->getComponent<CAnimation>().animation.getName() != "RunWiz")
        {
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("RunWiz"), true);
        }
        else if (m_player->getComponent<CState>().state == "jumping")
        {
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("JumpWiz"), true);
        }
        else if (m_player->getComponent<CState>().state == "standing" && m_player->getComponent<CAnimation>().animation.getName() != "IdleWiz")
        {
            m_player->addComponent<CAnimation>(m_game->assets().getAnimation("IdleWiz"), true);
        }
    }
    

    // for each entity with an animation, call entity->getComponent<CAnimation>().animation.update()
    // if the animation is not repeated, and it has ended, destroy the entity.
    for (auto e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CAnimation>())
        {
            e->getComponent<CAnimation>().animation.update();

            if (e->getComponent<CAnimation>().animation.hasEnded() == true && e->getComponent<CAnimation>().repeat == false)
            {
                e->destroy();
            }
        }
    }
   
    /*
    *     if (m_player->getComponent<CState>().state == "runningSide" && m_player->getComponent<CAnimation>().animation.getName() != "RunRight")
    {
        m_player->addComponent<CAnimation>(m_game->assets().getAnimation("RunRight"), true);
    }
    else if (m_player->getComponent<CState>().state == "runningUp" && m_player->getComponent<CAnimation>().animation.getName() != "RunUp")
    {
        m_player->addComponent<CAnimation>(m_game->assets().getAnimation("RunUp"), true);
    }
    else if (m_player->getComponent<CState>().state == "runningDown" && m_player->getComponent<CAnimation>().animation.getName() != "RunDown")
    {
        m_player->addComponent<CAnimation>(m_game->assets().getAnimation("RunDown"), true);
    }
    else if (m_player->getComponent<CState>().state == "idleSide" && m_player->getComponent<CAnimation>().animation.getName() != "StandRight")
    {
        m_player->addComponent<CAnimation>(m_game->assets().getAnimation("StandRight"), true);
    }
    else if (m_player->getComponent<CState>().state == "idleUp" && m_player->getComponent<CAnimation>().animation.getName() != "StandUp")
    {
        m_player->addComponent<CAnimation>(m_game->assets().getAnimation("StandUp"), true);
    }
    else if (m_player->getComponent<CState>().state == "idleDown" && m_player->getComponent<CAnimation>().animation.getName() != "StandDown")
    {
        m_player->addComponent<CAnimation>(m_game->assets().getAnimation("StandDown"), true);
    }

    for (auto e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CAnimation>())
        {
            e->getComponent<CAnimation>().animation.update();

            if (e->getComponent<CAnimation>().animation.hasEnded() == true && e->getComponent<CAnimation>().repeat == false)
            {
                e->destroy();
            }
        }
    }
    */

    for (auto& sword : m_entityManager.getEntities("weapon"))
    {
        Vec2 swordPos = pTransform.pos;

        Vec2 offset = Vec2(64.0f, 64.0f);

        if (pTransform.facing.x < 0.0f)
        {
            sword->addComponent<CAnimation>(m_game->assets().getAnimation(pWeapon.animationName), true);
            sword->getComponent<CTransform>().scale.x = -1.0f;
            swordPos.x += -1 * offset.x;

            if (pInput.attack == true)
            {
                m_player->addComponent<CAnimation>(m_game->assets().getAnimation("AtkRight"), true);
            }
        }
        else
        {
            sword->addComponent<CAnimation>(m_game->assets().getAnimation(pWeapon.animationName), true);
            sword->getComponent<CTransform>().scale.x = 1.0f;
            swordPos.x += offset.x;

            if (pInput.attack == true)
            {
                m_player->addComponent<CAnimation>(m_game->assets().getAnimation("AtkRight"), true);
            }
        }

        sword->getComponent<CTransform>().pos = swordPos;

    }
}

void Scene_Play::sCamera()
{
    
    // Implement camera view logic

    // get the current view, which we will modify in the if-statement below
    sf::View view = m_game->window().getView();

    auto& pPos = m_player->getComponent<CTransform>().pos;
                                      
    if (m_follow)
    {
        view.setCenter(pPos.x, pPos.y);
    }
    else if (m_box)
    {
        float PositiveBoundaryX = view.getCenter().x + 300.0f;
        float PositiveBoundaryY = view.getCenter().y + 200.0f;

        float NegativeBoundaryX = view.getCenter().x - 300.0f;
        float NegativeBoundaryY = view.getCenter().y - 200.0f;

        if (pPos.x > PositiveBoundaryX)
        {
            view.setCenter(view.getCenter().x + (pPos.x - PositiveBoundaryX), view.getCenter().y);
        }

        if (pPos.x < NegativeBoundaryX)
        {
            view.setCenter(view.getCenter().x + (pPos.x - NegativeBoundaryX), view.getCenter().y);
        }

        if (pPos.y > PositiveBoundaryY)
        {
            view.setCenter(view.getCenter().x, view.getCenter().y + (pPos.y - PositiveBoundaryY));
        }

        if (pPos.y < NegativeBoundaryY)
        {
            view.setCenter(view.getCenter().x, view.getCenter().y + (pPos.y - NegativeBoundaryY));
        }
    }
    else
    {
        // calcluate view for room-based camera
        int gridX = (pPos.x / 1280);
        int gridY = (pPos.y / 768);
        
        if (pPos.x < 0)
        {
            gridX = gridX - 1;
        }

        if (pPos.y < 0)
        {
            gridY = gridY - 1;
        }

        float viewCenterX = (gridX * 1280.0f) + (1280.0f / 2);
        float viewCenterY = (gridY * 768.0f) + (768.0f / 2);

        view.setCenter(viewCenterX, viewCenterY);

        

    }

    // then set the window view
    m_game->window().setView(view);
}

void Scene_Play::onEnd()
{
    
    // Implement scene end
    // - stop the music
    // - play the menu music
    // - change scene to menu

    saveData(m_saveData);
    m_game->assets().getSound("Music").stop();
    m_game->changeScene("VICTORY", std::make_shared<Scene_Victory>(m_game), true);
    
}

void Scene_Play::onDie()
{
    m_game->assets().getSound("Music").stop();
    m_game->changeScene("DEFEAT", std::make_shared<Scene_GameOver>(m_game), true);
}

void Scene_Play::sRender()
{
    // RENDERING DONE FOR YOU

    m_game->window().clear();
    sf::RectangleShape tick({ 1.0f, 6.0f });
    tick.setFillColor(sf::Color::Black);

    // draw all Entity textures / animations
    if (m_drawTextures)
    {



        // draw background
        Vec2 focal_point_speed = m_player->getComponent<CTransform>().velocity;
        Vec2 focal_point_pos = Vec2(m_game->window().getView().getCenter().x, m_game->window().getView().getCenter().y);
        //Vec2 focal_point_pos = m_player->getComponent<CTransform>().pos;

        sf::Texture background(m_game->assets().getTexture("TexBackL1"));
        sf::Sprite backgroundSprite(background);
        backgroundSprite.setPosition(m_game->window().getView().getCenter().x - 640 - (0.10f * focal_point_pos.x), m_game->window().getView().getCenter().y - 384);


        sf::Texture background2(m_game->assets().getTexture("TexBackL1"));
        sf::Sprite backgroundSprite2(background2);
        backgroundSprite2.setPosition(m_game->window().getView().getCenter().x - 640 + (m_game->window().getView().getSize().x) - 0.10f * focal_point_pos.x, m_game->window().getView().getCenter().y - 384);

        sf::Texture background3(m_game->assets().getTexture("TexBackL2"));
        sf::Sprite backgroundSprite3(background3);
        backgroundSprite3.setPosition(m_game->window().getView().getCenter().x - 640 - (0.20f * focal_point_pos.x), m_game->window().getView().getCenter().y - 384);


        sf::Texture background4(m_game->assets().getTexture("TexBackL2"));
        sf::Sprite backgroundSprite4(background4);
        backgroundSprite4.setPosition(m_game->window().getView().getCenter().x - 640 + (m_game->window().getView().getSize().x) - 0.20f * focal_point_pos.x, m_game->window().getView().getCenter().y - 384);

        sf::Texture background5(m_game->assets().getTexture("TexBackL3"));
        sf::Sprite backgroundSprite5(background5);
        backgroundSprite5.setPosition(m_game->window().getView().getCenter().x - 640 - (0.30f * focal_point_pos.x), m_game->window().getView().getCenter().y - 384);

        sf::Texture background6(m_game->assets().getTexture("TexBackL3"));
        sf::Sprite backgroundSprite6(background6);
        backgroundSprite6.setPosition(m_game->window().getView().getCenter().x - 640 + (m_game->window().getView().getSize().x) - 0.30f * focal_point_pos.x, m_game->window().getView().getCenter().y - 384);

        sf::Texture background7(m_game->assets().getTexture("TexBackL4"));
        sf::Sprite backgroundSprite7(background7);
        backgroundSprite7.setPosition(m_game->window().getView().getCenter().x - 640 - (0.40f * focal_point_pos.x), m_game->window().getView().getCenter().y - 384);

        sf::Texture background8(m_game->assets().getTexture("TexBackL4"));
        sf::Sprite backgroundSprite8(background8);
        backgroundSprite8.setPosition(m_game->window().getView().getCenter().x - 640 + (m_game->window().getView().getSize().x) - 0.40f * focal_point_pos.x, m_game->window().getView().getCenter().y - 384);

        float differencebg1 = backgroundSprite.getPosition().x - m_game->window().getView().getCenter().x;
        float differencebg2 = backgroundSprite2.getPosition().x - m_game->window().getView().getCenter().x;
        float differencebg3 = backgroundSprite3.getPosition().x - m_game->window().getView().getCenter().x;
        float differencebg4 = backgroundSprite4.getPosition().x - m_game->window().getView().getCenter().x;
        float differencebg5 = backgroundSprite5.getPosition().x - m_game->window().getView().getCenter().x;
        float differencebg6 = backgroundSprite6.getPosition().x - m_game->window().getView().getCenter().x;
        float differencebg7 = backgroundSprite7.getPosition().x - m_game->window().getView().getCenter().x;
        float differencebg8 = backgroundSprite8.getPosition().x - m_game->window().getView().getCenter().x;

        if (fabs(differencebg1) > (background.getSize().x + (m_game->window().getView().getSize().x / 2)) && (differencebg1 < 0))
        {
            backgroundSprite.setPosition(backgroundSprite.getPosition().x + (2.0f * background.getSize().x), backgroundSprite.getPosition().y);
        }

        else if (fabs(differencebg1) > (m_game->window().getView().getSize().x / 2) && (differencebg1 > 0))
        {
            backgroundSprite.setPosition(backgroundSprite.getPosition().x - (2.0f * background.getSize().x), backgroundSprite.getPosition().y);
        }

        if (fabs(differencebg2) > (m_game->window().getView().getSize().x / 2) && (differencebg2 > 0))
        {
            backgroundSprite2.setPosition(backgroundSprite2.getPosition().x - (2.0f * background2.getSize().x), backgroundSprite2.getPosition().y);
        }

        else if (fabs(differencebg2) > (background2.getSize().x + (m_game->window().getView().getSize().x / 2)) && (differencebg2 < 0))
        {
            backgroundSprite2.setPosition(backgroundSprite2.getPosition().x + (2.0f * background2.getSize().x), backgroundSprite2.getPosition().y);
        }

        if (fabs(differencebg3) > (background3.getSize().x + (m_game->window().getView().getSize().x / 2)) && (differencebg3 < 0))
        {
            backgroundSprite3.setPosition(backgroundSprite3.getPosition().x + (2.0f * background3.getSize().x), backgroundSprite3.getPosition().y);
        }

        else if (fabs(differencebg3) > (m_game->window().getView().getSize().x / 2) && (differencebg3 > 0))
        {
            backgroundSprite3.setPosition(backgroundSprite3.getPosition().x - (2.0f * background3.getSize().x), backgroundSprite3.getPosition().y);
        }

        if (fabs(differencebg4) > (m_game->window().getView().getSize().x / 2) && (differencebg4 > 0))
        {
            backgroundSprite4.setPosition(backgroundSprite4.getPosition().x - (2.0f * background4.getSize().x), backgroundSprite4.getPosition().y);
        }

        else if (fabs(differencebg4) > (background4.getSize().x + (m_game->window().getView().getSize().x / 2)) && (differencebg4 < 0))
        {
            backgroundSprite4.setPosition(backgroundSprite4.getPosition().x + (2.0f * background4.getSize().x), backgroundSprite4.getPosition().y);
        }

        if (fabs(differencebg5) > (background5.getSize().x + (m_game->window().getView().getSize().x / 2)) && (differencebg5 < 0))
        {
            backgroundSprite5.setPosition(backgroundSprite5.getPosition().x + (2.0f * background5.getSize().x), backgroundSprite5.getPosition().y);
        }

        else if (fabs(differencebg5) > (m_game->window().getView().getSize().x / 2) && (differencebg5 > 0))
        {
            backgroundSprite5.setPosition(backgroundSprite5.getPosition().x - (2.0f * background5.getSize().x), backgroundSprite5.getPosition().y);
        }

        if (fabs(differencebg6) > (m_game->window().getView().getSize().x / 2) && (differencebg6 > 0))
        {
            backgroundSprite6.setPosition(backgroundSprite6.getPosition().x - (2.0f * background6.getSize().x), backgroundSprite6.getPosition().y);
        }

        else if (fabs(differencebg6) > (background6.getSize().x + (m_game->window().getView().getSize().x / 2)) && (differencebg6 < 0))
        {
            backgroundSprite6.setPosition(backgroundSprite6.getPosition().x + (2.0f * background6.getSize().x), backgroundSprite6.getPosition().y);
        }

        if (fabs(differencebg7) > (background7.getSize().x + (m_game->window().getView().getSize().x / 2)) && (differencebg7 < 0))
        {
            backgroundSprite7.setPosition(backgroundSprite7.getPosition().x + (2.0f * background7.getSize().x), backgroundSprite7.getPosition().y);
        }

        else if (fabs(differencebg7) > (m_game->window().getView().getSize().x / 2) && (differencebg7 > 0))
        {
            backgroundSprite7.setPosition(backgroundSprite7.getPosition().x - (2.0f * background7.getSize().x), backgroundSprite7.getPosition().y);
        }

        if (fabs(differencebg8) > (m_game->window().getView().getSize().x / 2) && (differencebg8 > 0))
        {
            backgroundSprite8.setPosition(backgroundSprite8.getPosition().x - (2.0f * background8.getSize().x), backgroundSprite8.getPosition().y);
        }

        else if (fabs(differencebg8) > (background8.getSize().x + (m_game->window().getView().getSize().x / 2)) && (differencebg8 < 0))
        {
            backgroundSprite8.setPosition(backgroundSprite8.getPosition().x + (2.0f * background8.getSize().x), backgroundSprite8.getPosition().y);
        }

        m_game->window().draw(backgroundSprite);
        m_game->window().draw(backgroundSprite2);
        m_game->window().draw(backgroundSprite3);
        m_game->window().draw(backgroundSprite4);
        m_game->window().draw(backgroundSprite5);
        m_game->window().draw(backgroundSprite6);
        m_game->window().draw(backgroundSprite7);
        m_game->window().draw(backgroundSprite8);


        // Draw foreground decs
        for (auto e : m_entityManager.getEntities("dec"))
        {
            if (e->getComponent<CLayer>().layer == 1)
            {
                auto& transform = e->getComponent<CTransform>();
                sf::Color c = sf::Color::White;

                if (e->hasComponent<CAnimation>())
                {
                    auto& animation = e->getComponent<CAnimation>().animation;
                    animation.getSprite().setRotation(transform.angle);
                    animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                    animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                    animation.getSprite().setColor(c);
                    m_game->window().draw(animation.getSprite());
                }
            }
        }

        // draw non-dec entity animations
        for (auto e : m_entityManager.getEntities())
        {
            if (e->tag() != "dec")
            {
                auto& transform = e->getComponent<CTransform>();
                sf::Color c = sf::Color::White;
                if (e->hasComponent<CInvincibility>())
                {
                    c = sf::Color(255, 255, 255, 128);
                }

                if (e->hasComponent<CAnimation>())
                {
                    auto& animation = e->getComponent<CAnimation>().animation;
                    animation.getSprite().setRotation(transform.angle);
                    animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                    animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                    animation.getSprite().setColor(c);
                    m_game->window().draw(animation.getSprite());
                }
            }
        }

        // Draw foreground decs
        for (auto e : m_entityManager.getEntities("dec"))
        {
            if (e->getComponent<CLayer>().layer == 2)
            {
                auto& transform = e->getComponent<CTransform>();
                sf::Color c = sf::Color::White;

                if (e->hasComponent<CAnimation>())
                {
                    auto& animation = e->getComponent<CAnimation>().animation;
                    animation.getSprite().setRotation(transform.angle);
                    animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                    animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                    animation.getSprite().setColor(c);
                    m_game->window().draw(animation.getSprite());
                }
            }
        }

        // draw entity health bars
        for (auto e : m_entityManager.getEntities())
        {
            auto& transform = e->getComponent<CTransform>();
            if (e->hasComponent<CHealth>())
            {
                auto& h = e->getComponent<CHealth>();
                Vec2 size(64, 6);
                sf::RectangleShape rect({ size.x, size.y });
                rect.setPosition(transform.pos.x - 32, transform.pos.y - 48);
                rect.setFillColor(sf::Color(96, 96, 96));
                rect.setOutlineColor(sf::Color::Black);
                rect.setOutlineThickness(2);
                m_game->window().draw(rect);

                float ratio = (float)(h.current) / h.max;
                size.x *= ratio;
                rect.setSize({ size.x, size.y });
                rect.setFillColor(sf::Color(255, 0, 0));
                rect.setOutlineThickness(0);
                m_game->window().draw(rect);

                for (int i = 0; i < h.max; i++)
                {
                    tick.setPosition(rect.getPosition() + sf::Vector2f(i * 64 * 1 / h.max, 0));
                    m_game->window().draw(tick);
                }
            }

            if (e->hasComponent<CCooldown>())
            {
                if (e->getComponent<CCooldown>().ready != true)
                {
                    auto& c = e->getComponent<CCooldown>();
                    Vec2 size(64, 6);
                    sf::RectangleShape rect({ size.x, size.y });
                    rect.setPosition(transform.pos.x - 32, transform.pos.y - 64);
                    rect.setFillColor(sf::Color(96, 96, 96));
                    rect.setOutlineColor(sf::Color::Black);
                    rect.setOutlineThickness(2);
                    m_game->window().draw(rect);

                    float ratio = (float)(c.curTimer) / c.maxTimer;
                    size.x *= ratio;
                    rect.setSize({ size.x , size.y });
                    rect.setFillColor(sf::Color(255, 255, 0));
                    rect.setOutlineThickness(0);
                    m_game->window().draw(rect);
                }
            }

            if (e->hasComponent<CBoss>())
            {
                if (e->getComponent<CBoss>().mechanicOccured == true)
                {
                    auto& c = e->getComponent<CBoss>();
                    Vec2 size(64, 6);
                    sf::RectangleShape rect({ size.x, size.y });
                    rect.setPosition(transform.pos.x - 32, transform.pos.y - 64);
                    rect.setFillColor(sf::Color(96, 96, 96));
                    rect.setOutlineColor(sf::Color::Black);
                    rect.setOutlineThickness(2);
                    m_game->window().draw(rect);

                    float ratio = (float)(c.currentTimer) / c.maxTimer;
                    size.x *= ratio;
                    rect.setSize({ size.x , size.y });
                    rect.setFillColor(sf::Color(148, 0, 211));
                    rect.setOutlineThickness(0);
                    m_game->window().draw(rect);
                }
            }
        }
    }

    if (m_drawInventory)
    {

        //Draw a rectangle to encompass the menu options

        float menuX = m_game->window().getView().getCenter().x - 630;
        float menuY = m_game->window().getView().getCenter().y - 374;

        sf::RectangleShape rect({ 300, 250 });
        rect.setPosition(menuX, menuY);
        rect.setFillColor(sf::Color::White);
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(2);
        m_game->window().draw(rect);

        int HPamount = 0;
        int IPamount = 0;
        int SPamount = 0;
        for (int i = 0; i < m_player->getComponent<CInventory>().items.size(); i++)
        {
            if (m_player->getComponent<CInventory>().items[i]->itemID == m_items[0]->itemID)
            {
                HPamount += 1;
            }
            if (m_player->getComponent<CInventory>().items[i]->itemID == m_items[1]->itemID)
            {
                IPamount += 1;
            }
            if (m_player->getComponent<CInventory>().items[i]->itemID == m_items[2]->itemID)
            {
                SPamount += 1;
            }
        }

        // draw all of the menu options
        for (size_t i = 0; i < m_inventoryStrings.size(); i++)
        {
            m_menuText.setString(m_inventoryStrings[i]);
            m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::Black : sf::Color(100, 100, 100));
            m_menuText.setPosition(sf::Vector2f(menuX + 10, menuY + 10 + i * 72));
            m_game->window().draw(m_menuText);
            if (i == 0)
            {
                m_menuText.setString(std::to_string(HPamount));
                m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::Black : sf::Color(100, 100, 100));
                m_menuText.setPosition(sf::Vector2f(menuX + 260, menuY + 10 + i * 72));
                m_game->window().draw(m_menuText);
            }
            if (i == 1)
            {
                m_menuText.setString(std::to_string(IPamount));
                m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::Black : sf::Color(100, 100, 100));
                m_menuText.setPosition(sf::Vector2f(menuX + 260, menuY + 10 + i * 72));
                m_game->window().draw(m_menuText);
            }
            if (i == 2)
            {
                m_menuText.setString(std::to_string(SPamount));
                m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::Black : sf::Color(100, 100, 100));
                m_menuText.setPosition(sf::Vector2f(menuX + 260, menuY + 10 + i * 72));
                m_game->window().draw(m_menuText);
            }

        }

    }

    // draw all Entity collision bounding boxes with a rectangleshape
    if (m_drawCollision)
    {
        sf::CircleShape dot(4);
        dot.setFillColor(sf::Color::Black);
        for (auto e : m_entityManager.getEntities())
        {
            if (e->hasComponent<CBoundingBox>())
            {
                auto& box = e->getComponent<CBoundingBox>();
                auto& transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                                      
                if (box.blockMove && box.blockVision) { rect.setOutlineColor(sf::Color::Black); }
                if (box.blockMove && !box.blockVision) { rect.setOutlineColor(sf::Color::Blue); }
                if (!box.blockMove && box.blockVision) { rect.setOutlineColor(sf::Color::Red); }
                if (!box.blockMove && !box.blockVision) { rect.setOutlineColor(sf::Color::White); }
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);
            }
                                      
            if (e->hasComponent<CPatrol>())
            {
                auto& patrol = e->getComponent<CPatrol>().positions;
                for (size_t p = 0; p < patrol.size(); p++)
                {
                    dot.setPosition(patrol[p].x, patrol[p].y);
                    m_game->window().draw(dot);
                }
            }

            if (e->hasComponent<CFollowPlayer>())
            {
                sf::VertexArray lines(sf::LinesStrip, 2);
                lines[0].position.x = e->getComponent<CTransform>().pos.x;
                lines[0].position.y = e->getComponent<CTransform>().pos.y;
                lines[0].color = sf::Color::Black;
                lines[1].position.x = m_player->getComponent<CTransform>().pos.x;
                lines[1].position.y = m_player->getComponent<CTransform>().pos.y;
                lines[1].color = sf::Color::Black;
                m_game->window().draw(lines);
                dot.setPosition(e->getComponent<CFollowPlayer>().home.x, e->getComponent<CFollowPlayer>().home.y);
                m_game->window().draw(dot);
            }
        }
    }
}


// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2021FALL - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
