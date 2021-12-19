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
    std::vector<std::string>    m_actionStrings;
    std::vector<int>            m_keyCodes;
    sf::Text                    m_menuText;
    size_t                      m_selectedMenuIndex = 0;

    size_t                      m_selectedMenuIndex2 = 1;

    std::vector<std::string>    m_difficultyStrings;

    std::string                 m_saveData = "options.txt";

    bool                        m_awaitingKeybind = false;
    size_t                      m_keybindFrame;
    std::string                 m_actionToBind;
           
    void init();
    void update();
    void onEnd();
    void sDoAction(const Action& action);

    void sChangeMusicVolume(float volume);
    void sChangeSoundVolume(float volume);
    void sBindKey();
    std::string sCodeToString(int x);
    void saveData(const std::string& filename);
    void loadData(const std::string& filename);

public:

    Scene_Options(GameEngine* gameEngine = nullptr);
    void sRender();

};

