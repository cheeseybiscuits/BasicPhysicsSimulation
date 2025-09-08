#include "SFML/Graphics.hpp"
#include <iostream>

const int width = 1280;
const int height = 720;
const int size = 100;
float a = 980.0; // pixels/sec
float y_0 = -500;

int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height), "window", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    sf::RectangleShape rectangle;
    sf::RectangleShape pull;

    sf::Vector2f rectanglePos(640, 400 + y_0);
    rectangle.setPosition(rectanglePos);
    rectangle.setOrigin(sf::Vector2f(size / 2, size / 2));
    rectangle.setSize(sf::Vector2f(size, size));
    rectangle.setFillColor(sf::Color::Black);
    pull.setFillColor(sf::Color::Black);

    sf::Clock clock;

    float v_y = 0;
    float v_x = -200;
    bool pullActive = false;

    while (window.isOpen())
    {
        window.clear(sf::Color::White);
        pullActive = false; // for left click pull

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close(); // checks if windows closes, it closes (rather than freezing)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close(); // esc to close
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            float dir_x = mousePos.x - rectanglePos.x;
            float dir_y = mousePos.y - rectanglePos.y;

            // calculate length of rectangle
            float length = std::sqrt((dir_x * dir_x) + (dir_y * dir_y));
            pull.setSize(sf::Vector2f(length, 2.5));

            // set origin of pull to left-middle and set position to middle of square
            pull.setOrigin(0, pull.getSize().y / 2);
            pull.setPosition(sf::Vector2f(rectanglePos.x, rectanglePos.y));

            // calculate angle between y and x components and convert to degrees
            float angle = std::atan2(dir_y, dir_x) * 180 / 3.14159265;
            pull.setRotation(angle);

            v_y += (mousePos.y - rectanglePos.y) * 0.25;
            v_y += a * 0.05;
            v_x += (mousePos.x - rectanglePos.x) * 0.125;

            pullActive = true;
        }

        // physics stuff
        float dt = clock.restart().asSeconds();

        rectanglePos.y += v_y * (dt);
        v_y += a * (dt);
        rectanglePos.x += v_x * (dt);

        // bounce when hitting walls and floor (not roof)
        if (rectanglePos.y > height - size / 2)
        {
            rectanglePos.y = height - size / 2;
            v_y = -v_y * 0.45;
        }

        if ((rectanglePos.x < size / 2 || rectanglePos.x > width - size / 2))
        {
            rectanglePos.x < size / 2 ? rectanglePos.x = size / 2 : NULL;
            rectanglePos.x > width - size / 2 ? rectanglePos.x = width - size / 2 : NULL;
            v_x = -v_x * 0.35;

            if (abs(v_x) < 5.0)
            {
                v_x = 0;
            }
        }

        // update pos
        rectangle.setPosition(rectanglePos);

        // render
        window.draw(rectangle);
        if (pullActive)
        {
            window.draw(pull);
        }
        window.display();
    }

    return 0;
}
