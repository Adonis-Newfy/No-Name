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
#include "Scene_Credits.h"


Scene_Credits::Scene_Credits(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}

void Scene_Credits::init()
{
    
    registerAction(sf::Keyboard::Escape, "RETURN");

    m_title = "No Name";

    m_menuText.setFont(m_game->assets().getFont("Golem"));
    m_menuText.setCharacterSize(64);

    m_creditStrings.push_back("Developed by: ");
    m_creditStrings.push_back(" - Nathan Rogers (ncsrogers)");
    m_creditStrings.push_back(" - Nicholas Anstey (nmcanstey)");

    m_creditStrings.push_back("Developed using framework provided by:");
    m_creditStrings.push_back(" - Dr. David Churchill");

    m_creditStrings.push_back("All Texture Assets were sourced using itch.io free assets");

    m_creditStrings.push_back("Player Character Assets:");
    m_creditStrings.push_back(" - Rogue: Rogue Knight (Created by Kronovi-)");
    m_creditStrings.push_back(" - Knight: 2D SL Character (Created by Szadi Art)");
    m_creditStrings.push_back(" - Ranger: Archer Hero (Created by Kronovi-)");
    m_creditStrings.push_back(" - Wizard: Wizard Pack (Created by LuizMelo)");

    m_creditStrings.push_back("Enemy Assets:");
    m_creditStrings.push_back(" - Mushroom: Monsters Creature Fantasy (Created by LuizMelo)");
    m_creditStrings.push_back(" - Flying Eye: Monsters Creature Fantasy (Created by LuizMelo)");

    m_creditStrings.push_back("Weapon Assets:");
    m_creditStrings.push_back(" - Rogue Sword: Medieval Swords 2 (Created by RandomIndieDev)");
    m_creditStrings.push_back(" - Knight Sword: Medieval Swords 2 (Created by RandomIndieDev");
    m_creditStrings.push_back(" - Ranger Arrow: Pixel Arrow (Created by kenam0)");
    m_creditStrings.push_back(" - Fire Ball: Pixel Art Fire Ball (Created by Matheus Frantz)");

    m_creditStrings.push_back("Item Assets:");
    m_creditStrings.push_back(" - Coin: 2D Pixel Item Asset Pack (Created by Startled Pixels)");
    m_creditStrings.push_back(" - Health Potion: 2D Pixel Item Asset Pack (Created by Startled Pixels)");
    m_creditStrings.push_back(" - Invlunerability Potion: 2D Pixel Item Asset Pack (Created by Startled Pixels)");
    m_creditStrings.push_back(" - Strength Potion: 2D Pixel Item Asset Pack (Created by Startled Pixels)");

    m_creditStrings.push_back("Environment Assets: Pixel Fantasy Caves (Created by Szadi Art)");

    m_creditStrings.push_back("All Sound Assets were sourced using mixkit.co free sound assets");

    m_creditStrings.push_back("We Hope You Enjoyed!");

    m_game->playSound("Music");
    m_game->assets().getSound("Music").setLoop(true);
}

void Scene_Credits::update()
{
    m_entityManager.update();
}

void Scene_Credits::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "RETURN")
        {
            onEnd();
        }
    }
}

void Scene_Credits::sRender()
{


    // clear the window to a blue
    m_game->window().setView(m_game->window().getDefaultView());
    m_game->window().clear();

    

    m_menuText.setCharacterSize(25);

    for (int i = 0; i < m_creditStrings.size(); i++)
    {
        m_menuText.setString(m_creditStrings[i]);
        m_menuText.setFillColor(sf::Color::White);
        m_menuText.setPosition(sf::Vector2f(20, 200 + (i * 50) + m_scrollAmount));
        m_game->window().draw(m_menuText);
        
        if (m_menuText.getString() == m_creditStrings[m_creditStrings.size() - 1])
        {
            if (m_menuText.getPosition().y < 50)
            {
                m_menuText.setString("Press Esc to return to main menu");
                m_menuText.setFillColor(sf::Color::White);
                m_menuText.setPosition(sf::Vector2f(20, 500));
                m_game->window().draw(m_menuText);
            }
        }
    }


    sf::RectangleShape rect({ 1000, 100 });
    rect.setPosition(0, 0);
    rect.setFillColor(sf::Color::Black);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(2);
    m_game->window().draw(rect);

    m_menuText.setCharacterSize(50);
    m_menuText.setString("Thank You For Playing!");
    m_menuText.setPosition(10, 10);
    m_menuText.setColor(sf::Color::White);
    m_game->window().draw(m_menuText);

    m_scrollAmount -= 1;


    /*
    // draw background
    sf::Texture background(m_game->assets().getTexture("TexBackgr2"));
    sf::Sprite backgroundSprite(background);
    backgroundSprite.setPosition(m_game->window().getView().getCenter().x - 640, m_game->window().getView().getCenter().y - 384);
    m_game->window().draw(backgroundSprite);

    // draw the game title in the top-left of the screen
    m_menuText.setCharacterSize(100);
    m_menuText.setString(m_title);
    m_menuText.setFillColor(sf::Color(100, 100, 100));
    m_menuText.setPosition(sf::Vector2f(500, 20));
    m_game->window().draw(m_menuText);

    m_menuText.setCharacterSize(50);

    // draw all of the menu options
    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        m_menuText.setString(m_menuStrings[i]);
        m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color(100, 100, 100));
        m_menuText.setPosition(sf::Vector2f(500, 300 + i * 72));
        m_game->window().draw(m_menuText);
    }

    // draw the controls in the bottom-left
    m_menuText.setCharacterSize(20);
    m_menuText.setFillColor(sf::Color(100, 100, 100));
    m_menuText.setString("up: w     down: s    play: d      back: esc");
    m_menuText.setPosition(sf::Vector2f(10, 690));
    m_game->window().draw(m_menuText);
    */
}

void Scene_Credits::onEnd()
{
    m_game->changeScene("MENU", nullptr, true);
}