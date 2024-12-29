#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <utility>

enum Direction{
    NORTH = 0,
    SOUTH,
    EAST,
    WEST
};

class Tile{
public:
    int x;
    int y;
    long g;
    long h;
    long f;
    bool isWall;
    bool path;
    std::vector<Tile*> parent;

    Tile(int x, int y, bool wall){
        this->x = x;
        this->y = y;
        this->g = 0;
        this->h = 0;
        this->f = 0;
        this->isWall = wall;
        this->path = false;
    }
};



Direction detDirection(Tile* a, Tile* b){
    if(a->x > b->x){
        return WEST;
    }
    if(a->x < b->x){
        return EAST;
    }
    if(a->y > b->y){
        return NORTH;
    }
    if(a->y < b->y){
        return SOUTH;
    }
    return NORTH;
}

class Path{
public:
    std::vector<Tile*> path;
    Direction dir;
    long score;

    Path(){
        dir = EAST;
        score = 0;
    }

    bool hasTile(Tile* tile){
        for(int i = 0; i < path.size(); i++){
            if(path.at(i) == tile){
                return true;
            }
        }
        return false;
    }
};

Path* deepCopy(Path* path){
    Path* newpath = new Path();
    newpath->dir = path->dir;
    newpath->score = path->score;
    for(int i = 0; i < path->path.size(); i++){
        newpath->path.push_back(path->path.at(i));
    }
    return newpath;
}

int Rots(Direction paren, Direction child){
    if(paren == child){
        return 0;
    }
    if((paren == NORTH && child == EAST) || (paren == EAST && child == SOUTH) || (paren == SOUTH && child == WEST) 
    || (paren == WEST && child == NORTH) || (paren == NORTH && child == WEST) || (paren == WEST && child == SOUTH) 
    || (paren == SOUTH && child == EAST) || (paren == EAST && child == NORTH)){
        return 1000;
    }else{
        return 2000;
    }
}

long getRotPen(Tile* a, Tile* b){
    /* Direction dir = detDirection(a, b);
    if(a->parent == nullptr){
        return Rots(EAST, dir);
    }
    return Rots(detDirection(a->parent, a), dir); */

    if(a->parent.size() == 0){
        if(a->x == b->x){
            return 1000;
        }else{
            if(b->x > a->x){
                return 0;
            }else{
                return 2000;
            }
        }
    }

    Direction dir = detDirection(a->parent[a->parent.size() - 1], a);

    switch(dir){
        case NORTH:
            if(a->x == b->x){
                if(b->y < a->y){
                    return 0;
                }else{
                    return 2000;
                }
            }else{
                return 1000;
            }
            break;
        case SOUTH:
            if(a->x == b->x){
                if(b->y > a->y){
                    return 0;
                }else{
                    return 2000;
                }
            }else{
                return 1000;
            }
            break;
        case EAST:
            if(a->y == b->y){
                if(b->x > a->x){
                    return 0;
                }else{
                    return 2000;
                }
            }else{
                return 1000;
            }
            break;
        case WEST:
            if(a->y == b->y){
                if(b->x < a->x){
                    return 0;
                }else{
                    return 2000;
                }
            }else{
                return 1000;
            }
            break;
    }

    return 0;
}

long heur(Tile* par, Tile* a, Tile* end){
    if(a->x == end->x && a->y == end->y){
        return 0;
    }
    long dist = abs(a->x - end->x) + abs(a->y - end->y);
    //return dist;
    //Tile* tmp = a->parent;
    bool switched = false;
    if(par != nullptr){
        a->parent.push_back(par);
        switched = true;
    }
    long rot = getRotPen(a, end);
    if(switched){
        a->parent.erase(a->parent.end() - 1);
    }
    return dist + rot;
}

Tile* findTile(Tile* tile, std::vector<Tile*>* list){
    for(int i = 0; i < list->size(); i++){
        if(list->at(i) == tile){
            return list->at(i);
        }
    }
    return nullptr;
}

void printDir(Tile* tile){
    std::cout << "o";
    return;
    /* Direction dir = detDirection(tile->parent, tile);
    switch(dir){
        case NORTH:
            std::cout << "^";
            break;
        case SOUTH:
            std::cout << "v";
            break;
        case EAST:
            std::cout << ">";
            break;
        case WEST:
            std::cout << "<";
            break;
    } */
}

class Map{
public:
    std::vector<std::vector<Tile*>*> map;

    std::vector<Tile*> openList;
    std::vector<Tile*> closedList;

    Tile* startTile;
    Tile* endTile;

    std::vector<Path*> paths;
    long bestScore;

    Map(){
        startTile = nullptr;
        endTile = nullptr;
        bestScore = 0;
    }



    Tile* getTile(int x, int y){
        return map[y]->at(x);
    }

    std::vector<Tile*>* getNeighbours(Tile* tile){
        std::vector<Tile*>* neighbours = new std::vector<Tile*>();
        if(getTile(tile->x + 1, tile->y)->isWall == false){
            neighbours->push_back(getTile(tile->x + 1, tile->y));
        }
        if(getTile(tile->x - 1, tile->y)->isWall == false){
            neighbours->push_back(getTile(tile->x - 1, tile->y));
        }
        if(getTile(tile->x, tile->y + 1)->isWall == false){
            neighbours->push_back(getTile(tile->x, tile->y + 1));
        }
        if(getTile(tile->x, tile->y - 1)->isWall == false){
            neighbours->push_back(getTile(tile->x, tile->y - 1));
        }
        return neighbours;
    }

