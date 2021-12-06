#pragma once
#include "Common.h"
#include "Scene.h"
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
    bool                    m_follow = false;

    std::vector<Vec2>       BlackTiles;

    void init(const std::string& levelPath);

    void loadLevel(const std::string& filename);
    void saveLevel(const std::string& filename);

    void onEnd();
    void update();

    void sMovement();
    void sCamera();

    Vec2 getPosition(int rx, int ry, int tx, int ty) const;


public:

    Scene_Editor(GameEngine* game, const std::string& levelPath);

    void sRender();
    void sDoAction(const Action& action);
};