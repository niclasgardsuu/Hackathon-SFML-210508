#include <SFML/Graphics.hpp>
#include <../src/Track.cpp>
#include <../src/Car.cpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(900,900), "RaceFever!");
    window.setFramerateLimit(60);
    Track track(window);
    sf::RectangleShape background;
    background.setFillColor(sf::Color(15,125,30));
    background.setPosition(0,0);
    background.setSize(sf::Vector2f((float)window.getSize().x,(float)window.getSize().y));
    Car car(sf::Vector2f(10,30), sf::Vector2f(750,450));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if(event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                if (event.key.code == sf::Keyboard::Up)
                    car.startAcc();
                if (event.key.code == sf::Keyboard::Down)
                    car.startBrake();
                if (event.key.code == sf::Keyboard::Left)
                    car.startTurning('l');
                if (event.key.code == sf::Keyboard::Right)
                    car.startTurning('r');
            } else if(event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Up)
                    car.stopAcc();
                if (event.key.code == sf::Keyboard::Down)
                    car.stopBrake();
                if (event.key.code == sf::Keyboard::Left)
                    car.stopTurning('l');
                if (event.key.code == sf::Keyboard::Right)
                    car.stopTurning('r');
            }
        }

        sf::Vector2f checkpoints[CHECKPOINT_COUNT];
        for(int i = 0; i < CHECKPOINT_COUNT; i++) {
            checkpoints[i] = track.getCheckpoint(i);
        }
        car.update(checkpoints, track.getFinishPoint());

        window.draw(background);
        track.render(window);
        car.render(window);
        window.display();
    }

    return 0;
}