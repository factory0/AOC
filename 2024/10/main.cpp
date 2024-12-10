#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>

class Tile{
public:
    int x;
    int y;
    int height;
    std::vector<Tile*> endTiles;
    long rating;

    Tile(int x, int y, int height){
        this->x = x;
        this->y = y;
        this->height = height;
        rating = 0;
    }

    void printTile(){
        std::cout << "x: " << x << " y: " << y << " h: " << height << std::endl;
    }

    void addEndTile(Tile* tile){
        for(int i = 0; i < endTiles.size(); i++){
            if(endTiles[i]->x == tile->x && endTiles[i]->y == tile->y){
                return;
            }
        }
        endTiles.push_back(tile);
    }
};

class Path{
public:
    std::vector<Tile*> path;
    Tile* start;
    Tile* end;
};

Path* cpy(Path* paths){
    Path* newPaths = new Path();
    newPaths->start = paths->start;
    newPaths->end = paths->end;
    for(int i = 0; i < paths->path.size(); i++){
        newPaths->path.push_back(paths->path[i]);
    }
    return newPaths;
}

class Map{
public:
    std::vector<std::vector<Tile*>*>* map;
    std::vector<Path*> paths;

    Map(){
        map = new std::vector<std::vector<Tile*>*>();
    }

    void parseRow(std::string row){
        std::vector<Tile*>* rowTiles = new std::vector<Tile*>();
        for (int i = 0; i < row.length(); i++){
            int height = row[i] - '0';
            Tile* tile = new Tile(i, map->size(), height);
            rowTiles->push_back(tile);
        }
        map->push_back(rowTiles);
    }

    void printMap(){
        for (int i = 0; i < map->size(); i++){
            for (int j = 0; j < map->at(i)->size(); j++){
                std::cout << map->at(i)->at(j)->height;
            }
            std::cout << std::endl;
        }
    }

    Tile* getNeighbourWest(Tile* tile){
        if (tile->x <= 0){
            return NULL;
        }
        return map->at(tile->y)->at(tile->x - 1);
    }

    Tile* getNeighbourEast(Tile* tile){
        if (tile->x >= map->at(tile->y)->size() - 1){
            return NULL;
        }
        return map->at(tile->y)->at(tile->x + 1);
    }

    Tile* getNeighbourNorth(Tile* tile){
        if (tile->y <= 0){
            return NULL;
        }
        return map->at(tile->y - 1)->at(tile->x);
    }

    Tile* getNeighbourSouth(Tile* tile){
        if (tile->y >= map->size() - 1){
            return NULL;
        }
        return map->at(tile->y + 1)->at(tile->x);
    }

    bool isOneHigher(Tile* tile, Tile* neighbour){
        if (neighbour == NULL){
            return false;
        }
        return (neighbour->height - tile->height) == 1;
    }

    void recFind(Tile* tile, Path* path){
        bool deadend = true;
        path->path.push_back(tile);
        if(tile->height == 9){
            path->end = tile;
            paths.push_back(path);
            path->start->addEndTile(path->end);
            path->start->rating += 1;
        }
        if(isOneHigher(tile, getNeighbourWest(tile))){
            deadend = false;
            Path* newPath = cpy(path);
            recFind(getNeighbourWest(tile), newPath);
        }
        if(isOneHigher(tile, getNeighbourEast(tile))){
            deadend = false;
            Path* newPath = cpy(path);
            recFind(getNeighbourEast(tile), newPath);
        }
        if(isOneHigher(tile, getNeighbourNorth(tile))){
            deadend = false;
            Path* newPath = cpy(path);
            recFind(getNeighbourNorth(tile), newPath);
        }
        if(isOneHigher(tile, getNeighbourSouth(tile))){
            deadend = false;
            //Path* newPath = cpy(path);
            recFind(getNeighbourSouth(tile), path);
        }
        if(deadend){
            delete path;
        }
        return;
    }

    void findPaths(){
        for (int i = 0; i < map->size(); i++){
            for (int j = 0; j < map->at(i)->size(); j++){
                if(map->at(i)->at(j)->height == 0){
                    Path* path = new Path();
                    path->start = map->at(i)->at(j);
                    recFind(map->at(i)->at(j), path);
                }
            }
        }
    }

    long long getTotalTrailheads(){
        long long ret = 0;
        for(int i = 0; i < map->size(); i++){
            for(int j = 0; j < map->at(i)->size(); j++){
                ret += map->at(i)->at(j)->endTiles.size();
            }
        }
        return ret;
    }

    long long getTotalTrailheadRatings(){
        long long ret = 0;
        for(int i = 0; i < map->size(); i++){
            for(int j = 0; j < map->at(i)->size(); j++){
                ret += map->at(i)->at(j)->rating;
            }
        }
        return ret;
    }
};


int main()
{

    std::ifstream file("input.txt");
    // std::cout << "hello" << std::endl;
    std::string line;
    long long total = 0;
    long long xTotal = 0;
    std::vector<std::string> lines;

    Map map;

    while (std::getline(file, line))
    {
        map.parseRow(line);
    }

    map.printMap();
    map.findPaths();

    /*for(int i = 0; i < map.paths.size(); i++){
        std::cout << "start: "; map.paths.at(i)->start->printTile();
        std::cout << "end: "; map.paths.at(i)->end->printTile();

    }*/

    total = map.getTotalTrailheads();
    xTotal = map.getTotalTrailheadRatings();
    // std::cout << guard->x << " " << guard->y << std::endl;
    std::cout << total << std::endl;
    std::cout << xTotal << std::endl;

    return 0;
}