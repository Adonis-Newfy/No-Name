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
                                      
#include "Scene_Editor.h"
#include "Common.h"
#include "Physics.h"
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
    loadBlock();

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Golem"));


    // Register the actions required to play the game

    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Y, "TOGGLE_FOLLOW");       // Toggle follow camera
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");      // Toggle drawing (T)extures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");    // Toggle drawing (C)ollision Boxes
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");         // Toggle drawing (G)rid
    registerAction(sf::Keyboard::W, "MOVE_UP");             // Move Up
    registerAction(sf::Keyboard::A, "MOVE_LEFT");           // Move Left
    registerAction(sf::Keyboard::D, "MOVE_RIGHT");          // Move Right    
    registerAction(sf::Keyboard::S, "MOVE_DOWN");           // Move Down

    registerAction(sf::Keyboard::Space, "PLACE");           // Place Block
    registerAction(sf::Keyboard::BackSpace, "DELETE");      // Remove Block
    registerAction(sf::Keyboard::Tab, "SWAP");              // Cycle through available tiles

    registerAction(sf::Keyboard::V, "TEST");                // Run the TestOutput function.

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

            auto e = m_entityManager.addEntity("player");

            e->addComponent<CTransform>(Vec2(spawnX, spawnY));
            e->addComponent<CAnimation>(m_game->assets().getAnimation("IdleMan"), true);
            e->addComponent<CBoundingBox>(Vec2(boxX, boxY), true, false);
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
            dec->addComponent<CBoundingBox>(m_game->assets().getAnimation(animationName).getSize(), 0, 0);
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
        }
    }

    spawnPlayer();
    m_game->playSound("MusicLevel");
}

