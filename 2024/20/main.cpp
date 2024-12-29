#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

class Tile {
   public:
    int x;
    int y;

    int g;
    int h;
    int f;

    bool path;
    bool wall;

    Tile* parent;

    Tile(int x, int y) {
        this->x = x;
        this->y = y;
        this->f = 0;
        this->g = 0;
        this->h = 0;
        this->parent = nullptr;
        this->wall = false;
        this->path = false;
    }
};

int heur(Tile* st, Tile* end) {
    return abs(st->x - end->x) + abs(st->y - end->y);
}

class Map {
   public:
    std::vector<std::vector<Tile*>*> tiles;

    Tile* start;
    Tile* end;

    long steps;

    bool foundBaseline = false;
    long baseline = 0;

    std::map<long, long> saves;

    std::map<std::pair<Tile*, Tile*>, bool> donePairs;

    std::map<Tile*, bool> wallend;

    void printMap() {
        for (int i = 0; i < tiles.size(); i++) {
            for (int j = 0; j < tiles[i]->size(); j++) {
                if (tiles[i]->at(j) == start) {
                    std::cout << "S";
                    continue;
                }
                if (tiles[i]->at(j) == end) {
                    std::cout << "E";
                    continue;
                }
                if (tiles[i]->at(j)->wall) {
                    std::cout << "#";
                    continue;
                }
                if (tiles[i]->at(j)->path) {
                    std::cout << "X";
                    continue;
                }
                std::cout << ".";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    Tile* getTile(int x, int y) { return tiles[y]->at(x); }

    void parseInput(std::string line) {
        std::vector<Tile*>* row = new std::vector<Tile*>();
        for (int i = 0; i < line.size(); i++) {
            Tile* tile = new Tile(i, tiles.size());
            if (line[i] == '#') {
                tile->wall = true;
            } else if (line[i] == 'S') {
                start = tile;
            } else if (line[i] == 'E') {
                end = tile;
            }
            row->push_back(tile);
        }
        tiles.push_back(row);
    }

    std::vector<Tile*>* getNeighbors(Tile* tile) {
        std::vector<Tile*>* neighbors = new std::vector<Tile*>();
        if (tile->x > 0 && !tiles[tile->y]->at(tile->x - 1)->wall) {
            neighbors->push_back(getTile(tile->x - 1, tile->y));
        }
        if (tile->x < tiles[tile->y]->size() - 1 &&
            !tiles[tile->y]->at(tile->x + 1)->wall) {
            neighbors->push_back(getTile(tile->x + 1, tile->y));
        }
        if (tile->y > 0 && !tiles[tile->y - 1]->at(tile->x)->wall) {
            neighbors->push_back(getTile(tile->x, tile->y - 1));
        }
        if (tile->y < tiles.size() - 1 &&
            !tiles[tile->y + 1]->at(tile->x)->wall) {
            neighbors->push_back(getTile(tile->x, tile->y + 1));
        }
        return neighbors;
    }

    std::vector<Tile*>* getNeighborsWalls(Tile* tile) {
        std::vector<Tile*>* neighbors = new std::vector<Tile*>();
        if (tile->x > 0) {
            neighbors->push_back(getTile(tile->x - 1, tile->y));
        }
        if (tile->x < tiles[tile->y]->size() - 1) {
            neighbors->push_back(getTile(tile->x + 1, tile->y));
        }
        if (tile->y > 0) {
            neighbors->push_back(getTile(tile->x, tile->y - 1));
        }
        if (tile->y < tiles.size() - 1) {
            neighbors->push_back(getTile(tile->x, tile->y + 1));
        }
        return neighbors;
    }

    Tile* getLowestF(std::vector<Tile*>* open) {
        int fMin = open->at(0)->f;
        Tile* lowest = open->at(0);
        for (int i = 1; i < open->size(); i++) {
            if (open->at(i)->f < fMin) {
                fMin = open->at(i)->f;
                lowest = open->at(i);
            }
        }
        return lowest;
    }

    bool inList(std::vector<Tile*>* list, Tile* tile) {
        for (int i = 0; i < list->size(); i++) {
            if (list->at(i) == tile) {
                return true;
            }
        }
        return false;
    }

    void removeFromList(std::vector<Tile*>* list, Tile* tile) {
        for (int i = 0; i < list->size(); i++) {
            if (list->at(i) == tile) {
                list->erase(list->begin() + i);
            }
        }
    }

    bool hasPath() { return end->parent != nullptr; }

    void resetPaths() {
        for (int i = 0; i < tiles.size(); i++) {
            for (int j = 0; j < tiles[i]->size(); j++) {
                tiles[i]->at(j)->path = false;
            }
        }
    }

    void resetParents() {
        for (int i = 0; i < tiles.size(); i++) {
            for (int j = 0; j < tiles[i]->size(); j++) {
                tiles[i]->at(j)->parent = nullptr;
            }
        }
    }

    void findCheats(Tile* tile) {
        if (tile->wall) {
            return;
        }
        if (tile->x == 0 || tile->x == tiles[tile->y]->size() - 1 ||
            tile->y == 0 || tile->y == tiles.size() - 1) {
            return;
        }
        std::vector<Tile*>* neighbors = getNeighborsWalls(tile);
        for (int i = 0; i < neighbors->size(); i++) {
            Tile* neighbor = neighbors->at(i);
            if(wallend.find(neighbor) != wallend.end()){
                continue;
            }
            if(!neighbor->wall){
                continue;
            }
            wallend[neighbor] = true;
            std::vector<Tile*>* neighborNeighbors = getNeighborsWalls(neighbor);
            for (int j = 0; j < neighborNeighbors->size(); j++) {
                Tile* secNeighbour = neighborNeighbors->at(j);
                if (secNeighbour == tile) {
                    continue;
                }
                if(secNeighbour->wall){
                    continue;
                }
                if((donePairs.find(std::make_pair(neighbor, secNeighbour)) != donePairs.end()) || (donePairs.find(std::make_pair(secNeighbour, neighbor)) != donePairs.end())){
                    continue;
                }
                bool tmpNeigh = neighbor->wall;
                bool tmpSec = secNeighbour->wall;
                neighbor->wall = false;
                secNeighbour->wall = false;
                resetParents();
                resetPaths();
                findPath();
                neighbor->wall = tmpNeigh;
                secNeighbour->wall = tmpSec;
                donePairs[std::make_pair(neighbor, secNeighbour)] = true;
            }
            delete neighborNeighbors;
        }
        delete neighbors;
    }

    void trigCheats(){
        for(int i = 1; i < tiles.size()-1; i++){
            for(int j = 0; j < tiles[i]->size()-1; j++){
                findCheats(tiles[i]->at(j));
            }
            std::cout << "done line " << i << std::endl;
        }
        for (const auto& pair : saves) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }
    }

    void findPath() {
        steps = 0;
        std::vector<Tile*> open = {start};
        std::vector<Tile*> closed = {};

        

        while (open.size() > 0) {
            Tile* current = getLowestF(&open);
            std::vector<Tile*>* neighbors = getNeighbors(current);
            if (!inList(&closed, current)) {
                closed.push_back(current);
            }

            removeFromList(&open, current);

            for (int i = 0; i < neighbors->size(); i++) {
                Tile* neighbor = neighbors->at(i);
                if (inList(&closed, neighbor)) {
                    continue;
                }
                int g = current->g + 1;
                int h = heur(neighbor, end);
                int f = g + h;

                if (foundBaseline && f > baseline-98) {
                    continue;
                }

                if (!inList(&open, neighbor)) {
                    neighbor->g = g;
                    neighbor->h = h;
                    neighbor->f = f;
                    neighbor->parent = current;
                    open.push_back(neighbor);
                } else if (f < neighbor->f) {
                    neighbor->g = g;
                    neighbor->h = h;
                    neighbor->f = f;
                    neighbor->parent = current;
                }
            }
            delete neighbors;
        }
        Tile* cur = end;
        if(cur->parent == nullptr){
            return;
        }
        while (cur->parent != nullptr) {
            cur->path = true;
            cur = cur->parent;
            steps++;
        }
        if (!foundBaseline) {
            foundBaseline = true;
            baseline = steps;
        } else {
            long safe = baseline - steps;
            if(safe == 0){
                return;
            }
            std::cout << "found save of " << safe << std::endl;
            if(safe == 2)
                printMap();
            if (saves.find(safe) == saves.end()) {
                // Add a new record with a value of one
                saves[safe] = 1;
            } else {
                // Increment the existing record
                saves[safe]++;
            }
        }
    }
};

int main() {
    std::ifstream file("input.txt");
    // std::cout << "hello" << std::endl;
    std::string line;
    unsigned long long total = 0;
    unsigned long long xTotal = 0;
    std::vector<std::string> lines;

    Map map;

    while (std::getline(file, line)) {
        map.parseInput(line);
    }

    map.printMap();
    map.findPath();
    
    map.trigCheats();
    
    //map.printMap();
    total = map.baseline;
    std::cout << total << std::endl;
    std::cout << xTotal << std::endl;

    return 0;
}