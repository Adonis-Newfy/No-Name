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
    loadLevel(levelPath);

    
    // Register the actions required to play the game

    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Y, "TOGGLE_FOLLOW");       // Toggle follow camera
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");      // Toggle drawing (T)extures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");    // Toggle drawing (C)ollision Boxes
    registerAction(sf::Keyboard::W, "JUMP");                // Jump
    registerAction(sf::Keyboard::A, "MOVE_LEFT");           // Move Left
    registerAction(sf::Keyboard::D, "MOVE_RIGHT");          // Move Right    
    
    registerAction(sf::Keyboard::Space, "ATTACK");          // Attack
    registerAction(sf::Keyboard::I, "ITEM");                // Use Item From Inventory (TEMP)
    registerAction(sf::Keyboard::Tab, "SWAP");              // Swap Currently Equipped Weapon
    registerAction(sf::Keyboard::V, "TEST");                // For testing purposes, prints a value to console.

    std::srand(static_cast<unsigned int>(std::time(NULL)));
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
            m_playerConfig.HEALTH = health;
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
                npc->addComponent<CDamage>(damage);
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
                npc->addComponent<CDamage>(damage);
                npc->addComponent<CFollowPlayer>(getPosition(roomX, roomY, tileX, tileY), speed);
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
    m_game->playSound("MusicLevel");
}

void Scene_Play::saveLevel(const std::string& filename)
{
    std::ofstream file(filename);
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
    m_player->addComponent<CHealth>(m_playerConfig.HEALTH, m_playerConfig.HEALTH - 1);
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
            weapon->addComponent<CDamage>(pWeapon.damage * m_player->getComponent<CBuffed>().multiplier);

            m_player->addComponent<CCooldown>(pWeapon.delay);

            m_game->playSound("Slash");
            pState.state = "attacking";
        }
        else if (pWeapon.ranged == true)
        {
            Vec2 bulletPos = entity->getComponent<CTransform>().pos;

            auto bullet = m_entityManager.addEntity("bullet");

            bullet->addComponent<CAnimation>(m_game->assets().getAnimation(pWeapon.animationName), true);
            bullet->addComponent<CTransform>(bulletPos);
            bullet->addComponent<CBoundingBox>(bullet->getComponent<CAnimation>().animation.getSize());
            bullet->addComponent<CLifeSpan>(60, m_currentFrame);
            bullet->addComponent<CDamage>(pWeapon.damage * m_player->getComponent<CBuffed>().multiplier);


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
    /*
    auto& testVal = m_player->getComponent<CWeapons>().getWeapon().animationName;

    std::cout << testVal << std::endl;
    */


    const std::string filepath = "test.txt";

    saveLevel(filepath);
}

