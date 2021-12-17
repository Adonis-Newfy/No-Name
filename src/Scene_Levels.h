#pragma once

#include "Common.h"
#include "Scene.h"
#include <map>
#include <memory>
#include <deque>

#include "EntityManager.h"

class Scene_Levels : public Scene
{

protected:

    std::string                 m_title;
    std::vector<std::string>    m_menuStrings;
    std::vector<std::string>    m_levelPaths;
    sf::Text                    m_menuText;
    size_t                      m_selectedMenuIndex = 0;
    std::string                 m_saveData = "savedata.txt";

    std::vector<bool>           unlocked;

    void init();
    void update();
    void onEnd();
    void sDoAction(const Action& action);
    void loadData(const std::string& filename);
    void loadLevel(const std::string& filename);
    Vec2 getPosition(int sx, int sy, int tx, int ty) const;

public:

    Scene_Levels(GameEngine* gameEngine = nullptr);
    void sRender();

};