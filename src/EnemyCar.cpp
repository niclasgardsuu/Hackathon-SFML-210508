#include <SFML/Graphics.hpp>
#include <../src/Car.cpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#define PI 3.14159265
#define POINTS 100
#define CHECKPOINT_COUNT 4

class EnemyCar : public Car {
    private: 
        sf::Vector2f target;
        int index;
        sf::Texture texture;
    public:
        EnemyCar(sf::Vector2f size, sf::Vector2f startPos, int enemyIndex) : Car(size,startPos) {
            target = startPos;
            index = enemyIndex;
            texture.loadFromFile("./img/AIcar.png");
        }

        int decideTargetIndex(sf::RenderWindow &window, sf::Vector2f checkpoints[4], int trackCheckpointsIndex[4]) {
            startAcc();
            //first need to find current index we're on
            sf::Vector2f center(window.getSize().x/2, window.getSize().y/2);
            sf::Vector2f centerToPosition = position - center;
            float angleFromCenter = atan(centerToPosition.y/centerToPosition.x);
            if(centerToPosition.x < 0 && centerToPosition.y < 0) angleFromCenter += PI;
            else if(centerToPosition.x < 0) angleFromCenter += PI;
            else if(centerToPosition.y < 0) angleFromCenter += 2*PI;
            int currentIndex = (angleFromCenter/(2*PI))*POINTS;
            //then the index of the direction we want to head in
            int nextIndex = (currentIndex + (int)(POINTS/8));
            //om "långt" bakom närmsta checkpoint, target pi/8 framför nuvarande index
            int indexOfNextCheckpoint = 99;
            for(int i = 0; i < CHECKPOINT_COUNT; i++) {
                if(!checkpointsPassed[i]) {
                    indexOfNextCheckpoint = trackCheckpointsIndex[i];
                    break;
                }
            }
            if(indexOfNextCheckpoint > nextIndex && indexOfNextCheckpoint - nextIndex < 20) {
                return nextIndex;
            }
            else {//alltså current position nära/framför checkpoint som jag ska ta, då blir 
                //target på den checkpointen eftersom nextIndex hade lett ifrån checkpointen
                return indexOfNextCheckpoint;
            }
            //if there are no checkpoints left, the initialized value 0
            //will be leading the car to the finish line, which is good
        }

        void setTarget(sf::Vector2f newTarget) {
            target = newTarget;
        } 

        void configureActions() {
            sf::Vector2f posToTarget = target - position;
            float angleToTarget = atan(posToTarget.y/posToTarget.x);
            if(posToTarget.x < 0 && posToTarget.y < 0) angleToTarget += PI;
            else if(posToTarget.x < 0) angleToTarget += PI;
            else if(posToTarget.y < 0) angleToTarget += 2*PI;

            /*if(angle < 0)
                angle = 2*PI - fmod(angle,2*PI);
            else
                angle = fmod(angle,2*PI);*/
                

            float velocityAngle = atan(velocity.y/velocity.x);
            if(velocity.x < 0 && velocity.y < 0) velocityAngle += PI;
            else if(velocity.x < 0) velocityAngle += PI;
            else if(velocity.y < 0) velocityAngle += 2*PI;

            float optimalAngle = angleToTarget;
            if(abs(angleToTarget - velocityAngle) < PI/2 && length(velocity) > 2) {
                if(velocityAngle < angleToTarget && angleToTarget - velocityAngle < PI) optimalAngle = angleToTarget*2 - velocityAngle;
                else if(velocityAngle < angleToTarget && angleToTarget - velocityAngle >= PI) optimalAngle = angleToTarget-(2*PI - angleToTarget) -velocityAngle;
                else if(velocityAngle >= angleToTarget && velocityAngle - angleToTarget < PI) optimalAngle = angleToTarget*2 - velocityAngle;
                else if(velocityAngle >= angleToTarget && velocityAngle - angleToTarget >= PI) optimalAngle = angleToTarget-(2*PI - angleToTarget) -velocityAngle;
            }

            float leftoptimalAngle = 0;
            if(optimalAngle > angle) {
                leftoptimalAngle = angle + (2*PI - optimalAngle);
            } else {
                leftoptimalAngle = angle - optimalAngle;
            }

            float rightoptimalAngle = 0;
            if(optimalAngle > angle) {
                rightoptimalAngle = optimalAngle - angle;
            } else {
                rightoptimalAngle = optimalAngle + (2*PI - angle);
            }
            float velLeftoptimalAngle = 0;
            if(optimalAngle > angle) {
                velLeftoptimalAngle = velocityAngle + (2*PI - optimalAngle);
            } else {
                velLeftoptimalAngle = velocityAngle - optimalAngle;
            }

            float velRightoptimalAngle = 0;
            if(optimalAngle > velocityAngle) {
                velRightoptimalAngle = optimalAngle - velocityAngle;
            } else {
                velRightoptimalAngle = optimalAngle + (2*PI - velocityAngle);
            }
            if(velRightoptimalAngle > 0.9 && velLeftoptimalAngle > 0.9 && length(velocity) > 3.5) {
                startBrake();
                stopAcc();
            } else {
                stopBrake();
                startAcc();
            }

            if(rightoptimalAngle < leftoptimalAngle) {
                //if(velRightoptimalAngle < PI && velRightoptimalAngle*3 > leftoptimalAngle) {
                //    stopTurning('l');
                //    startTurning('r');
                //} else {
                    stopTurning('l');
                    startTurning('r');
                //}
            } else {
                //if(velLeftoptimalAngle < PI && velLeftoptimalAngle*3 > rightoptimalAngle) {
                //    stopTurning('r');
                //    startTurning('l');
                //} else {
                    stopTurning('r');
                    startTurning('l');
                //}
            }
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

            sf::CircleShape targetShape;
            targetShape.setFillColor(sf::Color(120,10,10,128));
            targetShape.setOrigin(20,20);
            targetShape.setRadius(20);
            targetShape.setPosition(target);
            window.draw(targetShape);

            sf::CircleShape checkpointShape;
            checkpointShape.setRadius(10);
            for(int i = 0; i < CHECKPOINT_COUNT; i++) {
                checkpointShape.setPosition(sf::Vector2f(i*25,index*50 + 50));
                if(checkpointsPassed[i]) checkpointShape.setFillColor(sf::Color(50,200,50));
                else checkpointShape.setFillColor(sf::Color(90,90,90));
                window.draw(checkpointShape);
            }
            
            sf::CircleShape lapShape;
            lapShape.setRadius(5);
            lapShape.setFillColor(sf::Color(120,100,20));
            for(int i = 0; i < currentLap; i++) {
                lapShape.setPosition(sf::Vector2f(i*12 + 4*25,50 + index*50));
                window.draw(lapShape);
            }
        }
        ~EnemyCar() {return;}
};