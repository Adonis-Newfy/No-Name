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

    

    m_menuStrings.push_back("Move Left:");
    m_menuStrings.push_back("Move Right");
    m_menuStrings.push_back("Jump:");
    m_menuStrings.push_back("Attack:");
    m_menuStrings.push_back("Swap Character:");
    m_menuStrings.push_back("Open Inventory:");
    m_menuStrings.push_back("Menu Up:");
    m_menuStrings.push_back("Menu Down:");
    m_menuStrings.push_back("Menu Left:");
    m_menuStrings.push_back("Menu Right:");

    m_menuStrings.push_back("Music Volume:");
    m_menuStrings.push_back("Sound Effect Volume:");
    m_menuStrings.push_back("Difficulty");

    m_actionStrings.push_back("MOVE_LEFT");
    m_actionStrings.push_back("MOVE_RIGHT");
    m_actionStrings.push_back("JUMP");
    m_actionStrings.push_back("ATTACK");
    m_actionStrings.push_back("SWAP");
    m_actionStrings.push_back("OPEN_INVENTORY");
    m_actionStrings.push_back("MENU_UP");
    m_actionStrings.push_back("MENU_DOWN");
    m_actionStrings.push_back("MENU_LEFT");
    m_actionStrings.push_back("MENU_RIGHT");

    m_keyCodes.push_back(sf::Keyboard::A);
    m_keyCodes.push_back(sf::Keyboard::D);
    m_keyCodes.push_back(sf::Keyboard::W);
    m_keyCodes.push_back(sf::Keyboard::Space);
    m_keyCodes.push_back(sf::Keyboard::Tab);
    m_keyCodes.push_back(sf::Keyboard::I);
    m_keyCodes.push_back(sf::Keyboard::Up);
    m_keyCodes.push_back(sf::Keyboard::Down);
    m_keyCodes.push_back(sf::Keyboard::Left);
    m_keyCodes.push_back(sf::Keyboard::Right);

    loadData(m_saveData);

    m_menuStrings2.push_back(sCodeToString(m_keyCodes[0]));
    m_menuStrings2.push_back(sCodeToString(m_keyCodes[1]));
    m_menuStrings2.push_back(sCodeToString(m_keyCodes[2]));
    m_menuStrings2.push_back(sCodeToString(m_keyCodes[3]));
    m_menuStrings2.push_back(sCodeToString(m_keyCodes[4]));
    m_menuStrings2.push_back(sCodeToString(m_keyCodes[5]));
    m_menuStrings2.push_back(sCodeToString(m_keyCodes[6]));
    m_menuStrings2.push_back(sCodeToString(m_keyCodes[7]));
    m_menuStrings2.push_back(sCodeToString(m_keyCodes[8]));
    m_menuStrings2.push_back(sCodeToString(m_keyCodes[9]));

    m_menuStrings2.push_back("100");
    m_menuStrings2.push_back("100");
    m_menuStrings2.push_back("Normal");

    m_difficultyStrings.push_back("Easy");
    m_difficultyStrings.push_back("Normal");
    m_difficultyStrings.push_back("Hard");

    m_menuText.setFont(m_game->assets().getFont("Golem"));
    m_menuText.setCharacterSize(64);
}

void Scene_Options::saveData(const std::string& filename)
{
    std::ofstream file(filename);

    file << "Difficulty ";
    if (m_menuStrings2[12] == m_difficultyStrings[0])
    {
        file << 0.5f << std::endl;
    }

    if (m_menuStrings2[12] == m_difficultyStrings[1])
    {
        file << 1.0f << std::endl;
    }

    if (m_menuStrings2[12] == m_difficultyStrings[2])
    {
        file << 2.0f << std::endl;
    }
    
    for (int i = 0; i < m_actionStrings.size(); i++)
    {
        file << "Keybind " << m_actionStrings[i] << " " << m_keyCodes[i] << std::endl;
    }
}

void Scene_Options::loadData(const std::string& filename)
{
    std::ifstream fin(filename);
    std::string text;

    int i = 0;

    while (fin >> text)
    {
        if (text == "Difficulty")
        {
            std::string value = "";

            fin >> value;

            if (value == "0.5")
            {
                m_menuStrings2[12] == m_difficultyStrings[0];
            }

            if (value == "1")
            {
                m_menuStrings2[12] == m_difficultyStrings[0];
            }

            if (value == "2")
            {
                m_menuStrings2[12] == m_difficultyStrings[0];
            }
        }

        std::string action = "";
        int code;

        if (text == "Keybind")
        {
            fin >> action >> code;
            m_keyCodes[i] = code;
            i++;
        }


        /*
        for (int i = 0; i < m_actionStrings.size(); i++)
        {
            if (text == "Keybind")
            {
                std::string action = "";
                int code;

                fin >> action >> code;

                m_keyCodes[i] = code;
            }
        }
        */
    }
}