void Scene_Play::update()
{
    m_entityManager.update();
                                      
    
    // Implement pause functionality
       
    if (!m_paused)
    {
        sAI();
        sMovement();
        sStatus();
        sCollision();
        sAnimation();
        sCamera();
        // sLighting();
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
        else if (action.name() == "TOGGLE_FOLLOW") { m_follow = !m_follow; }
        else if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
        else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
        else if (action.name() == "JUMP") { m_player->getComponent<CInput>().up = true; }
        else if (action.name() == "MOVE_LEFT") { m_player->getComponent<CInput>().left = true; }
        else if (action.name() == "MOVE_RIGHT") { m_player->getComponent<CInput>().right = true; }
        else if (action.name() == "ATTACK") { spawnWeapon(m_player);  if (m_player->getComponent<CCooldown>().ready == true) m_player->getComponent<CInput>().attack = true; }
        else if (action.name() == "ITEM") { sUseItem(m_player, m_player->getComponent<CInventory>().items[0]); } //Uses the first item in the player's inventory
        else if (action.name() == "SWAP") { sSwapWeapon(); }
        else if (action.name() == "TEST") { sTestValue(); }
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
        if (e->tag() == "npc" || e->tag() == "tile")
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


                for (auto b : m_entityManager.getEntities())
                {
                    if (b->hasComponent<CBoundingBox>() && b->getComponent<CBoundingBox>().blockVision == true)
                    {
                        if (Physics::EntityIntersect(m_player->getComponent<CTransform>().pos, e->getComponent<CTransform>().pos, b))
                        {
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
                    e->destroy();
                    m_game->playSound("EnemyDie");
                }
            }
            else
            {
                if (e->getComponent<CHealth>().current <= 0)
                {
                    e->destroy();
                    spawnPlayer();
                    m_game->playSound("LinkDie");
                }
            }
        }
    }
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
            if (Physics::GetOverlap(e, b).x > 0 && Physics::GetOverlap(e, b).y > 0 && e->hasComponent<CBoundingBox>() && b->tag() != "tile")
            { 
                //if movement came from below
                if (Physics::GetPreviousOverlap(e, b).x > 0 && b->getComponent<CTransform>().pos.y > e->getComponent<CTransform>().pos.y)
                {
                    b->getComponent<CTransform>().pos.y += Physics::GetOverlap(e, b).y;
                    b->getComponent<CTransform>().velocity.y = 0;

                    if (b->hasComponent<CInput>())
                    {
                        b->getComponent<CInput>().canJump = false;
                    }
                }

                //if movement came from above
                else if (Physics::GetPreviousOverlap(e, b).x > 0 && b->getComponent<CTransform>().pos.y < e->getComponent<CTransform>().pos.y)
                {
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
                m_game->playSound("GetItem");
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
                
                m_game->playSound("EnemyHit");
            }
        }

        for (auto b : m_entityManager.getEntities("bullet"))
        {
            if (Physics::GetOverlap(e, b).x > 0 && Physics::GetOverlap(e, b).y > 0 && b->hasComponent<CBoundingBox>())
            {
                b->destroy();

                // Deal damage to enemy
                e->getComponent<CHealth>().current -= b->getComponent<CDamage>().damage;

                // Temporary solution to a wacky problem. Using removeComponent() didn't actually remove the component for whatever reason, so we just set damage to 0
                b->getComponent<CDamage>().damage = 0;

                m_game->playSound("EnemyHit");
            }
        }

        for (auto b : m_entityManager.getEntities("player"))
        {
            if (Physics::GetOverlap(e, b).x > 0 && Physics::GetOverlap(e, b).y > 0 && b->hasComponent<CBoundingBox>() && !m_player->hasComponent<CInvincibility>())
            {
                b->getComponent<CHealth>().current -= e->getComponent<CDamage>().damage;

                b->addComponent<CInvincibility>(30);

                m_game->playSound("LinkHurt");
            }
        }
    }
    /*

    */
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
    
    // sets the animation of the player based on its CState component
    if (m_player->getComponent<CState>().state == "running" && m_player->getComponent<CAnimation>().animation.getName() != "RunRight")
    {
        m_player->addComponent<CAnimation>(m_game->assets().getAnimation("RunRight"), true);
    }
    else if (m_player->getComponent<CState>().state == "jumping")
    {
        m_player->addComponent<CAnimation>(m_game->assets().getAnimation("RunRight"), true);
    }
    else if (m_player->getComponent<CState>().state == "standing")
    {
        m_player->addComponent<CAnimation>(m_game->assets().getAnimation("StandRight"), true);
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

    m_game->assets().getSound("MusicLevel").stop();
    m_game->playSound("MusicTitle");
    m_game->changeScene("MENU", nullptr, true);
    
}

void Scene_Play::sRender()
{
    // RENDERING DONE FOR YOU

    m_game->window().clear(sf::Color(255, 192, 122));
    sf::RectangleShape tick({ 1.0f, 6.0f });
    tick.setFillColor(sf::Color::Black);

    // draw all Entity textures / animations
    if (m_drawTextures)
    {
        // draw entity animations
        for (auto e : m_entityManager.getEntities())
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
