#include "Scene_Menu.h"
#include "Scene_Play.h"
#include "Scene_Levels.h"
#include "Scene_Levels_Editor.h"
#include "Common.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include "Scene_GameOver.h"
#include "Scene_Victory.h"


Scene_Victory::Scene_Victory(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}

void Scene_Victory::init()
{
    registerAction(sf::Keyboard::W, "UP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::D, "ZELDA");
    registerAction(sf::Keyboard::Escape, "QUIT");

    m_menuText.setFont(m_game->assets().getFont("Golem"));
    m_menuText.setCharacterSize(64);
    m_menuStrings.push_back("CONTINUE");

    m_entityManager = EntityManager();

    auto playerSprite = m_entityManager.addEntity("player");
    playerSprite->addComponent<CAnimation>(m_game->assets().getAnimation("SitMan"), true);
    playerSprite->addComponent<CTransform>(Vec2(608, 325));

}

void Scene_Victory::update()
{
    m_entityManager.update();

    for (auto e : m_entityManager.getEntities("player"))
    {
        e->getComponent<CAnimation>().animation.update();
    }
}

void Scene_Victory::sDoAction(const Action& action)
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
            if (m_selectedMenuIndex == 0)
            {
                m_game->changeScene("LEVELS", std::make_shared<Scene_Levels>(m_game), true);
            }
        }


    }
}

void Scene_Victory::sRender()
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



    // draw the game title in the top-left of the screen
    m_menuText.setCharacterSize(100);
    m_menuText.setString("LEVEL COMPLETE");
    m_menuText.setFillColor(sf::Color(100, 100, 100));
    m_menuText.setPosition(sf::Vector2f(300, 20));
    m_game->window().draw(m_menuText);

    m_menuText.setCharacterSize(50);

    // draw all of the menu options
    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        m_menuText.setString(m_menuStrings[i]);
        m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color(100, 100, 100));
        m_menuText.setPosition(sf::Vector2f(500, 450 + i * 72));
        m_game->window().draw(m_menuText);
    }
}

void Scene_Victory::onEnd()
{

}