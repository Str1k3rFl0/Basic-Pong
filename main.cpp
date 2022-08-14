#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <cstdlib>

class Bat
{
private:
    sf::Vector2f m_Position;
    sf::RectangleShape m_Shape;
    float m_Speed = 800.0f;
    bool m_MovingUp = false;
    bool m_MovingDown = false;

public:
    Bat(float startX, float startY);
    sf::FloatRect getPosition();
    sf::RectangleShape getShape();
    void moveUp();
    void moveDown();
    void stopUp();
    void stopDown();
    void update(sf::Time dt);
};

Bat::Bat(float startX, float startY)
{
    m_Position.x = startX;
    m_Position.y = startY;

    m_Shape.setSize(sf::Vector2f(5, 80));
    m_Shape.setPosition(m_Position);
}

sf::FloatRect Bat::getPosition()
{
    return m_Shape.getGlobalBounds();
}

sf::RectangleShape Bat::getShape()
{
    return m_Shape;
}

void Bat::moveUp()
{
    m_MovingUp = true;
}

void Bat::moveDown()
{
    m_MovingDown = true;
}

void Bat::stopUp()
{
    m_MovingUp = false;
}

void Bat::stopDown()
{
    m_MovingDown = false;
}

void Bat::update(sf::Time dt)
{
    if (m_MovingUp)
    {
        m_Position.y -= m_Speed * dt.asSeconds();
    }
    if (m_MovingDown)
    {
        m_Position.y += m_Speed * dt.asSeconds();
    }

    m_Shape.setPosition(m_Position);
}

class Ball
{
private:
    sf::Vector2f m_Position;
    sf::RectangleShape m_Shape;

    float m_Speed = 1500.0f;
    float m_DirectionX = .2f;
    float m_DirectionY = .2f;

public:
    Ball(float startX, float startY);
    sf::FloatRect getPosition();
    sf::RectangleShape getShape();
    void reboundSides();
    void reboundTop();
    void reboundBottom();
    void update(sf::Time dt);
};

Ball::Ball(float startX, float startY)
{
    m_Position.x = startX;
    m_Position.y = startY;

    m_Shape.setSize(sf::Vector2f(10, 10));
    m_Shape.setPosition(m_Position);
}

sf::FloatRect Ball::getPosition()
{
    return m_Shape.getGlobalBounds();
}

sf::RectangleShape Ball::getShape()
{
    return m_Shape;
}

void Ball::reboundSides()
{
    m_DirectionX = -m_DirectionX;
}

void Ball::reboundTop()
{
    m_DirectionY = -m_DirectionY;
}

void Ball::reboundBottom()
{
    m_DirectionY = -m_DirectionY;
}

void Ball::update(sf::Time dt)
{
    m_Position.y += m_DirectionY * m_Speed * dt.asSeconds();
    m_Position.x += m_DirectionX * m_Speed * dt.asSeconds();

    m_Shape.setPosition(m_Position);
}

// Intro Game
void intro();

int main()
{
    const unsigned WINDOW_WIDTH = 1920;
    const unsigned WINDOW_HEIGHT = 1080;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pong", sf::Style::Fullscreen);

    Bat bat1(5, 503), bat2(1903, 543);
    Ball ball(1920 / 2, 0);

    int livesPlayer1 = 3;
    int livesPlayer2 = 3;

    // Hud player 1
    sf::Text hud;
    sf::Font font;
    font.loadFromFile("fonts/AltonaSans-Regular.ttf");
    hud.setFont(font);
    hud.setCharacterSize(45);
    hud.setFillColor(sf::Color(255, 127, 0));
    hud.setPosition(20, 20);

    // Hud player 2
    sf::Text hud2;
    sf::Font font2;
    font2.loadFromFile("fonts/AltonaSans-Regular.ttf");
    hud2.setFont(font2);
    hud2.setCharacterSize(45);
    hud2.setFillColor(sf::Color(255, 127, 0));
    hud.setPosition(1790, 0);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }

        // Player 1
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            bat1.moveUp();
        }
        else
        {
            bat1.stopUp();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            bat1.moveDown();
        }
        else
        {
            bat1.stopDown();
        }

        // Player 2
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            bat2.moveUp();
        }
        else
        {
            bat2.stopUp();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            bat2.moveDown();
        }
        else
        {
            bat2.stopDown();
        }

        // Physics
        // Bottom collision
        if (ball.getPosition().top > window.getSize().y)
        {
            ball.reboundBottom();
        }

        // Top collision
        if (ball.getPosition().top < 0)
        {
            ball.reboundTop();
        }

        // Sides collision
        if (ball.getPosition().left < 0 ||
            ball.getPosition().left + ball.getPosition().width >
            window.getSize().x)
        {
            ball.reboundSides();
            if (ball.getPosition().left < 0)
            {
                livesPlayer2--;
                if (livesPlayer2 < 1)
                {
                    livesPlayer2 = 3;
                }
            }

            if (ball.getPosition().left + ball.getPosition().width >
                window.getSize().x)
            {
                livesPlayer1--;
                if (livesPlayer1 < 1)
                {
                    livesPlayer1 = 3;
                }
            }
        }

        // Ball hitting bats
        if (ball.getPosition().intersects(bat1.getPosition()))
        {
            ball.reboundSides();
        }
        if (ball.getPosition().intersects(bat2.getPosition()))
        {
            ball.reboundSides();
        }

        // Update the time
        sf::Time dt = clock.restart();
        bat1.update(dt);
        bat2.update(dt);
        ball.update(dt);
        std::stringstream ss;
        std::stringstream ss2;
        ss << "Lives: " << livesPlayer1;
        ss2 << "Lives: " << livesPlayer2;
        hud.setString(ss.str());
        hud2.setString(ss2.str());

        window.clear();
        window.draw(hud);
        window.draw(hud2);
        window.draw(bat1.getShape());
        window.draw(bat2.getShape());
        window.draw(ball.getShape());
        window.display();
    }

    return 0;
}
