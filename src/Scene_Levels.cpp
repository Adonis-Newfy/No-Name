#include "Scene_Menu.h"
#include "Scene_Play.h"
#include "Scene_Levels.h"
#include "Common.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"

Scene_Levels::Scene_Levels(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}

void Scene_Levels::init()
{

    unlocked.push_back(true);

    for (int i = 1; i < 5; i++)
    {
        unlocked.push_back(false);
    }

    registerAction(sf::Keyboard::A, "UP");
    registerAction(sf::Keyboard::D, "DOWN");
    registerAction(sf::Keyboard::W, "ZELDA");
    registerAction(sf::Keyboard::Escape, "QUIT");

    m_title = "No Name";
    m_menuStrings.push_back("Entrance");
    m_menuStrings.push_back("Depths B1");
    m_menuStrings.push_back("Depths B2");
    m_menuStrings.push_back("Final Boss");
    m_menuStrings.push_back("Testing");
    m_menuStrings.push_back("Back");

    m_levelPaths.push_back("level1.txt");
    m_levelPaths.push_back("level2.txt");
    m_levelPaths.push_back("level3.txt");
    m_levelPaths.push_back("level4.txt");
    m_levelPaths.push_back("level5.txt");

    m_menuText.setFont(m_game->assets().getFont("Golem"));
    m_menuText.setCharacterSize(64);

    loadLevel("LevelSelect.txt");

    sf::View view = m_game->window().getView();
    view.setCenter(getPosition(0, 0, 7, 10).x, getPosition(0, 0, 7, 10).y - 128);
    view.zoom(0.5);
    m_game->window().setView(view);

    loadData(m_saveData);
}

void Scene_Levels::loadLevel(const std::string& filename)
{
    m_entityManager = EntityManager();
    std::ifstream fin(filename);

    std::string text = "";

    int count = 1;



    while (fin >> text)
    {
        std::cout << text << "\n";

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

        if (text == "Dec")
        {
            std::string animationName = "";
            int roomX;
            int roomY;
            int tileX;
            int tileY;
            int layer;

            fin >> animationName >> roomX >> roomY >> tileX >> tileY >> layer;

            auto dec = m_entityManager.addEntity("dec");
            dec->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
            dec->addComponent<CTransform>(getPosition(roomX, roomY, tileX, tileY));
            dec->addComponent<CLayer>(layer);
        }
    }

    auto playerSprite = m_entityManager.addEntity("player");
    playerSprite->addComponent<CAnimation>(m_game->assets().getAnimation("IdleMan"), true);
    playerSprite->addComponent<CTransform>(getPosition(0, 0, 3, 10) - Vec2(20, 0));

    // Load the level file and put all entities in the manager
    // Use the getPosition() function below to convert room-tile coords to game world coords
}

void Scene_Levels::loadData(const std::string& filename)
{
    std::ifstream fin(filename);
    std::string text;

    while (fin >> text)
    {

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
    }
}
Vec2 Scene_Levels::getPosition(int rx, int ry, int tx, int ty) const
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

void Scene_Levels::update()
{
    m_entityManager.update();
    for (auto e : m_entityManager.getEntities("player"))
    {
        e->getComponent<CAnimation>().animation.update();
    }
}

void Scene_Levels::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "UP")
        {
            if (m_selectedMenuIndex > 0) { m_selectedMenuIndex--; }
            else { m_selectedMenuIndex = m_menuStrings.size() - 1; }
        }
        else if (action.name() == "DOWN")
        {
            m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
        }
        else if (action.name() == "ZELDA")
        {
            if (m_selectedMenuIndex < 5)
            {
                if (unlocked[m_selectedMenuIndex] == true)
                {
                    m_game->changeScene("GAME", std::make_shared<Scene_Play>(m_game, m_levelPaths[m_selectedMenuIndex]), true);
                    m_game->assets().getSound("MusicTitle").stop();
                }
            }

            if (m_menuStrings[m_selectedMenuIndex] == "Back")
            {
                onEnd();
            }
            
        }
        else if (action.name() == "QUIT")
        {
            onEnd();
        }
    }
}

void Scene_Levels::sRender()
{

    

    // clear the window to a blue
    m_game->window().clear(sf::Color(0, 0, 0));

    // Draw all tiles
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

    // draw the game title in the top-left of the screen
    m_menuText.setCharacterSize(50);
    m_menuText.setString("Level Select");
    m_menuText.setFillColor(sf::Color::White);
    m_menuText.setPosition(sf::Vector2f(m_game->window().getView().getCenter().x - 120, m_game->window().getView().getCenter().y - 150));
    m_game->window().draw(m_menuText);

    m_menuText.setCharacterSize(16);

    // draw all of the menu options
    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        if (i != 5)
        {
            m_menuText.setString(m_menuStrings[i]);
            m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color(100, 100, 100));
            m_menuText.setPosition(sf::Vector2f(192 + (i * 128), 576));
            m_game->window().draw(m_menuText);
        }

        if (i == 5)
        {
            m_menuText.setString(m_menuStrings[i]);
            m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color(100, 100, 100));
            m_menuText.setPosition(170, 400);
            m_game->window().draw(m_menuText);
        }

        if (m_selectedMenuIndex != 5)
        {
            for (auto e : m_entityManager.getEntities("player"))
            {
                e->getComponent<CTransform>().pos = Vec2(getPosition(0, 0, 3 + (2 * m_selectedMenuIndex), 10));
            }
        }
    }

    // draw the controls in the middle
    m_menuText.setCharacterSize(20);
    m_menuText.setFillColor(sf::Color(100, 100, 100));
    m_menuText.setString("Left: A     Right: D    Enter: W");
    m_menuText.setPosition(sf::Vector2f(170, 354));
    m_game->window().draw(m_menuText);

    

}

void Scene_Levels::onEnd()
{
    m_game->changeScene("MENU", nullptr, true);
}