void Scene_Options::update()
{
    m_entityManager.update();
    if (m_awaitingKeybind)
    {
        sBindKey();
    }

    m_currentFrame++;
}

void Scene_Options::sBindKey()
{
    if (m_currentFrame > m_keybindFrame + 60)
    {
        std::cout << "Begin binding Key" << std::endl;

        for (int k = 0; k < sf::Keyboard::KeyCount; k++)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(k)))
            {
                std::cout << "Bound Action " << m_actionToBind << " to key code " << k << std::endl;
                m_awaitingKeybind = false;
                m_menuStrings2[m_selectedMenuIndex] = sCodeToString(k);
                m_keyCodes[m_selectedMenuIndex] = k;
            }
        }
    }
}

std::string Scene_Options::sCodeToString(int x)
{
    switch (x)
    {
    case 0:
        return "A";
        break;
    case 1:
        return "B";
        break;
    case 2:
        return "C";
        break;
    case 3:
        return "D";
        break;
    case 4:
        return "E";
        break;
    case 5:
        return "F";
        break;
    case 6:
        return "G";
        break;
    case 7:
        return "H";
        break;
    case 8:
        return "I";
        break;
    case 9:
        return "J";
        break;
    case 10:
        return "K";
        break;
    case 11:
        return "L";
        break;
    case 12:
        return "M";
        break;
    case 13:
        return "N";
        break;
    case 14:
        return "O";
        break;
    case 15:
        return "P";
        break;
    case 16:
        return "Q";
        break;
    case 17:
        return "R";
        break;
    case 18:
        return "S";
        break;
    case 19:
        return "T";
        break;
    case 20:
        return "U";
        break;
    case 21:
        return "V";
        break;
    case 22:
        return "W";
        break;
    case 23:
        return "X";
        break;
    case 24:
        return "Y";
        break;
    case 25:
        return "Z";
        break;
    case 26:
        return "0";
        break;
    case 27:
        return "1";
        break;
    case 28:
        return "2";
        break;
    case 29:
        return "3";
        break;
    case 30:
        return "4";
        break;
    case 31:
        return "5";
        break;
    case 32:
        return "6";
        break;
    case 33:
        return "7";
        break;
    case 34:
        return "8";
        break;
    case 35:
        return "9";
        break;
    case 36:
        return "Escape";
        break;
    case 37:
        return "LControl";
        break;
    case 38:
        return "LShift";
        break;
    case 39:
        return "LAlt";
        break;
    case 40:
        return "LSystem";
        break;
    case 41:
        return "RControl";
        break;
    case 42:
        return "RShift";
        break;
    case 43:
        return "RAlt";
        break;
    case 44:
        return "RSystem";
        break;
    case 45:
        return "Menu";
        break;
    case 46:
        return "[";
        break;
    case 47:
        return "]";
        break;
    case 48:
        return ";";
        break;
    case 49:
        return ",";
        break;
    case 50:
        return ".";
        break;
    case 51:
        return "'";
        break;
    case 52:
        return "/";
        break;
    case 53:
        return "Backslash";
        break;
    case 54:
        return "~";
        break;
    case 55:
        return "=";
        break;
    case 56:
        return "-";
        break;
    case 57:
        return "Space";
        break;
    case 58:
        return "Enter";
        break;
    case 59:
        return "Backspace";
        break;
    case 60:
        return "Tab";
        break;
    case 61:
        return "PageUp";
        break;
    case 62:
        return "PageDown";
        break;
    case 63:
        return "End";
        break;
    case 64:
        return "Home";
        break;
    case 65:
        return "Insert";
        break;
    case 66:
        return "Delete";
        break;
    case 67:
        return "Num +";
        break;
    case 68:
        return "Num -";
        break;
    case 69:
        return "Num *";
        break;
    case 70:
        return "Num /";
        break;
    case 71:
        return "Left";
        break;
    case 72:
        return "Right";
        break;
    case 73:
        return "Up";
        break;
    case 74:
        return "Down";
        break;
    case 75:
        return "Num 0";
        break;
    case 76:
        return "Num 1";
        break;
    case 77:
        return "Num 2";
        break;
    case 78:
        return "Num 3";
        break;
    case 79:
        return "Num 4";
        break;
    case 80:
        return "Num 5";
        break;
    case 81:
        return "Num 6";
        break;
    case 82:
        return "Num 7";
        break;
    case 83:
        return "Num 8";
        break;
    case 84:
        return "Num 9";
        break;
    case 85:
        return "F1";
        break;
    case 86:
        return "F2";
        break;
    case 87:
        return "F3";
        break;
    case 88:
        return "F4";
        break;
    case 89:
        return "F5";
        break;
    case 90:
        return "F6";
        break;
    case 91:
        return "F7";
        break;
    case 92:
        return "F8";
        break;
    case 93:
        return "F9";
        break;
    case 94:
        return "F10";
        break;
    case 95:
        return "F11";
        break;
    case 96:
        return "F12";
        break;
    case 97:
        return "F13";
        break;
    case 98:
        return "F14";
        break;
    case 99:
        return "F15";
        break;
    case 100:
        return "Pause";
        break;
    }
}

