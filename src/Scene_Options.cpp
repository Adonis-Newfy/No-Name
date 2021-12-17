#include "Scene_Menu.h"
#include "Scene_Play.h"
#include "Scene_Levels.h"
#include "Scene_Levels_Editor.h"
#include "Common.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include "Scene_Options.h"

Scene_Options::Scene_Options(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}

void Scene_Options::init()
{
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::D, "POSITIVE");
    registerAction(sf::Keyboard::A, "NEGATIVE");
    registerAction(sf::Keyboard::Escape, "BACK");

    m_title = "No Name";

    m_menuStrings.push_back("Music Volume:");
    m_menuStrings.push_back("Sound Effect Volume:");
    m_menuStrings.push_back("Difficulty");

    m_menuStrings.push_back("Move Left:");
    m_menuStrings.push_back("Move Right:");
    m_menuStrings.push_back("Move Jump:");
    m_menuStrings.push_back("Attack");

    m_menuStrings2.push_back("100");
    m_menuStrings2.push_back("100");
    m_menuStrings2.push_back("Normal (FUNCTIONALITY WIP)");

    m_menuStrings2.push_back("A             (FUNCTIONALITY WIP)");
    m_menuStrings2.push_back("D             (FUNCTIONALITY WIP)");
    m_menuStrings2.push_back("W             (FUNCTIONALITY WIP)");
    m_menuStrings2.push_back("Space   (FUNCTIONALITY WIP)");

    m_difficultyStrings.push_back("Easy");
    m_difficultyStrings.push_back("Normal");
    m_difficultyStrings.push_back("Hard");

    m_menuText.setFont(m_game->assets().getFont("Golem"));
    m_menuText.setCharacterSize(64);
}

void Scene_Options::update()
{
    m_entityManager.update();
}

void Scene_Options::sDoAction(const Action& action)
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
        else if (action.name() == "POSITIVE")
        {
            if (m_selectedMenuIndex == 0)
            {
                if (m_menuStrings2[0] != "100")
                {
                    int increaseSound = 25;
                    int oldVolume = std::stoi(m_menuStrings2[0]);
                    int newVolume = oldVolume + increaseSound;
                    m_menuStrings2[0] = std::to_string(newVolume);

                    sChangeMusicVolume(6.25f);
                }
            }

            if (m_selectedMenuIndex == 1)
            {
                if (m_menuStrings2[1] != "100")
                {
                    int increaseSound = 25;
                    int oldVolume = std::stoi(m_menuStrings2[1]);
                    int newVolume = oldVolume + increaseSound;
                    m_menuStrings2[1] = std::to_string(newVolume);

                    sChangeSoundVolume(6.25);
                }
            }
        }
        else if (action.name() == "NEGATIVE")
        {
            if (m_selectedMenuIndex == 0)
            {
                if (m_menuStrings2[0] != "0")
                {
                    int reduceSound = -25;
                    int oldVolume = std::stoi(m_menuStrings2[0]);
                    int newVolume = oldVolume + reduceSound;
                    m_menuStrings2[0] = std::to_string(newVolume);

                    sChangeMusicVolume(-6.25f);
                }
            }

            if (m_selectedMenuIndex == 1)
            {
                if (m_menuStrings2[1] != "0")
                {
                    int reduceSound = -25;
                    int oldVolume = std::stoi(m_menuStrings2[1]);
                    int newVolume = oldVolume + reduceSound;
                    m_menuStrings2[1] = std::to_string(newVolume);

                    sChangeSoundVolume(-6.25);
                }
            }
        }
        else if (action.name() == "BACK")
        {
            m_game->changeScene("MENU", nullptr, true);
        }
    }
}

void Scene_Options::sChangeMusicVolume(float volume)
{
    m_game->assets().getSound("Music").setVolume(m_game->assets().getSound("Music").getVolume() + volume);
}

void Scene_Options::sChangeSoundVolume(float volume)
{
    m_game->assets().getSound("Attack").setVolume(m_game->assets().getSound("Attack").getVolume() + volume);
    m_game->assets().getSound("EnemyHurt").setVolume(m_game->assets().getSound("EnemyHurt").getVolume() + volume);
    m_game->assets().getSound("GameOver").setVolume(m_game->assets().getSound("GameOver").getVolume() + volume);
    m_game->assets().getSound("PickupItem").setVolume(m_game->assets().getSound("PickupItem").getVolume() + volume);
    m_game->assets().getSound("PlayerHurt").setVolume(m_game->assets().getSound("PlayerHurt").getVolume() + volume);
    m_game->assets().getSound("UseItem").setVolume(m_game->assets().getSound("UseItem").getVolume() + volume);
    m_game->assets().getSound("Victory").setVolume(m_game->assets().getSound("Victory").getVolume() + volume);
}

void Scene_Options::sRender()
{


    // clear the window to a blue
    m_game->window().setView(m_game->window().getDefaultView());
    m_game->window().clear(sf::Color::Black);

    // draw the game title in the top-left of the screen
    m_menuText.setCharacterSize(100);
    m_menuText.setString("Options");
    m_menuText.setFillColor(sf::Color::White);
    m_menuText.setPosition(sf::Vector2f(500, 20));
    m_game->window().draw(m_menuText);

    m_menuText.setCharacterSize(50);

    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        m_menuText.setString(m_menuStrings[i]);
        m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color(100, 100, 100));
        m_menuText.setPosition(sf::Vector2f(20, 150 + i * 50));
        m_game->window().draw(m_menuText);

        m_menuText.setString(m_menuStrings2[i]);
        m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color(100, 100, 100));
        m_menuText.setPosition(sf::Vector2f(600, 150 + i * 50));
        m_game->window().draw(m_menuText);
    }

    

    // draw the controls in the bottom-left
    m_menuText.setCharacterSize(20);
    m_menuText.setFillColor(sf::Color(100, 100, 100));
    m_menuText.setString("up: w     down: s    Left: a    Right: d      back: esc");
    m_menuText.setPosition(sf::Vector2f(10, 690));
    m_game->window().draw(m_menuText);
}

void Scene_Options::onEnd()
{

}