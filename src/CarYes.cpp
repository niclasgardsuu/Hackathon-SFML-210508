#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#define PI 3.14159265
#define CHECKPOINT_COUNT 4
class CarYes {
    protected:
        sf::Vector2f position, velocity, acceleration;
        bool accelerating, turningRight, turningLeft, braking;
        float angle, angleVel, angleAcc, frictionFactor;
        bool checkpointsPassed[CHECKPOINT_COUNT];
        int currentLap;
        sf::RectangleShape car; 
    public:
        CarYes(sf::Vector2f size, sf::Vector2f startPos) {
            car.setSize(size);
            car.setFillColor(sf::Color::Blue);
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
            if(direction == 'r') {
                turningRight = true;
                }
            else if(direction == 'l') {
                turningLeft = true;
            }
        }

        void stopTurning(char direction) {
            if(direction == 'r') {
                turningRight = false;
            }
            else if(direction == 'l') {
                turningLeft = false;
            }
        }


        float length(sf::Vector2f vector) {
            return sqrt(pow(vector.x,2) + pow(vector.y,2));
        }

        void update(sf::Vector2f checkpoints[CHECKPOINT_COUNT], sf::Vector2f finishLine) {
            if(angleVel < -0.06) angleVel = -0.06;
            if(angleVel > 0.06) angleVel = 0.06;
            if(turningRight) angleVel += angleAcc;
            if(turningLeft) angleVel -= angleAcc;
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
                if(length(position - finishLine) < 25) {
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
            car.setPosition(position);
            car.setRotation((angle*180/PI)+90);
            window.draw(car);
            sf::Vector2f newPos = position;
            newPos.x += cos(angle)*10;
            newPos.y += sin(angle)*10;
            car.setPosition(newPos);
            window.draw(car);
        }
};