#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

enum Direction { UP, DOWN, LEFT, RIGHT };

class Robot {
   public:
    int x;
    int y;

    Robot(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class Box {
   public:
    int x;
    int y;

    Box(int x, int y) {
        this->x = x;
        this->y = y;
    }

    long gps() { return x + 100 * y; }
};

class Tile {
   public:
    int x;
    int y;
    bool wall;

    Tile(int x, int y, bool wall) {
        this->x = x;
        this->y = y;
        this->wall = wall;
    }
};

class Map {
   public:
    std::vector<std::vector<Tile*>*>* tiles;

    Robot* robot;

    std::vector<Box*> boxes;

    Map() { tiles = new std::vector<std::vector<Tile*>*>(); }

    void parseMap(std::string line) {
        std::vector<Tile*>* row = new std::vector<Tile*>();
        for (int i = 0; i < line.size(); i++) {
            Tile* tile = new Tile(i * 2, tiles->size(), false);
            Tile* sec = new Tile(i * 2 + 1, tiles->size(), false);
            if (line[i] == '#') {
                tile->wall = true;
                sec->wall = true;
            }
            if (line[i] == '@') {
                robot = new Robot(i * 2, tiles->size());
            }
            if (line[i] == 'O') {
                Box* box = new Box(i * 2, tiles->size());
                // Box* dou = new Box(i*2+1, tiles->size());
                boxes.push_back(box);
            }
            row->push_back(tile);
            row->push_back(sec);
        }
        tiles->push_back(row);
    }

    void printMap() {
        for (int i = 0; i < tiles->size(); i++) {
            for (int j = 0; j < tiles->at(i)->size(); j++) {
                if (tiles->at(i)->at(j)->wall) {
                    std::cout << "#";
                    continue;
                } else {
                    if (tiles->at(i)->at(j)->x == robot->x &&
                        tiles->at(i)->at(j)->y == robot->y) {
                        std::cout << "@";
                        continue;
                    }
                    bool box = false;
                    for (int k = 0; k < boxes.size(); k++) {
                        if (boxes[k]->x == tiles->at(i)->at(j)->x &&
                            boxes[k]->y == tiles->at(i)->at(j)->y) {
                            std::cout << "[]";
                            box = true;
                            break;
                        }
                    }
                    if (box) {
                        j++;
                    }
                    if (!box) {
                        std::cout << ".";
                    }
                }
            }
            std::cout << std::endl;
        }
    }

    Box* getBox(int x, int y) {
        for (int i = 0; i < boxes.size(); i++) {
            if (boxes[i]->x == x && boxes[i]->y == y ||
                boxes[i]->x == x - 1 && boxes[i]->y == y) {
                return boxes[i];
            }
        }
        return nullptr;
    }

    void adjCoords(int* x, int* y, Direction direction) {
        switch (direction) {
            case UP:
                (*y)--;
                break;
            case DOWN:
                (*y)++;
                break;
            case LEFT:
                (*x)--;
                break;
            case RIGHT:
                (*x)++;
                break;
        }
    }

    bool findEmptySpace(Direction direction, int* x, int* y) {
        if (direction == UP || direction == DOWN) {
            int tmpX = robot->x;
            int tmpY = robot->y;
            adjCoords(&tmpX, &tmpY, direction);
            if (wallInWay(tmpX, tmpY)) {
                return false;
            }
            Box* b = getBox(tmpX, tmpY);
            if (b == nullptr) {
                return true;
            } else {
                if (direction == UP) {
                    return recSpaceUp(b);
                } else {
                    return recSpaceDown(b);
                }
            }
        }
        int tmpX = robot->x;
        int tmpY = robot->y;
        bool ret = false;
        while (!ret) {
            adjCoords(&tmpX, &tmpY, direction);
            if (wallInWay(tmpX, tmpY)) {
                break;
            }
            if (boxInWay(tmpX, tmpY)) {
                continue;
            }
            ret = true;
            *x = tmpX;
            *y = tmpY;
        }
        return ret;
    }

    bool wallInWay(int x, int y) {
        if (x < 0 || x >= tiles->at(0)->size() || y < 0 || y >= tiles->size()) {
            return true;
        }
        if (tiles->at(y)->at(x)->wall) {
            return true;
        }
        return false;
    }

    bool boxInWay(int x, int y) {
        if (x < 0 || x >= tiles->at(0)->size() || y < 0 || y >= tiles->size()) {
            return true;
        }
        for (int i = 0; i < boxes.size(); i++) {
            if (boxes[i]->x == x && boxes[i]->y == y ||
                boxes[i]->x == x - 1 && boxes[i]->y == y) {
                return true;
            }
        }
        return false;
    }

    void boxesAbove(Box* b, Box** left, Box** right) {

        Box* l = getBox(b->x, b->y - 1);
        if (l != nullptr) {
            *left = l;
        }
        Box* r = getBox(b->x + 1, b->y - 1);
        if (r != nullptr && r != l) {
            *right = r;
        }
    }

    bool spaceAbove(Box* b) {
        bool ret = true;
        if (wallInWay(b->x, b->y - 1)) {
            ret = false;
        }
        if (wallInWay(b->x + 1, b->y - 1)) {
            ret = false;
        }
        return ret;
    }

    bool spaceBelow(Box* b) {
        bool ret = true;
        if (wallInWay(b->x, b->y + 1)) {
            ret = false;
        }
        if (wallInWay(b->x + 1, b->y + 1)) {
            ret = false;
        }
        return ret;
    }

    void boxesBelow(Box* b, Box** left, Box** right) {

        Box* l = getBox(b->x, b->y + 1);
        if (l != nullptr) {
            *left = l;
        }
        Box* r = getBox(b->x + 1, b->y + 1);
        if (r != nullptr && r != l) {
            *right = r;
        }
    }

    bool recSpaceUp(Box* b) {
        Box* tp = new Box(b->x, b->y - 1);
        Box* l = nullptr;
        Box* r = nullptr;
        boxesAbove(b, &l, &r);
        delete tp;
        if (!spaceAbove(b)) {
            return false;
        }
        bool spaceLeft = true;
        bool spaceRight = true;
        if (l != nullptr) {
            spaceLeft = recSpaceUp(l);
        }
        if (r != nullptr) {
            spaceRight = recSpaceUp(r);
        }

        return spaceLeft && spaceRight;
    }

    bool recSpaceDown(Box* b) {
        Box* l = nullptr;
        Box* r = nullptr;
        boxesBelow(b, &l, &r);
        if (!spaceBelow(b)) {
            return false;
        }
        bool spaceLeft = true;
        bool spaceRight = true;
        if (l != nullptr) {
            spaceLeft = recSpaceDown(l);
        }
        if (r != nullptr) {
            spaceRight = recSpaceDown(r);
        }

        return spaceLeft && spaceRight;
    }

    void moveRecUp(Box* b) {
        Box* l = nullptr;
        Box* r = nullptr;
        boxesAbove(b, &l, &r);
        //std::cout << b->x << " " << b->y << std::endl;
        adjCoords(&b->x, &b->y, UP);
        if (l != nullptr) {
            //std::cout << "rec " << l->x << " " << l->y << std::endl;
            moveRecUp(l);
        }
        if (r != nullptr) {
            //std::cout << "rec r " << r->x << " " << r->y << std::endl;
            moveRecUp(r);
        }
        
    }

    void moveRecDown(Box* b) {
        Box* l = nullptr;
        Box* r = nullptr;
        boxesBelow(b, &l, &r);
        adjCoords(&b->x, &b->y, DOWN);
        if (l != nullptr) {
            moveRecDown(l);
        }
        if (r != nullptr) {
            moveRecDown(r);
        }
        
    }

    void move(Direction direction) {
        int tmpX = robot->x;
        int tmpY = robot->y;
        int startX = robot->x;
        int startY = robot->y;
        bool empty = findEmptySpace(direction, &tmpX, &tmpY);
        //std::cout << empty << " " << tmpX << " " << tmpY << std::endl;
        if (empty) {
            adjCoords(&robot->x, &robot->y, direction);
            if (direction == UP) {
                Box* b = getBox(robot->x, robot->y);
                if (b != nullptr) {
                    moveRecUp(b);
                }
            } else if (direction == DOWN) {
                Box* b = getBox(robot->x, robot->y);
                if (b != nullptr) {
                    moveRecDown(b);
                }
            } else {
                Box* b = nullptr;
                std::vector<Box*> adjBoxes;
                while (tmpX != startX || tmpY != startY) {
                    // std::cout << startX << " " << startY << std::endl;
                    b = getBox(startX, startY);
                    bool adj = true;
                    if (b != nullptr) {
                        for (int i = 0; i < adjBoxes.size(); i++) {
                            if (b == adjBoxes[i]) {
                                adj = false;
                            }
                        }
                        if (adj) {
                            adjCoords(&b->x, &b->y, direction);
                            adjBoxes.push_back(b);
                        }
                    }

                    adjCoords(&startX, &startY, direction);
                }
            }
        }
        //printMap();
    }

    void moveChar(char c) {
        switch (c) {
            case '^':
                move(UP);
                break;
            case 'v':
                move(DOWN);
                break;
            case '<':
                move(LEFT);
                break;
            case '>':
                move(RIGHT);
                break;
        }
    }

    void moveLine(std::string line) {
        for (int i = 0; i < line.size(); i++) {
            moveChar(line[i]);
        }
    }

    long totalGPS() {
        long ret = 0;
        for (int i = 0; i < boxes.size(); i++) {
            ret += boxes[i]->gps();
        }
        return ret;
    }
};

int main() {
    std::ifstream file("input.txt");
    // std::cout << "hello" << std::endl;
    std::string line;
    unsigned long long total = 0;
    long long xTotal = 0;
    std::vector<std::string> lines;

    Map map;

    while (std::getline(file, line)) {
        if (line.size() == 0) {
            break;
        }
        map.parseMap(line);
    }

    map.printMap();

    while (std::getline(file, line)) {
        map.moveLine(line);
        //map.printMap();
    }

    total = map.totalGPS();

    std::cout << total << std::endl;
    std::cout << xTotal << std::endl;

    return 0;
}