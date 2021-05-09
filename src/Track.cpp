#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#define PI 3.14159265
#define POINTS 100
#define OFFSET_FACTOR 50
#define TRACK_WIDTH 50
#define CHECKPOINT_COUNT 4

class Track {
    private:
        sf::Vector2f trackPoints[POINTS];
        sf::Vector2f checkpoints[CHECKPOINT_COUNT];
        int checkpointsIndex[CHECKPOINT_COUNT];
        sf::Vector2f finishLine;
    public:
        void initPoints(sf::RenderWindow &window) {
            sf::Vector2f center(window.getSize().x*0.5f, window.getSize().y*0.5f);
            unsigned int radius;
            if(center.x < center.y) {
                radius = center.x*0.6;
            } else {
                radius = center.y*0.6;
            }
            for(int i = 0; i < POINTS; i++) {
                trackPoints[i] = sf::Vector2f(cos((i*PI)/50)*radius + center.x, sin((i*PI)/50)*radius + center.y);
            }
        }

        void offsetPoints() {
            srand(time(NULL));
            float offset = 0;
            //offset the first half of points
            for(int i = 0; i < POINTS/2; i++) {
                offset += ((float)(rand()%(100*OFFSET_FACTOR))/100)-((float)OFFSET_FACTOR/2);
                sf::Vector2f offsetVector(cos((i*PI)/50)*offset,sin((i*PI)/50)*offset);
                trackPoints[i] = trackPoints[i] + offsetVector;
                //mirror the offsetted points to make the track intersect with itself
                unsigned int mirrorIndex = POINTS-1-i;
                sf::Vector2f mirrorOffsetVector(cos((mirrorIndex*PI)/50)*offset,sin((mirrorIndex*PI)/50)*offset);
                trackPoints[POINTS-1-i] = trackPoints[POINTS-1-i] + offsetVector;
            }
        } 

        float length(sf::Vector2f vector) {
            return sqrt(pow(vector.x,2) + pow(vector.y,2));
        }

        Track(sf::RenderWindow &window) {
            initPoints(window);
            offsetPoints();
            //TODO: smooth out the track, take average distance from center from 5 closeby points
            //TODO: add rocks on track on the edge of the circle that 
            //      represents the road so you avoid it by going in the middle of the road
            //TODO: add checkpoints every 1/5 of track, and add finish line
            float currentMostOffset = 0;
            int currentMostOffsetIndex = 0;
            sf::Vector2f center(window.getSize().x/2,window.getSize().y/2);
            for(int i = 0; i < POINTS; i++) {
                if(currentMostOffset < length(trackPoints[i]-center)) {
                    currentMostOffset = length(trackPoints[i]-center);
                    currentMostOffsetIndex = i;
                }
            }
            //The most offset point will have a checkpoint, and then 4 other checkpoints
            //are spaced evenly.

            for(int i = 0; i < CHECKPOINT_COUNT+1; i++) {
                int index = ((currentMostOffsetIndex+POINTS)-(POINTS/5*i))%POINTS;
                if(index < 91 && index > 10) {
                    std::cout << index;
                    std::cout << "\n";
                    if(index <= 30) {
                        checkpointsIndex[0] = index;
                        checkpoints[0] = trackPoints[index];
                    }
                    else if(index <= 50) {
                        checkpointsIndex[1] = index;
                        checkpoints[1] = trackPoints[index];
                    }
                    else if(index <= 70) {
                        checkpointsIndex[2] = index;
                        checkpoints[2] = trackPoints[index];
                    }
                    else if (index <= 90){
                        checkpointsIndex[3] = index;
                        checkpoints[3] = trackPoints[index];
                    }
                }
            }
            for(int i = 0; i < CHECKPOINT_COUNT; i++) {
                    std::cout << "\n";
                    std::cout << checkpointsIndex[i];
                    std::cout << "\n";
            }
            //Finish line, and also starting line, is always gonna be the first point in the array
            finishLine = trackPoints[0];
        }
    
        sf::Vector2f getTrackPoint(int index) {
            return trackPoints[index];
        }

        sf::Vector2f getCheckpoint(int index) {
            return checkpoints[index];
        }
        int getCheckpointIndex(int index) {
            return checkpointsIndex[index];
        }

        sf::Vector2f getFinishPoint() {
            return finishLine;
        }

        float getGroundFriction(sf::Vector2f pos) {
            for(int i = 0; i < POINTS; i++) {
                if(length(pos - trackPoints[i]) < TRACK_WIDTH) return 0;
            }
            return 0.3;
        }

        void render(sf::RenderWindow &window) {
            sf::CircleShape point;

            //road blocks
            sf::Color color(25,25,25);
            point.setFillColor(color);
            point.setRadius(TRACK_WIDTH);
            point.setOrigin(sf::Vector2f(point.getRadius(),point.getRadius()));
            for(int i = 0; i < POINTS; i++) {
                point.setPosition(trackPoints[i]);
                window.draw(point);
            }

            //white marks;
            sf::RectangleShape mark;
            mark.setFillColor(sf::Color::White);
            mark.setSize(sf::Vector2f(5,20));
            mark.setOrigin(10,2.5);
            for(int i = 3; i < POINTS-3; i+=3) {
                float angle = atan((trackPoints[i-2].y - trackPoints[i+2].y)/(trackPoints[i-2].x - trackPoints[i+2].x));
                mark.setRotation(angle*180/PI + 90);
                mark.setPosition(trackPoints[i]);
                window.draw(mark);
            }

            //checkpoints
            sf::Color color2(90,90,25);
            sf::RectangleShape checkpointBar;
            checkpointBar.setSize(sf::Vector2f(10,TRACK_WIDTH*2 + 20));
            checkpointBar.setOrigin(5,TRACK_WIDTH + 10);
            checkpointBar.setFillColor(color2);
            for(int i = 0; i < CHECKPOINT_COUNT; i++) {
                checkpointBar.setPosition(checkpoints[i]);
                checkpointBar.setRotation(180*atan((checkpoints[i].y - window.getSize().y/2)/(checkpoints[i].x - window.getSize().x/2))/PI + 90);
                window.draw(checkpointBar);
            }
            //finish
            sf::Color color3(200,170,25);
            sf::RectangleShape finishBar;
            finishBar.setSize(sf::Vector2f(10,TRACK_WIDTH*2 + 20));
            finishBar.setOrigin(5,TRACK_WIDTH + 10);
            finishBar.setFillColor(color3);
            finishBar.setPosition(finishLine);
            finishBar.setRotation(180*atan((finishLine.y - window.getSize().y/2)/(finishLine.x - window.getSize().x/2))/PI + 90);
            window.draw(finishBar);
            

        }

        ~Track() {return;}
};