void Scene_Editor::saveLevel(const std::string& filename)
{
    std::ofstream file(filename);

    for (auto e : m_entityManager.getEntities())
    {
        if (e != m_player)
        {
            int i = 0;

            //If Player Spawn
            if (e->getComponent<CAnimation>().animation.getName() == blockVector[i].animationName)
            {
                int spawnX = e->getComponent<CTransform>().pos.x;
                int spawnY = e->getComponent<CTransform>().pos.y;
                int boxX = e->getComponent<CBoundingBox>().size.x;
                int boxY = e->getComponent<CBoundingBox>().size.y;
                int speed = m_playerConfig.SPEED;
                int health = m_playerConfig.HEALTH;
                float gravity = m_playerConfig.GRAVITY;
                float jump = m_playerConfig.JUMP;

                // output all this information to the file
                file << "Player " << spawnX << " " << spawnY << " " << boxX << " " << boxY << " " << speed << " " << health << " " << gravity << " " << jump << std::endl;
            }

            i++;

            //If Tiles
            if (e->getComponent<CAnimation>().animation.getName() == blockVector[i].animationName || e->tag() == "tile")
            {
                if (e->hasComponent<CPatrol>())
                {

                    std::string animationName = e->getComponent<CAnimation>().animation.getName();
                    int roomX = int(e->getComponent<CTransform>().pos.x / m_game->window().getSize().x);
                    int roomY = int(e->getComponent<CTransform>().pos.y / m_game->window().getSize().y);
                    int tileX = int(e->getComponent<CTransform>().pos.x - (roomX * m_game->window().getSize().x)) / m_gridSize.x;
                    int tileY = int(e->getComponent<CTransform>().pos.y - (roomY * m_game->window().getSize().y)) / m_gridSize.y;
                    int blockV = e->getComponent<CBoundingBox>().blockVision;
                    int blockM = e->getComponent<CBoundingBox>().blockMove;
                    float speed = e->getComponent<CPatrol>().speed;
                    int patrolPoints = e->getComponent<CPatrol>().positions.size();

                    std::string AI = "Patrol";

                    file << "Tile " << animationName << " " << roomX << " " << roomY << " " << tileX << " " << tileY << " " << blockM << " " << blockV << " " << AI << " " << speed << " " << patrolPoints;

                    for (int size_t = 0; i < e->getComponent<CPatrol>().positions.size(); i++)
                    {
                        file << " " << int(e->getComponent<CPatrol>().positions[i].x - (roomX * m_game->window().getSize().x)) / m_gridSize.x << " "
                            << int(e->getComponent<CPatrol>().positions[i].y - (roomY * m_game->window().getSize().y)) / m_gridSize.y;
                    }

                    file << std::endl;
                }

                else
                {
                    std::string animationName = e->getComponent<CAnimation>().animation.getName();
                    int roomX = int(e->getComponent<CTransform>().pos.x / m_game->window().getSize().x);
                    int roomY = int(e->getComponent<CTransform>().pos.y / m_game->window().getSize().y);
                    int tileX = int(e->getComponent<CTransform>().pos.x - (roomX * m_game->window().getSize().x)) / m_gridSize.x;
                    int tileY = int(e->getComponent<CTransform>().pos.y - (roomY * m_game->window().getSize().y)) / m_gridSize.y;
                    int blockV = e->getComponent<CBoundingBox>().blockVision;
                    int blockM = e->getComponent<CBoundingBox>().blockMove;

                    std::string AI = "Static";

                    file << "Tile " << animationName << " " << roomX << " " << roomY << " " << tileX << " " << tileY << " " << blockM << " " << blockV << " " << AI << std::endl;
                }
            }

            i++;

            //If Enemies
            if (e->getComponent<CAnimation>().animation.getName() == blockVector[i].animationName || e->tag() == "npc")
            {
                if (e->hasComponent<CPatrol>())
                {
                    std::string animationName = e->getComponent<CAnimation>().animation.getName();
                    int roomX = int(e->getComponent<CTransform>().pos.x / m_game->window().getSize().x);
                    int roomY = int(e->getComponent<CTransform>().pos.y / m_game->window().getSize().y);
                    int tileX = int(e->getComponent<CTransform>().pos.x - (roomX * m_game->window().getSize().x)) / m_gridSize.x;
                    int tileY = int(e->getComponent<CTransform>().pos.y - (roomY * m_game->window().getSize().y)) / m_gridSize.y;
                    int blockV = e->getComponent<CBoundingBox>().blockVision;
                    int blockM = e->getComponent<CBoundingBox>().blockMove;
                    int maxHealth = 5;
                    int damage = 2;
                    float gravity = 0.75;
                    float speed = e->getComponent<CPatrol>().speed;
                    int patrolPoints = e->getComponent<CPatrol>().positions.size();

                    std::string AI = "Patrol";

                    file << "NPC " << animationName << " " << roomX << " " << roomY << " " << tileX << " " << tileY << " " << blockM << " " << blockV << " "
                         << maxHealth << " " << damage << " " << gravity << " " << AI << " " << speed << " " << patrolPoints;

                    for (int size_t = 0; i < e->getComponent<CPatrol>().positions.size(); i++)
                    {
                        file << " " << int(e->getComponent<CPatrol>().positions[i].x - (roomX * m_game->window().getSize().x)) / m_gridSize.x << " " 
                                    << int(e->getComponent<CPatrol>().positions[i].y - (roomY * m_game->window().getSize().y)) / m_gridSize.y;
                    }

                    file << std::endl;
                }

                else
                {
                    std::string animationName = e->getComponent<CAnimation>().animation.getName();
                    int roomX = int(e->getComponent<CTransform>().pos.x / m_game->window().getSize().x);
                    int roomY = int(e->getComponent<CTransform>().pos.y / m_game->window().getSize().y);
                    int tileX = int(e->getComponent<CTransform>().pos.x - (roomX * m_game->window().getSize().x)) / m_gridSize.x;
                    int tileY = int(e->getComponent<CTransform>().pos.y - (roomY * m_game->window().getSize().y)) / m_gridSize.y;
                    int blockV = e->getComponent<CBoundingBox>().blockVision;
                    int blockM = e->getComponent<CBoundingBox>().blockMove;
                    int maxHealth = 5;
                    int damage = 2;
                    float gravity = 0.75;
                    float speed = 1;

                    std::string AI = "Follow";

                    file << "NPC " << animationName << " " << roomX << " " << roomY << " " << tileX << " " << tileY << " " << blockM << " " << blockV << " "
                        << maxHealth << " " << damage << " " << gravity << " " << AI << " " << speed;
                }

                file << std::endl;
            }

            i++;

            // If Decs
            if (e->getComponent<CAnimation>().animation.getName() == blockVector[i].animationName || e->getComponent<CAnimation>().animation.getName() == blockVector[++i].animationName || e->tag() == "dec")
            {
                std::string animationName = e->getComponent<CAnimation>().animation.getName();
                int roomX = int(e->getComponent<CTransform>().pos.x / m_game->window().getSize().x);
                int roomY = int(e->getComponent<CTransform>().pos.y / m_game->window().getSize().y);
                int tileX = int(e->getComponent<CTransform>().pos.x - (roomX * m_game->window().getSize().x)) / m_gridSize.x;
                int tileY = int(e->getComponent<CTransform>().pos.y - (roomY * m_game->window().getSize().y)) / m_gridSize.y;
                int layer = 0;

                if (e->getComponent<CBoundingBox>().blockVision == 0)
                {
                    layer = 1;
                }
                if (e->getComponent<CBoundingBox>().blockVision == 1)
                {
                    layer = 2;
                }

                file << "Dec " << animationName << " " << roomX << " " << roomY << " " << tileX << " " << tileY << " " << layer << std::endl;
            }
        }
    }
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

Vec2 Scene_Editor::convertToGridSpace(std::shared_ptr<Entity> entity)
{
    float CELLSIZE = 64.0f;

    int cell_x = entity->getComponent<CTransform>().pos.x / CELLSIZE;
    int cell_y = entity->getComponent<CTransform>().pos.y / CELLSIZE;

    if (entity->getComponent<CTransform>().pos.x < 0)
    {
        cell_x -= 1;
    }

    if (entity->getComponent<CTransform>().pos.y < 0)
    {
        cell_y -= 1;
    }

    // Return the x and y grid position of the entity
    return Vec2(cell_x, cell_y);
}

void Scene_Editor::spawnPlayer()
{
    m_player = m_entityManager.addEntity("player");

    m_playerConfig.X = 640;
    m_playerConfig.Y = 360;
    m_playerConfig.CX = 48;
    m_playerConfig.CY = 48;
    m_playerConfig.SPEED = 5;
    m_playerConfig.HEALTH = 3;
    m_playerConfig.GRAVITY = 0.75;
    m_playerConfig.JUMP = -20;
    
    m_player->addComponent<CTransform>(Vec2(m_playerConfig.X, m_playerConfig.Y));
    m_player->addComponent<CAnimation>(m_game->assets().getAnimation("IdleMan"), true);
    m_player->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY), true, false);
    m_player->addComponent<CInput>();
}