    void parseLine(std::string line){
        std::vector<Tile*>* row = new std::vector<Tile*>();
        for(int i = 0; i < line.length(); i++){
            bool wall = false;
            if(line[i] == '#'){
                wall = true;
            }
            Tile* tile = new Tile(i, map.size(), wall);
            if(line[i] == 'S'){
                startTile = tile;
            }
            if(line[i] == 'E'){
                endTile = tile;
            }
            row->push_back(tile);
        }
        map.push_back(row);
    }

    void printMap(){
        for(int i = 0; i < map.size(); i++){
            for(int j = 0; j < map[i]->size(); j++){
                if(map[i]->at(j)->path){
                    //std::cout << "*";
                    printDir(map[i]->at(j));
                    continue;
                }
                 /* if(map[i]->at(j)->parent.size() > 0){
                    std::cout << "i";
                    continue;
                }  */
                if(map[i]->at(j)->isWall){
                    std::cout << "#";
                }else if(map[i]->at(j) == startTile){
                    std::cout << "S";
                }else if(map[i]->at(j) == endTile){
                    std::cout << "E";
                }
                else {
                    std::cout << ".";
                }
            }
            std::cout << std::endl;
        }
    }

    Tile* getLowestF(){
        Tile* lowest = openList.at(0);
        for(int i = 1; i < openList.size(); i++){
            if(openList.at(i)->f < lowest->f){
                lowest = openList.at(i);
            }
        }
        return lowest;
    }

    void doAstar(){

        std::vector<Path*> openpaths;
        Path* current = new Path();
        current->path.push_back(startTile);
        openpaths.push_back(current);
        long long junction = 0;

        while(openpaths.size() > 0){
            current = openpaths.at(openpaths.size() - 1);
            Tile* currentTile = current->path.at(current->path.size() - 1);

            std::vector<Tile*>* neighbours = getNeighbours(currentTile);
            
            Path* savedCp = deepCopy(current);

            int saved = 0;

            for(int i = 0; i < neighbours->size(); i++){
                Tile* neighbour = neighbours->at(i);
                if(current->hasTile(neighbour)){
                    continue;
                }
                if(107468 < current->score){
                    continue;
                }
                Path* tmp = current;
                if(saved > 0){
                    if(currentTile == startTile){
                        std::cout << "created " << std::endl;
                    }
                    tmp = deepCopy(savedCp);
                    openpaths.push_back(tmp);
                }
                saved++;
                tmp->path.push_back(neighbour);
                if(neighbour->x == endTile->x && neighbour->y == endTile->y + 1){
                    /* for(int k = 0; k < tmp->path.size(); k++){
                    std::cout << tmp->path.at(k)->x << "," << tmp->path.at(k)->y << " ";
                    }    
                     std::cout << std::endl; */
                     std::cout << "stack: " << openpaths.size() << " closed " << paths.size() << " score " << bestScore << std::endl;
                }
                
               
                Direction d = detDirection(currentTile, neighbour);
                int rot = 0;
                if(d != tmp->dir){
                    rot = 1000;
                }
                tmp->score = tmp->score + 1 + rot;
                tmp->dir = d;
                /* if(tmp->score > bestScore){
                    openpaths.erase(openpaths.begin() + openpaths.size() - 1);
                    delete tmp;
                    continue;
                } */
                if(neighbour == endTile){
                    //std::cout << "found " << tmp->score << std::endl;
                    openpaths.erase(openpaths.begin() + openpaths.size() - 1);
                    if(bestScore > tmp->score || bestScore == 0){

                        paths.clear();
                        bestScore = tmp->score;
                        paths.push_back(tmp);
                    }else if(bestScore == tmp->score){
                        paths.push_back(tmp);
                    }
                }
            }
            /* if(currentTile->x == 3 && currentTile->y == 13){
                std::cout << currentTile->x << "," << currentTile->y << " neighbours: " << neighbours->size()  << " saved: " << saved  << std::endl;
            } */
           if(saved > 1){
            junction++;
            if(junction % 100000 == 0){
                std::cout << "stack: " << openpaths.size() << " closed " << paths.size() << " score " << bestScore << " junctions " << junction << std::endl;
            }
                
           }
            if(saved == 0){
                //std::cout << currentTile->x << "," << currentTile->y <<  " deleted"  << std::endl;
                delete current;
                openpaths.erase(openpaths.begin() + openpaths.size() - 1);
            }
            delete neighbours;
            delete savedCp;
        }

    }
};

void pathing(Tile* tile){
    if(tile->parent.size() > 1){
        std::cout << "double" << std::endl;
    }
    tile->path = true;
    for(int i = 0; i < tile->parent.size(); i++){
        pathing(tile->parent.at(i));
    }
}


int main()
{

    std::ifstream file("input.txt");
    // std::cout << "hello" << std::endl;
    std::string line;
    unsigned long long total = 0;
    long long xTotal = 0;
    std::vector<std::string> lines;

    Map map;

    while (std::getline(file, line))
    {
        map.parseLine(line);

    }

    //map.printMap();
    map.doAstar();
    
    std::cout << "paths: " << map.paths.size() << std::endl;

    total = map.paths[0]->score;


    for(int i = 0; i < map.paths.size(); i++){
        Path* cur = map.paths.at(i);
        for(int j = 0; j < cur->path.size(); j++){
            cur->path.at(j)->path = true;
        }
    }

    map.printMap();

    for(int i = 0; i < map.map.size(); i++){
        for(int j = 0; j < map.map.at(i)->size(); j++){
            if(map.map.at(i)->at(j)->path){
                xTotal += 1;
            }
        }
    }

    std::cout << total << std::endl;
    std::cout << xTotal << std::endl;
    //107468 for my input
    return 0;
}