#pragma once
#include "Common.h"
#include "Scene.h"
#include "Blocks.h"
#include <map>
#include <memory>
#include <vector>

#include "EntityManager.h"

class Scene_Editor : public Scene
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
    bool                    m_drawGrid = true;
    bool                    m_follow = false;
    const Vec2              m_gridSize = { 64, 64 };

    std::vector<Vec2>       BlackTiles;

    std::vector<Block>      blockVector;
    size_t index = 0;
    size_t maxIndex = 3;

    sf::Text                m_gridText;

    void init(const std::string& levelPath);

    void loadLevel(const std::string& filename);
    void saveLevel(const std::string& filename);
    void spawnPlayer();

    void onEnd();
    void update();
    void sTestValue();

    void drawLine(const Vec2& p1, const Vec2& p2);

    void loadBlock();
    void placeBlock();
    void deleteBlock();
    void cycleBlock();
    Vec2 convertToGridSpace(std::shared_ptr<Entity> entity);

    void sMovement();
    void sCamera();

    Vec2 getPosition(int rx, int ry, int tx, int ty) const;


public:

    Scene_Editor(GameEngine* game, const std::string& levelPath);

    void sRender();
    void sDoAction(const Action& action);
};