void Scene_Editor::loadBlock()
{
    Player player;
    Tile tile;
    Enemy enemy;
    Dec1 dec1;
    Dec2 dec2;

    blockVector.push_back(player);
    blockVector.push_back(tile);
    blockVector.push_back(enemy);
    blockVector.push_back(dec1);
    blockVector.push_back(dec2);

    maxIndex = blockVector.size();
    std::cout << maxIndex << std::endl;
}

void Scene_Editor::placeBlock()
{
    Vec2 cursorPosition = convertToGridSpace(m_player);

    cursorPosition.x = cursorPosition.x * m_gridSize.x + m_gridSize.x/2;
    cursorPosition.y = cursorPosition.y * m_gridSize.y + m_gridSize.y/2;


    auto e = m_entityManager.addEntity(m_player->tag());

    e->addComponent<CTransform>(cursorPosition);
    e->addComponent<CAnimation>(m_player->getComponent<CAnimation>());
    e->addComponent<CBoundingBox>(m_player->getComponent<CBoundingBox>());
}

void Scene_Editor::deleteBlock()
{
    Vec2 cursorPosition = convertToGridSpace(m_player);

    for (auto e : m_entityManager.getEntities())
    {
        if (Physics::GetOverlap(e, m_player).x > 0 && Physics::GetOverlap(e, m_player).y > 0 && e->hasComponent<CBoundingBox>() && e != m_player)
        {
            if (cursorPosition == convertToGridSpace(e))
            {
                e->destroy();
            }
        }
    }
}

void Scene_Editor::cycleBlock()
{
    if (index + 1 < maxIndex)
    {
        index ++;
    }
    else
    {
        index = 0;
    }

    m_player->addComponent<CAnimation>(m_game->assets().getAnimation(blockVector[index].animationName), true);
    m_player->addComponent<CBoundingBox>(m_game->assets().getAnimation(blockVector[index].animationName).getSize(), blockVector[index].move, blockVector[index].vision);
}

