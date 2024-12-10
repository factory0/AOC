#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Tile {
   public:
    bool visited;
    bool obstacle;
    int x;
    int y;

    Tile(int x, int y, bool obstacle) {
        this->x = x;
        this->y = y;
        this->obstacle = obstacle;
        visited = false;
    }
};

enum Direction { UP, DOWN, LEFT, RIGHT };

class Map {
   public:
    std::vector<std::vector<Tile *> *> *tiles;

    Map() { tiles = new std::vector<std::vector<Tile *> *>(); }

    bool isObstacle(int x, int y) { return tiles->at(y)->at(x)->obstacle; }

    void parse(std::string line, int *x, int *y) {
        if (line.length() > 0) {
            std::vector<Tile *> *row = new std::vector<Tile *>();
            for (int i = 0; i < line.length(); i++) {
                Tile *tile = new Tile(i, tiles->size(), line[i] == '#');
                row->push_back(tile);
                if (line[i] == '^') {
                    *x = i;
                    *y = tiles->size();
                }
            }
            tiles->push_back(row);
        }
    }

    long countVisited() {
        long total = 0;
        for (int i = 0; i < tiles->size(); i++) {
            for (int j = 0; j < tiles->at(i)->size(); j++) {
                if (tiles->at(i)->at(j)->visited) {
                    total++;
                    std::cout << "X";
                } else {
                    if (tiles->at(i)->at(j)->obstacle) {
                        std::cout << "#";
                    } else {
                        std::cout << ".";
                    }
                }
            }
            std::cout << std::endl;
        }
        return total;
    }
};

class Guard {
   public:
    int x;
    int y;
    Direction direction;
    bool onmap;

    Guard(int x, int y) {
        this->x = x;
        this->y = y;
        direction = UP;
        onmap = true;
    }

    void checkifoutofmap(Map *map) {
        if (y < 0 || y >= map->tiles->size() || x < 0 ||
            x >= map->tiles->at(y)->size()) {
            onmap = false;
        }
    }

    void turn(Map *map) {
        if (direction == UP) {
            direction = RIGHT;
        } else if (direction == RIGHT) {
            direction = DOWN;
        } else if (direction == DOWN) {
            direction = LEFT;
        } else if (direction == LEFT) {
            direction = UP;
        }
    }

    bool checkifobstacle(Map *map) {
        bool ret = false;
        switch (direction) {
            case UP:
                if (y > 0 && map->isObstacle(x, y - 1)) {
                    ret = true;
                }
                break;
            case DOWN:
                if (y < map->tiles->size() - 1 && map->isObstacle(x, y + 1)) {
                    ret = true;
                }
                break;
            case LEFT:
                if (x > 0 && map->isObstacle(x - 1, y)) {
                    ret = true;
                }
                break;
            case RIGHT:
                if (x < map->tiles->at(y)->size() - 1 &&
                    map->isObstacle(x + 1, y)) {
                    ret = true;
                }
                break;
            default:
                break;
        }
        return ret;
    }

    void move(Map *map) {
        if(map->tiles->at(y)->at(x)->obstacle) {
            std::cout << "hit an obstacle at " << x << " " << y << std::endl;
        }
        switch (direction) {
            case UP:
                map->tiles->at(y)->at(x)->visited = true;
                y--;
                break;
            case DOWN:
                map->tiles->at(y)->at(x)->visited = true;
                y++;
                break;
            case LEFT:
                map->tiles->at(y)->at(x)->visited = true;
                x--;
                break;
            case RIGHT:
                map->tiles->at(y)->at(x)->visited = true;
                x++;
                break;
        }
        
    }
};

Map* cop(Map* map){
    Map* ret = new Map();
    ret->tiles = new std::vector<std::vector<Tile *> *>();
    for(int i = 0; i < map->tiles->size(); i++) {
        std::vector<Tile *> *row = new std::vector<Tile *>();
        for(int j = 0; j < map->tiles->at(i)->size(); j++) {
            Tile *tile = new Tile(j, i, map->isObstacle(j, i));
            row->push_back(tile);
        }
        ret->tiles->push_back(row);
    }
    return ret;
}

int main() {
    std::ifstream file("input.txt");
    // std::cout << "hello" << std::endl;
    std::string line;
    long long total = 0;
    long xtotal = 0;
    std::vector<std::string> lines;

    Map map;
    Guard *guard;

    while (std::getline(file, line)) {
        int x = -1;
        int y = -1;
        map.parse(line, &x, &y);
        if (x != -1 && y != -1) {
            guard = new Guard(x, y);
        }
    }

    int x = guard->x;
    int y = guard->y;
    Map* tplate = cop(&map);

    while (guard->onmap) {
        while (guard->checkifobstacle(&map)) {
            guard->turn(&map);
        }
        guard->move(&map);
        guard->checkifoutofmap(&map);
        /*map.countVisited();
        std::cout<<std::endl;*/
    }

    total = map.countVisited();

    for(int i = 0; i < tplate->tiles->size(); i++) {
        for(int j = 0; j < tplate->tiles->at(i)->size(); j++) {
            if(map.tiles->at(i)->at(j)->visited) {
                if(i == y && j == x){
                    continue;
                }
                Guard* g = new Guard(x, y);
                Map* test = cop(tplate);
                test->tiles->at(i)->at(j)->obstacle = true;
                long long loops = 0;
                while(g->onmap) {
                    while(g->checkifobstacle(test)) {
                        g->turn(test);
                    }
                    g->move(test);
                    g->checkifoutofmap(test);
                    loops++;
                    if(loops > 2500000) {
                        break;
                    }
                }
                if(g->onmap) {
                    xtotal++;
                    std::cout << "found obstacle at " << j << " " << i << std::endl;
                    std::cout << xtotal << std::endl;
                }else{
                    //std::cout << "no obstacle at " << j << " " << i << std::endl;
                }
            }
            
        }
    }

    // std::cout << guard->x << " " << guard->y << std::endl;
    std::cout << total << std::endl;
    std::cout << xtotal << std::endl;

    return 0;
}