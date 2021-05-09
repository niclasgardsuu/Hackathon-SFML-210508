#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#define PI 3.14159265
#define CHECKPOINT_COUNT 4
class Car {
    protected:
        sf::Vector2f position, velocity, acceleration;
        bool accelerating, turningRight, turningLeft, braking;
        float angle, angleVel, angleAcc, frictionFactor;
        bool checkpointsPassed[CHECKPOINT_COUNT];
        int currentLap;
        sf::RectangleShape car; 
        sf::Texture texture;
    public:
        Car(sf::Vector2f size, sf::Vector2f startPos) {
            car.setSize(size);
            car.setOrigin(size.x/2,size.y/2);
            position = startPos;
            velocity = sf::Vector2f(0.f,0.f);
            acceleration = sf::Vector2f(0.1f,0.1f);
            currentLap = 0;
            for(int i = 0; i < CHECKPOINT_COUNT; i++) {
                checkpointsPassed[i] = false;
            }
            angle = PI/4;
            angleVel = 0;
            angleAcc = 0.005;
            frictionFactor = 0;
            accelerating = false;
            turningRight = false;
            turningLeft = false;
            braking = false;
            texture.loadFromFile("./img/car.png");
        }

        void move() {
            position += velocity;
        }

        void accelerate() {
            velocity.x += cos(angle)*acceleration.x*(1-frictionFactor);
            velocity.y += sin(angle)*acceleration.y*(1-frictionFactor);
        }

        void startAcc() {
            accelerating = true;
        }

        void stopAcc() {
            accelerating = false;
        }

        void startBrake() {
            braking = true;
        }

        void stopBrake() {
            braking = false;
        }

        void startTurning(char direction) {
            if(direction == 'r') turningRight = true;
            else if(direction == 'l') turningLeft = true;
        }

        void stopTurning(char direction) {
            if(direction == 'r') turningRight = false;
            else if(direction == 'l') turningLeft = false;
        }

        void setFrictionFactor(float factor) {
            frictionFactor = factor;
        } 

        sf::Vector2f getPos() {
            return position;
        }

        float length(sf::Vector2f vector) {
            return sqrt(pow(vector.x,2) + pow(vector.y,2));
        }

        void update(sf::Vector2f checkpoints[CHECKPOINT_COUNT], sf::Vector2f finishLine) {
            if(turningRight && std::abs(angleVel) < 0.06) angleVel += angleAcc;
            if(turningLeft && std::abs(angleVel) < 0.06) angleVel -= angleAcc;
            if(accelerating && length(velocity) < 5*(1-frictionFactor)) accelerate();
            
            velocity.x *= (0.99-(frictionFactor*0.02));
            velocity.y *= (0.99-(frictionFactor*0.02));
            if(braking) {
                velocity.x *= 0.97;
                velocity.y *= 0.97;
            }
            for(int i = 0; i < CHECKPOINT_COUNT; i++) {
                if(length(position - checkpoints[i]) < 50) {
                    checkpointsPassed[i] = true;
                }
            }
            int allCheckpoints = 0;
            for(int i = 0; i < CHECKPOINT_COUNT; i++) {
                allCheckpoints += !checkpointsPassed[i];
            }
            if(!allCheckpoints) {
                if(length(position - finishLine) < 50) {
                    for(int i = 0; i < CHECKPOINT_COUNT; i++) {
                        checkpointsPassed[i] = false;
                    }
                    currentLap++;
                    std::cout << currentLap;
                    std::cout << "\n";
                }
            }

            //TODO: minska angle om den inte ska ökas, inte sätta till 0 bara
            if(!(turningLeft ^ turningRight)) angleVel = 0;
            
            angle += angleVel;
            move();
        }

        void render(sf::RenderWindow &window) {
            sf::CircleShape shadow;
            shadow.setRadius(18);
            shadow.setOrigin(18,18);
            shadow.setFillColor(sf::Color(0,0,0,90));
            shadow.setPosition(position);
            window.draw(shadow);
            car.setPosition(position);
            car.setTexture(&texture);
            car.setRotation((angle*180/PI)+90);
            window.draw(car);

            sf::CircleShape checkpointShape;
            checkpointShape.setRadius(10);
            for(int i = 0; i < CHECKPOINT_COUNT; i++) {
                checkpointShape.setPosition(sf::Vector2f(i*25,0));
                if(checkpointsPassed[i]) checkpointShape.setFillColor(sf::Color(50,200,50));
                else checkpointShape.setFillColor(sf::Color(90,90,90));
                window.draw(checkpointShape);
            }
            
            sf::CircleShape lapShape;
            lapShape.setRadius(5);
            lapShape.setFillColor(sf::Color(120,100,20));
            for(int i = 0; i < currentLap; i++) {
                lapShape.setPosition(sf::Vector2f(i*12 + 4*25,0));
                window.draw(lapShape);
            }
        }
};