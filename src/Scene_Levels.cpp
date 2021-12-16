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
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::D, "ZELDA");
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
}

void Scene_Levels::update()
{
    m_entityManager.update();
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
                m_game->changeScene("ZELDA", std::make_shared<Scene_Play>(m_game, m_levelPaths[m_selectedMenuIndex]));
                m_game->assets().getSound("MusicTitle").stop();
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
    m_game->window().setView(m_game->window().getDefaultView());
    m_game->window().clear(sf::Color(0, 0, 0));

    // draw the game title in the top-left of the screen
    m_menuText.setCharacterSize(48);
    m_menuText.setString(m_title);
    m_menuText.setFillColor(sf::Color(100, 100, 100));
    m_menuText.setPosition(sf::Vector2f(10, 10));
    m_game->window().draw(m_menuText);

    // draw all of the menu options
    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        m_menuText.setString(m_menuStrings[i]);
        m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color(100, 100, 100));
        m_menuText.setPosition(sf::Vector2f(10, 110 + i * 72));
        m_game->window().draw(m_menuText);
    }

    // draw the controls in the bottom-left
    m_menuText.setCharacterSize(20);
    m_menuText.setFillColor(sf::Color(100, 100, 100));
    m_menuText.setString("up: w     down: s    play: d      back: esc");
    m_menuText.setPosition(sf::Vector2f(10, 690));
    m_game->window().draw(m_menuText);
}

void Scene_Levels::onEnd()
{
    m_game->changeScene("MENU", nullptr, true);
}