#pragma once

#include "Common.h"
#include "Scene.h"
#include <map>
#include <memory>
#include <deque>

#include "EntityManager.h"

class Scene_Options : public Scene
{

protected:

    std::string                 m_title;
    std::vector<std::string>    m_menuStrings;
    std::vector<std::string>    m_menuStrings2;
    sf::Text                    m_menuText;
    size_t                      m_selectedMenuIndex = 0;

    size_t                      m_selectedMenuIndex2 = 1;

    std::vector<std::string>    m_difficultyStrings;

    void init();
    void update();
    void onEnd();
    void sDoAction(const Action& action);

    void sChangeMusicVolume(float volume);
    void sChangeSoundVolume(float volume);

public:

    Scene_Options(GameEngine* gameEngine = nullptr);
    void sRender();

};

