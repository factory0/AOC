#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <utility>

class Pos{
public:
    int x;
    int y;

    Pos(int x, int y){
        this->x = x;
        this->y = y;
    }

    Pos(){
        x = 0;
        y = 0;
    }

    void add(Pos p){
        this->x += p.x;
        this->y += p.y;
    }
};

class Tank{
public:
    Pos pos;
    Pos vel;

    Tank(int x, int y, int vx, int vy){
        this->pos = Pos(x, y);
        this->vel = Pos(vx, vy);
    }

    void move(){
        pos.add(vel);
    }

    void checkBoundaries(int width, int height){
        if(pos.x < 0){
            pos.x = width+pos.x;
        }
        if(pos.x >= width){
            pos.x = pos.x - width;
        }
        if(pos.y < 0){
            pos.y = height+pos.y;
        }
        if(pos.y >= height){
            pos.y = pos.y - height;
        }
    }

};

class Map{
public:
    std::vector<Tank*> tanks;

    int width;
    int height;

    void parseTank(std::string line){
        int pos = line.find(",");
        int v = line.find("v=");
        int pos2 = line.find (",", v);

        int x = std::stoi(line.substr(2, pos-2));
        int y = std::stoi(line.substr(pos+1, v-2));

        int vx = std::stoi(line.substr(v+2, pos2-v-2));
        int vy = std::stoi(line.substr(pos2+1, line.length()-pos2-1));

        tanks.push_back(new Tank(x, y, vx, vy));

    }

    void printTanks(){
        for (int i = 0; i < tanks.size(); i++){
            std::cout << tanks[i]->pos.x << ", " << tanks[i]->pos.y << ", " << tanks[i]->vel.x << ", " << tanks[i]->vel.y << std::endl;
        }
    }

    void moveTanks(){
        for(int i = 0; i < tanks.size(); i++){
            tanks[i]->move();
            tanks[i]->checkBoundaries(width, height);
        }
    }

    void printMap(){
        int m[height][width];
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                m[i][j] = 0;
            }
        }
        for(int i = 0; i < tanks.size(); i++){
            m[tanks[i]->pos.y][tanks[i]->pos.x] += 1;
        }

        int divX = width/2;
        int divY = height/2;

        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                /*if(i == divY || j == divX){
                    std::cout << " ";
                    continue;
                }*/
                std::cout << m[i][j];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    long long calculateScore(){
        long long score = 0;
        long topLeft = 0;
        long topRight = 0;
        long bottomLeft = 0;
        long bottomRight = 0;

        int divX = width/2;
        int divY = height/2;

        for(int i = 0; i < tanks.size(); i++){
            if(tanks[i]->pos.x < divX && tanks[i]->pos.y < divY){
                topLeft += 1;
            }
            if(tanks[i]->pos.x > divX && tanks[i]->pos.y < divY){
                topRight += 1;
            }
            if(tanks[i]->pos.x < divX && tanks[i]->pos.y > divY){
                bottomLeft += 1;
            }
            if(tanks[i]->pos.x > divX && tanks[i]->pos.y > divY){
                bottomRight += 1;
            }
        }
        std::cout << topLeft << ", " << topRight << ", " << bottomLeft << ", " << bottomRight << std::endl;
        score = topLeft * topRight * bottomLeft * bottomRight;
        return score;
    }

    void christmas(int layer){
        int m[height][width];
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                m[i][j] = 0;
            }
        }
        for(int i = 0; i < tanks.size(); i++){
            m[tanks[i]->pos.y][tanks[i]->pos.x] += 1;
        }

        for(int i = 0; i < height-3; i++){
            for(int j = 2; j < width-2; j++){
                if(m[i][j] > 0){
                    if(m[i+1][j-1] > 0 && m[i+1][j] > 0 && m[i+1][j+1] > 0 && 
                        m[i+2][j-2] > 0 && m[i+2][j-1] > 0 && m[i+2][j] > 0 && m[i+2][j+1] > 0 && m[i+2][j+2] > 0){
                        std::cout << layer << std::endl;
                        printMap();
                        return;
                    }
                }
            }
        }
    }
};


int main()
{

    std::ifstream file("input.txt");
    // std::cout << "hello" << std::endl;
    std::string line;
    unsigned long long total = 0;
    long long xTotal = 0;
    std::vector<std::string> lines;

    Map map;
    map.width = 101;
    map.height = 103;

    while (std::getline(file, line))
    {
       map.parseTank(line);
    }

    //map.printMap();

    for(int i = 0; i < 10000; i++){
        map.moveTanks();
        map.christmas(i);
        //std::cout << std::endl;
        //map.printMap();
    }
    std::cout << std::endl;
    //map.printMap();
    total = map.calculateScore();

    std::cout << total << std::endl;
    std::cout << xTotal << std::endl;

    return 0;
}