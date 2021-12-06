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

    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CTransform>(Vec2(m_playerConfig.X, m_playerConfig.Y));

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
    while (!m_paused)
    {
        sCamera();
        sMovement();
        sRender();
    }
}

void Scene_Editor::sMovement()
{
    Vec2 playerVelocity(0, 0);

    if (m_player->getComponent<CInput>().up)
    {
        playerVelocity.y -= m_playerConfig.SPEED;
    }
    if (m_player->getComponent<CInput>().down)
    {
        playerVelocity.y += m_playerConfig.SPEED;
    }
    if (m_player->getComponent<CInput>().right)
    {
        playerVelocity.x += m_playerConfig.SPEED;
    }
    if (m_player->getComponent<CInput>().left)
    {
        playerVelocity.x -= m_playerConfig.SPEED;
    }

    // movement speed update

    m_player->getComponent<CTransform>().velocity = playerVelocity;

    for (auto e : m_entityManager.getEntities())
    {
        e->getComponent<CTransform>().pos += e->getComponent<CTransform>().velocity;
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

void Scene_Editor::sRender()
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
