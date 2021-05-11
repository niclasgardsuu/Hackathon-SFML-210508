#ifndef CAR_CPP
#define CAR_CPP

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

        void skid(sf::RenderTexture &surface) {
            if(1) {//length(velocity) > 1) {
                sf::CircleShape skid;
                float skidRadius = car.getSize().x/5;
                skid.setRadius(skidRadius);
                skid.setOrigin(skidRadius,skidRadius);
        
                sf::Vector2f carSize = car.getSize();
                sf::Vector2f carCorners[4] = { //[0] topleft, [1] topright, [2] downleft, [3] downright
                    sf::Vector2f(position.x + (carSize.y/2.4)*sin(angle - (PI/2)) + (carSize.x/2.4)*sin(angle), 900-position.y + (carSize.y/2.4)*cos(angle - (PI/2)) + (carSize.x/2.4)*cos(angle)),
                    sf::Vector2f(position.x + (carSize.y/2.4)*sin(angle - (PI/2)) - (carSize.x/2.4)*sin(angle), 900-position.y + (carSize.y/2.4)*cos(angle - (PI/2)) - (carSize.x/2.4)*cos(angle)),
                    sf::Vector2f(position.x + -1*(carSize.y/2.4)*sin(angle - (PI/2)) + (carSize.x/2.4)*sin(angle), 900-position.y + -1*(carSize.y/2.4)*cos(angle - (PI/2)) + (carSize.x/2.4)*cos(angle)),
                    sf::Vector2f(position.x + -1*(carSize.y/2.4)*sin(angle - (PI/2)) - (carSize.x/2.4)*sin(angle), 900-position.y + -1*(carSize.y/2.4)*cos(angle - (PI/2)) - (carSize.x/2.4)*cos(angle))
                };

                
                float velocityAngle = atan(velocity.y/velocity.x);
                if(velocity.x < 0 && velocity.y < 0) velocityAngle += PI;
                else if(velocity.x < 0) velocityAngle += PI;
                else if(velocity.y < 0) velocityAngle += 2*PI;

                float angleDifference = fmod((velocityAngle - angle) + 2*PI, 2*PI);
                if(angleDifference > PI) angleDifference = (2*PI) - angleDifference;
                std::cout << angleDifference;
                std::cout << "\n";
                double skidOpacity = (1 - fabs((angleDifference / (PI/2.f))-1)) / 2.f;
                std::cout << abs(-1.3337f);
                std::cout << "\n";
                std::cout << skidOpacity;
                std::cout << "\n";
                std::cout << "\n";
                std::cout << "\n";
                skid.setFillColor(sf::Color(0,0,0,skidOpacity * 200 * (length(velocity)/5)));
                for(int i = 0; i < 4; i++) {
                    skid.setPosition(carCorners[i]);
                    surface.draw(skid);
                }
            }
        }

        void update(sf::Vector2f checkpoints[CHECKPOINT_COUNT], sf::Vector2f finishLine) {
            if(angle < 0)
                angle = 2*PI - fmod(angle,2*PI);
            else
                angle = fmod(angle,2*PI);
            
            if(angleVel < -0.06) angleVel = -0.06;
            if(angleVel > 0.06) angleVel = 0.06;
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

#endif