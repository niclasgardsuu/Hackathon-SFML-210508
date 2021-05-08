#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#define POINTS 100
#define PI 3.14159265
#define OFFSET_FACTOR 50
#define CHECKPOINT_COUNT 4

class Track {
    private:
        sf::Vector2f trackPoints[POINTS];
        sf::Vector2f checkpoints[CHECKPOINT_COUNT];
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

            //j is used because we want to pick out one of the checkpoints lcoses to the finsih, so
            //we remove ignore one of the items we look at but j is then decreased to still keep track
            //of where to store the ones we actually want
            int j = 0;
            for(int i = 0; i < 5; i++) {
                int index = ((currentMostOffsetIndex+POINTS)-(POINTS/5*i))%POINTS;
                if(index < 91 && index > 10) {
                    checkpoints[j] = trackPoints[index];
                    j++;
                }
            }
            //Finish line, and also starting line, is always gonna be the first point in the array
            finishLine = trackPoints[0];
        }






        void render(sf::RenderWindow &window) {
            sf::CircleShape point;

            //road blocks
            sf::Color color(25,25,25);
            point.setFillColor(color);
            point.setRadius(50);
            point.setOrigin(sf::Vector2f(point.getRadius(),point.getRadius()));
            for(int i = 0; i < POINTS; i++) {
                point.setPosition(trackPoints[i]);
                window.draw(point);
            }
            //checkpoints
            sf::Color color2(90,90,25);
            point.setFillColor(color2);
            point.setRadius(15);
            point.setOrigin(sf::Vector2f(point.getRadius(),point.getRadius()));
            for(int i = 0; i < CHECKPOINT_COUNT; i++) {
                point.setPosition(checkpoints[i]);
                window.draw(point);
            }
            //finish
            sf::Color color3(200,90,25);
            point.setFillColor(color3);
            point.setRadius(15);
            point.setOrigin(sf::Vector2f(point.getRadius(),point.getRadius()));
            point.setPosition(finishLine);
            window.draw(point);

        }

        ~Track() {return;}
};