void Scene_Editor::update()
{
    m_entityManager.update();


    // Implement pause functionality

    if (!m_paused)
    {
        sMovement();
        sCamera();
    }

    m_currentFrame++;
}

void Scene_Editor::sTestValue()
{
    saveLevel(m_levelPath);
}

void Scene_Editor::sMovement()
{
    auto& pTransform = m_player->getComponent<CTransform>();
    auto& pInput = m_player->getComponent<CInput>();

    auto& pState = m_player->getComponent<CState>();

    Vec2 playerSpeed = m_player->getComponent<CTransform>().velocity;

    playerSpeed = Vec2(0, 0);

    if (pInput.right)
    {
        playerSpeed.x += m_playerConfig.SPEED;
    }
    if (pInput.left)
    {
        playerSpeed.x -= m_playerConfig.SPEED;
    }
    if (pInput.up)
    {
        playerSpeed.y -= m_playerConfig.SPEED;
    }
    if (pInput.down)
    {
        playerSpeed.y += m_playerConfig.SPEED;
    }
    
    pTransform.velocity = playerSpeed;

    for (auto& e : m_entityManager.getEntities())
    {
        e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
        e->getComponent<CTransform>().pos += e->getComponent<CTransform>().velocity;
    }
}

void Scene_Editor::sDoAction(const Action& action)
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
        else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
        else if (action.name() == "MOVE_UP") { m_player->getComponent<CInput>().up = true; }
        else if (action.name() == "MOVE_LEFT") { m_player->getComponent<CInput>().left = true; }
        else if (action.name() == "MOVE_RIGHT") { m_player->getComponent<CInput>().right = true; }
        else if (action.name() == "MOVE_DOWN") { m_player->getComponent<CInput>().down = true; }
        else if (action.name() == "TEST") { sTestValue(); }
        else if (action.name() == "PLACE") { placeBlock(); }
        else if (action.name() == "DELETE") { deleteBlock(); }
        else if (action.name() == "SWAP") { cycleBlock(); }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "MOVE_UP") { m_player->getComponent<CInput>().up = false; }
        else if (action.name() == "MOVE_LEFT") { m_player->getComponent<CInput>().left = false; }
        else if (action.name() == "MOVE_DOWN") { m_player->getComponent<CInput>().down = false; }
        else if (action.name() == "MOVE_RIGHT") { m_player->getComponent<CInput>().right = false; }
    }
}

void Scene_Editor::sCamera()
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

void Scene_Editor::onEnd()
{

    // Implement scene end
    // - stop the music
    // - play the menu music
    // - change scene to menu

    m_game->assets().getSound("MusicLevel").stop();
    m_game->playSound("MusicTitle");
    m_game->changeScene("MENU", nullptr, true);

}

void Scene_Editor::drawLine(const Vec2& p1, const Vec2& p2)
{
    sf::Vertex line[] = { sf::Vector2f(p1.x, p1.y), sf::Vector2f(p2.x, p2.y) };
    m_game->window().draw(line, 2, sf::Lines);
}


void Scene_Editor::sRender()
{
    // RENDERING DONE FOR YOU

    m_game->window().clear(sf::Color(255, 192, 122));
    sf::RectangleShape tick({ 1.0f, 6.0f });
    tick.setFillColor(sf::Color::Black);

    // draw all Entity textures / animations
    if (m_drawTextures)
    {
        sf::Texture background(m_game->assets().getTexture("TexBackground"));
        sf::Sprite backgroundSprite(background);
        backgroundSprite.setPosition(m_game->window().getView().getCenter().x - 640, m_game->window().getView().getCenter().y - 384);
        m_game->window().draw(backgroundSprite);

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

    if (m_drawGrid)
    {
        float leftX = m_game->window().getView().getCenter().x - width() / 2;
        float rightX = leftX + width() + m_gridSize.x;
        float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

        float topY = m_game->window().getView().getCenter().y - height() / 2;
        float bottomY = topY + height() + m_gridSize.y;
        float nextGridY = topY - ((int)topY % (int)m_gridSize.y);

        for (float x = nextGridX; x < rightX; x += m_gridSize.x)
        {
            drawLine(Vec2(x, topY), Vec2(x, bottomY));
        }

        for (float y = nextGridY; y < bottomY; y += m_gridSize.y)
        {
            drawLine(Vec2(leftX, y), Vec2(rightX, y));

        }
    }
}


// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2021FALL - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
