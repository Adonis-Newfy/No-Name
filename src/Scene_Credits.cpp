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

    m_entityManager = EntityManager();

    auto playerSprite = m_entityManager.addEntity("player");
    playerSprite->addComponent<CAnimation>(m_game->assets().getAnimation("RunMan"), true);
    playerSprite->addComponent<CTransform>(Vec2(200,384));
    playerSprite->getComponent<CTransform>().scale = Vec2(-5.0f, 5.0f);

    auto playerSprite2 = m_entityManager.addEntity("player");
    playerSprite2->addComponent<CAnimation>(m_game->assets().getAnimation("RunWar"), true);
    playerSprite2->addComponent<CTransform>(Vec2(520, 384));
    playerSprite2->getComponent<CTransform>().scale = Vec2(-5.0f, 5.0f);

    auto playerSprite3 = m_entityManager.addEntity("player");
    playerSprite3->addComponent<CAnimation>(m_game->assets().getAnimation("RunRang"), true);
    playerSprite3->addComponent<CTransform>(Vec2(840, 384));
    playerSprite3->getComponent<CTransform>().scale = Vec2(-5.0f, 5.0f);

    auto playerSprite4 = m_entityManager.addEntity("player");
    playerSprite4->addComponent<CAnimation>(m_game->assets().getAnimation("RunWiz"), true);
    playerSprite4->addComponent<CTransform>(Vec2(1160, 384));
    playerSprite4->getComponent<CTransform>().scale = Vec2(-5.0f, 5.0f);
    
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
    for (auto e : m_entityManager.getEntities("player"))
    {
        e->getComponent<CAnimation>().animation.update();
    }
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

    // Draw all tiles
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

}

void Scene_Credits::onEnd()
{
    m_game->changeScene("MENU", nullptr, true);
}