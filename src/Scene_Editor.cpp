#include "Scene_Editor.h"

#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Scene_Menu.h"

Scene_Editor::Scene_Editor(GameEngine* game, const std::string& levelPath)
    : Scene(game)
    , m_levelPath(levelPath)
{
    init(m_levelPath);
}

void Scene_Editor::init(const std::string& levelPath)
{
    loadLevel(levelPath);

    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Y, "TOGGLE_FOLLOW");           // Toggle follow camera
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");          // Toggle drawing (T)extures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");        // Toggle drawing (C)ollision Boxes
    registerAction(sf::Keyboard::W, "MOVE_UP");                 // Move Up
    registerAction(sf::Keyboard::A, "MOVE_LEFT");               // Move Left
    registerAction(sf::Keyboard::S, "MOVE_DOWN");               // Move Down
    registerAction(sf::Keyboard::D, "MOVE_RIGHT");              // Move Right    

    registerAction(sf::Mouse::Left, "PLACE");                   // Attack
    registerAction(sf::Mouse::Right, "REMOVE");                 // Use Item From Inventory (TEMP)
    registerAction(sf::Keyboard::Left, "SWAPFWD");              // Swap Currently Equipped Weapon
    registerAction(sf::Keyboard::Right, "SWAPBWD");             // For testing purposes, prints a value to console.

    std::srand(static_cast<unsigned int>(std::time(NULL)));
}

void Scene_Editor::loadLevel(const std::string& filename)
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
        }
    }

    // Load the level file and put all entities in the manager
    // Use the getPosition() function below to convert room-tile coords to game world coords
}

Vec2 Scene_Editor::getPosition(int rx, int ry, int tx, int ty) const
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

void Scene_Editor::saveLevel(const std::string& filename)
{
}

void Scene_Editor::onEnd()
{
}

void Scene_Editor::update()
{
}

void Scene_Editor::sMovement()
{
}

void Scene_Editor::sCamera()
{
}

void Scene_Editor::sRender()
{
}

void Scene_Editor::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {

        if (action.name() == "PAUSE") { setPaused(!m_paused); }
        else if (action.name() == "QUIT") { onEnd(); }
        else if (action.name() == "TOGGLE_FOLLOW") { m_follow = !m_follow; }
        else if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
        else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }

        else if (action.name() == "MOVE_UP")    { m_player->getComponent<CInput>().up = true; }
        else if (action.name() == "MOVE_LEFT")  { m_player->getComponent<CInput>().left = true; }
        else if (action.name() == "MOVE_DOWN") { m_player->getComponent<CInput>().down = true; }
        else if (action.name() == "MOVE_RIGHT") { m_player->getComponent<CInput>().right = true; }

        else if (action.name() == "PLACE") {}
        else if (action.name() == "REMOVE") {}
        else if (action.name() == "SWAPFWD") {}
        else if (action.name() == "SWAPBWD") {}
    }
    else if (action.type() == "END")
    {
        if (action.name() == "JUMP") { m_player->getComponent<CInput>().up = false; }
        else if (action.name() == "MOVE_LEFT") { m_player->getComponent<CInput>().left = false; }
        else if (action.name() == "MOVE_DOWN") { m_player->getComponent<CInput>().down = false; }
        else if (action.name() == "MOVE_RIGHT") { m_player->getComponent<CInput>().right = false; }
    }
}
