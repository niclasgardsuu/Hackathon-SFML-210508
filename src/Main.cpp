#include <SFML/Graphics.hpp>
#include <../src/Track.cpp>
#include <../src/Car.cpp>
#include <../src/EnemyCar.cpp>
#include <iostream>

#define ENEMY_COUNT 5

int main()
{
    sf::RenderWindow window(sf::VideoMode(900,900), "RaceFever!");
    window.setFramerateLimit(60);
    Track track(window);
    sf::RectangleShape background;
    background.setFillColor(sf::Color(120,190,150));
    sf::Texture grass;
    grass.loadFromFile("./img/grass.png");
    background.setTexture(&grass);
    background.setPosition(0,0);
    background.setSize(sf::Vector2f((float)window.getSize().x,(float)window.getSize().y));
    Car car(sf::Vector2f(20,35), sf::Vector2f(750,450));
    EnemyCar enemies[ENEMY_COUNT] = {
        EnemyCar(sf::Vector2f(20,35), sf::Vector2f(700+(rand()%50),400+(rand()%50)), 1),
        EnemyCar(sf::Vector2f(20,35), sf::Vector2f(700+(rand()%50),400+(rand()%50)), 2),
        EnemyCar(sf::Vector2f(20,35), sf::Vector2f(700+(rand()%50),400+(rand()%50)), 3),
        EnemyCar(sf::Vector2f(20,35), sf::Vector2f(700+(rand()%50),400+(rand()%50)), 4),
        EnemyCar(sf::Vector2f(20,35), sf::Vector2f(700+(rand()%50),400+(rand()%50)), 5)
    };
    sf::RenderTexture skidSurface;
    skidSurface.create(900,900);

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
        int checkpointsIndex[CHECKPOINT_COUNT];
        for(int i = 0; i < CHECKPOINT_COUNT; i++) {
            checkpoints[i] = track.getCheckpoint(i);
            checkpointsIndex[i] = track.getCheckpointIndex(i);
        }
        
        for(int i = 0; i < ENEMY_COUNT; i++) {
            enemies[i].setTarget(track.getTrackPoint(enemies[i].decideTargetIndex(window,checkpoints,checkpointsIndex)));
            enemies[i].configureActions();
            enemies[i].setFrictionFactor(track.getGroundFriction(enemies[i].getPos()));
            enemies[i].update(checkpoints, track.getFinishPoint());
            enemies[i].skid(skidSurface);
        }
        

        car.setFrictionFactor(track.getGroundFriction(car.getPos()));
        car.update(checkpoints, track.getFinishPoint());
        car.skid(skidSurface);

        window.draw(background);
        track.render(window);
        const sf::Texture &skid = skidSurface.getTexture();
        sf::Sprite skidd(skid);
        window.draw(skidd);
        car.render(window);
        
        for(int i = 0; i < ENEMY_COUNT; i++) {
            enemies[i].render(window);
        }
        
        window.display();
    }
    delete enemies;
    return 0;
}