#pragma once

#include "Common.h"
#include "Scene.h"
#include <map>
#include <memory>
#include <deque>

#include "EntityManager.h"

class Scene_Credits : public Scene
{

protected:

    std::string                 m_title;
    std::vector<std::string>    m_creditStrings;
    float                         m_scrollAmount = 0;
    sf::Text                    m_menuText;
    size_t                      m_selectedMenuIndex = 0;

    void init();
    void update();
    void onEnd();
    void sDoAction(const Action& action);

public:

    Scene_Credits(GameEngine* gameEngine = nullptr);
    void sRender();

};
