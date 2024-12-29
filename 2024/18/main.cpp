#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <utility>

class Tile{
public:
    int x;
    int y;

    int g;
    int h;
    int f;

    bool corrupted;
    bool path;

    Tile* parent;

    Tile(int x, int y){
        this->x = x;
        this->y = y;
        this->f = 0;
        this->g = 0;
        this->h = 0;
        this->corrupted = false;
        this->parent = nullptr;
        this->path = false;
    }
};

int heur(Tile* st, Tile* end){
    return abs(st->x - end->x) + abs(st->y - end->y);
}

class Map{
public:

    int dimensions;
    std::vector<std::vector<Tile*>*> tiles;

    Tile* start;
    Tile* end;

    long steps;

    void createMap(){
        for(int i = 0; i < dimensions; i++){
            std::vector<Tile*>* row = new std::vector<Tile*>();
            for(int j = 0; j < dimensions; j++){
                row->push_back(new Tile(j, i));
            }
            tiles.push_back(row);
        }

        start = tiles[0]->at(0);
        end = tiles[dimensions - 1]->at(dimensions - 1);
        steps = 0;
    }

    void corrupt(std::string line){
        int pos = line.find(",");
        int x = std::stoi(line.substr(0, pos));
        int y = std::stoi(line.substr(pos + 1));
        tiles[y]->at(x)->corrupted = true;
    }

    void printMap(){
        for(int i = 0; i < dimensions; i++){
            for(int j = 0; j < dimensions; j++){
                if(tiles[i]->at(j)->corrupted){
                    std::cout << "#";
                    continue;
                }
                if(tiles[i]->at(j)->path){
                    std::cout << "X";
                    continue;
                }
                std::cout << ".";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    Tile* getTile(int x, int y){
        return tiles[y]->at(x);
    }

    std::vector<Tile*>* getNeighbors(Tile* tile){
        std::vector<Tile*>* neighbors = new std::vector<Tile*>();
        if(tile->x > 0 && !tiles[tile->y]->at(tile->x - 1)->corrupted){
            neighbors->push_back(getTile(tile->x - 1, tile->y));
        }
        if(tile->x < dimensions - 1 && !tiles[tile->y]->at(tile->x + 1)->corrupted){
            neighbors->push_back(getTile(tile->x + 1, tile->y));
        }
        if(tile->y > 0 && !tiles[tile->y - 1]->at(tile->x)->corrupted){
            neighbors->push_back(getTile(tile->x, tile->y - 1));
        }
        if(tile->y < dimensions - 1 && !tiles[tile->y + 1]->at(tile->x)->corrupted){
            neighbors->push_back(getTile(tile->x, tile->y + 1));
        }
        return neighbors;
    }

    Tile* getLowestF(std::vector<Tile*>* open){
        int fMin = open->at(0)->f;
        Tile* lowest = open->at(0);
        for(int i = 1; i < open->size(); i++){
            if(open->at(i)->f < fMin){
                fMin = open->at(i)->f;
                lowest = open->at(i);
            }
        }
        return lowest;
    }

    bool inList(std::vector<Tile*>* list, Tile* tile){
        for(int i = 0; i < list->size(); i++){
            if(list->at(i) == tile){
                return true;
            }
        }
        return false;
    }

    void removeFromList(std::vector<Tile*>* list, Tile* tile){
        for(int i = 0; i < list->size(); i++){
            if(list->at(i) == tile){
                list->erase(list->begin() + i);
            }
        }
    }

    void clearPath(){
        for(int i = 0; i < dimensions; i++){
            for(int j = 0; j < dimensions; j++){
                tiles[i]->at(j)->path = false;
                tiles[i]->at(j)->parent = nullptr;
                tiles[i]->at(j)->g = 0;
                tiles[i]->at(j)->h = 0;
                tiles[i]->at(j)->f = 0;
            }
        }
        steps = 0;
    }

    bool hasPath(){
        return end->parent != nullptr;
    }

    void findPath(){
        std::vector<Tile*> open = {start};
        std::vector<Tile*> closed = {};

        while(open.size() > 0){

            Tile* current = getLowestF(&open);
            std::vector<Tile*>* neighbors = getNeighbors(current);
            if(!inList(&closed, current)){
                closed.push_back(current);
            }
            
            removeFromList(&open, current);


            for(int i = 0; i < neighbors->size(); i++){
                Tile* neighbor = neighbors->at(i);
                if(inList(&closed, neighbor)){
                    continue;
                }
                int g = current->g + 1;
                int h = heur(neighbor, end);
                int f = g + h;

                if(!inList(&open, neighbor)){
                    neighbor->g = g;
                    neighbor->h = h;
                    neighbor->f = f;
                    neighbor->parent = current;
                    open.push_back(neighbor);
                } else if(f < neighbor->f){
                    neighbor->g = g;
                    neighbor->h = h;
                    neighbor->f = f;
                    neighbor->parent = current;
                }
            }
            delete neighbors;
        }
        Tile* cur = end;
        while(cur->parent != nullptr){
            cur->path = true;
            cur = cur->parent;
            steps++;
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
    map.dimensions = 71;

    map.createMap();
    //map.printMap();

    int count = 0;

    while (std::getline(file, line))
    {
        map.corrupt(line);
        if(count == 1023){
            break;
        }
        
        count++;
    }

    map.findPath();
    total = map.steps;
    std::cout << total << std::endl;

    while(map.hasPath()){
        map.clearPath();
        if(!std::getline(file, line)){
            break;
        }
        std::cout << "found " << line << std::endl;
        map.corrupt(line);
        map.findPath();
    }
    

    map.printMap();

    std::cout << line << std::endl;

    std::cout << total << std::endl;
    std::cout << xTotal << std::endl;

    return 0;
}