void Scene_Options::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "UP")
        {
            if (!m_awaitingKeybind)
            {
                if (m_selectedMenuIndex > 0) { m_selectedMenuIndex--; }
                else { m_selectedMenuIndex = m_menuStrings.size() - 1; }
            }
        }
        else if (action.name() == "DOWN")
        {
            if (!m_awaitingKeybind)
            {
                m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
            }
        }
        else if (action.name() == "POSITIVE")
        {
            if (!m_awaitingKeybind)
            {
                if (m_selectedMenuIndex == 10)
                {
                    if (m_menuStrings2[10] != "100")
                    {
                        int increaseSound = 25;
                        int oldVolume = std::stoi(m_menuStrings2[10]);
                        int newVolume = oldVolume + increaseSound;
                        m_menuStrings2[10] = std::to_string(newVolume);

                        sChangeMusicVolume(6.25f);
                    }
                }

                if (m_selectedMenuIndex == 11)
                {
                    if (m_menuStrings2[11] != "100")
                    {
                        int increaseSound = 25;
                        int oldVolume = std::stoi(m_menuStrings2[11]);
                        int newVolume = oldVolume + increaseSound;
                        m_menuStrings2[11] = std::to_string(newVolume);

                        sChangeSoundVolume(6.25);
                    }
                }

                if (m_selectedMenuIndex == 12)
                {
                    if (m_selectedMenuIndex2 > 0) { m_selectedMenuIndex2--; }
                    else { m_selectedMenuIndex2 = m_difficultyStrings.size() - 1; }
                    m_menuStrings2[m_selectedMenuIndex] = m_difficultyStrings[m_selectedMenuIndex2];
                }

                if (m_selectedMenuIndex < 10)
                {
                    m_actionToBind = m_actionStrings[m_selectedMenuIndex];
                    m_keybindFrame = m_currentFrame;
                    m_awaitingKeybind = true;
                }
            }
        }
        else if (action.name() == "NEGATIVE")
        {
            if (m_selectedMenuIndex == 10)
            {
                if (m_menuStrings2[10] != "0")
                {
                    int reduceSound = -25;
                    int oldVolume = std::stoi(m_menuStrings2[10]);
                    int newVolume = oldVolume + reduceSound;
                    m_menuStrings2[10] = std::to_string(newVolume);

                    sChangeMusicVolume(-6.25f);
                }
            }

            if (m_selectedMenuIndex == 11)
            {
                if (m_menuStrings2[11] != "0")
                {
                    int reduceSound = -25;
                    int oldVolume = std::stoi(m_menuStrings2[11]);
                    int newVolume = oldVolume + reduceSound;
                    m_menuStrings2[11] = std::to_string(newVolume);

                    sChangeSoundVolume(-6.25);
                }
            }

            if (m_selectedMenuIndex == 12)
            {
                m_selectedMenuIndex2 = (m_selectedMenuIndex2 + 1) % m_difficultyStrings.size();
                m_menuStrings2[m_selectedMenuIndex] = m_difficultyStrings[m_selectedMenuIndex2];
            }
        }
        else if (action.name() == "BACK")
        {
            if (!m_awaitingKeybind)
            {
                saveData(m_saveData);
                m_game->changeScene("MENU", nullptr, true);
            }
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

    m_menuText.setCharacterSize(30);

    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        m_menuText.setString(m_menuStrings[i]);
        m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color(100, 100, 100));
        m_menuText.setPosition(sf::Vector2f(20, 150 + i * 30));
        m_game->window().draw(m_menuText);

        m_menuText.setString(m_menuStrings2[i]);
        m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color(100, 100, 100));
        m_menuText.setPosition(sf::Vector2f(600, 150 + i * 30));
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