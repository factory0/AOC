#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <utility>

enum Neighbours{
    TOP = 0,
    RIGHT = 1,
    BOTTOM = 2,
    LEFT = 3
};

class Tile{
public:
    int x;
    int y;

    char plant;
    bool edge;

    Tile(int x, int y, char plant){
        this->x = x;
        this->y = y;
        this->plant = plant;
        edge = false;
    }
};

class Edge{
public:
    std::vector<Tile*> tiles;
    Neighbours neighbour;

    Edge(Neighbours neighbour){
        this->neighbour = neighbour;
    }
};

class Region{
public:
    std::vector<Tile*> tiles;

    std::vector<Edge*> edges;

    void addUniqueTile(Tile* tile){
        for(int i = 0; i < tiles.size(); i++){
            if(tiles[i]->x == tile->x && tiles[i]->y == tile->y){
                return;
            }
            
        }
        tiles.push_back(tile);
        //std::cout << tile->x << " " << tile->y << std::endl;
    }

    long area(){
        return tiles.size();    
    }
    
    Edge* getEdge(Tile* tile, Neighbours neighbour){
        for(int i = 0; i < edges.size(); i++){
            if(edges[i]->neighbour != neighbour){
                continue;
            }
            for(int j = 0; j < edges[i]->tiles.size(); j++){
                if(edges[i]->tiles[j] == tile){
                    return edges[i];
                }
            }
        }
        return nullptr;
    }

};

class Map{
public:
    std::vector<Region*> regions;
    std::vector<std::vector<Tile*>*> tiles;

    void parseTiles(std::string line){
        std::vector<Tile*>* rowTiles = new std::vector<Tile*>();
        for(int i = 0; i < line.length(); i++){
            rowTiles->push_back(new Tile(i, tiles.size(), line[i]));
        }
        tiles.push_back(rowTiles);
    }

    void printTiles(){
        for(int i = 0; i < tiles.size(); i++){
            for(int j = 0; j < tiles[i]->size(); j++){
                std::cout << tiles[i]->at(j)->plant;
            }
            std::cout << std::endl;
        }
    }

    bool hasSameneighbourTop(Tile* tile){
        if(tile->y == 0){
            return false;
        }
        return tiles[tile->y - 1]->at(tile->x)->plant == tile->plant;
    }

    bool hasSameneighbourBottom(Tile* tile){
        if(tile->y == tiles.size() - 1){
            return false;
        }
        return tiles[tile->y + 1]->at(tile->x)->plant == tile->plant;
    }

    bool hasSameneighbourLeft(Tile* tile){
        if(tile->x == 0){
            return false;
        }
        return tiles[tile->y]->at(tile->x - 1)->plant == tile->plant;
    }

    bool hasSameneighbourRight(Tile* tile){
        if(tile->x == tiles[tile->y]->size() - 1){
            return false;
        }
        return tiles[tile->y]->at(tile->x + 1)->plant == tile->plant;
    }

    Tile* getNeighbour(Tile* tile, Neighbours neighbour){
        switch(neighbour){
            case TOP:
                if(tile->y == 0){
                    return nullptr;
                }
                return tiles[tile->y - 1]->at(tile->x);
            case RIGHT:
                if(tile->x == tiles[tile->y]->size() - 1){
                    return nullptr;
                }
                return tiles[tile->y]->at(tile->x + 1);
            case BOTTOM:
                if(tile->y == tiles.size() - 1){
                    return nullptr;
                }
                return tiles[tile->y + 1]->at(tile->x);
            case LEFT:
                if(tile->x == 0){
                    return nullptr;
                }
                return tiles[tile->y]->at(tile->x - 1);
        }
    }

    Region* getRegion(Tile* tile){
        for(int i = 0; i < regions.size(); i++){
            for(int j = 0; j < regions[i]->tiles.size(); j++){
                if(regions[i]->tiles[j] == tile){
                    return regions[i];
                }
            }
        }
        return nullptr;
    }

    void growRegion(Tile* tile, Region* region){
        if(region == nullptr){
            region = new Region();
            regions.push_back(region);
        }
        if(getRegion(tile) != nullptr){
            return;
        }
        Tile* top = getNeighbour(tile, TOP);
        Tile* right = getNeighbour(tile, RIGHT);
        Tile* bottom = getNeighbour(tile, BOTTOM);
        Tile* left = getNeighbour(tile, LEFT);
        region->addUniqueTile(tile);
        if(top != nullptr && hasSameneighbourTop(tile)){
            growRegion(top, region);
        }
        if(right != nullptr && hasSameneighbourRight(tile)){
            growRegion(right, region);
        }
        if(bottom != nullptr && hasSameneighbourBottom(tile)){
            growRegion(bottom, region);
        }
        if(left != nullptr && hasSameneighbourLeft(tile)){
            growRegion(left, region);
        }
    }

    void parseRegions(){
        for(int i = 0; i < tiles.size(); i++){
            for(int j = 0; j < tiles[i]->size(); j++){
                if(getRegion(tiles[i]->at(j)) != nullptr){
                    continue;
                }
                //std::cout << tiles[i]->at(j)->x << " " << tiles[i]->at(j)->y << " " << tiles[i]->at(j)->plant << std::endl;
                growRegion(tiles[i]->at(j), nullptr);
            }
        }
    }

    long perimeter(Region* region){
        long total = 0;
        for(int i = 0; i < region->tiles.size(); i++){
            if(!hasSameneighbourTop(region->tiles[i])){
                total += 1;
                region->tiles[i]->edge = true;
            }
            if(!hasSameneighbourRight(region->tiles[i])){
                total += 1;
                region->tiles[i]->edge = true;
            }
            if(!hasSameneighbourBottom(region->tiles[i])){
                total += 1;
                region->tiles[i]->edge = true;
            }
            if(!hasSameneighbourLeft(region->tiles[i])){
                total += 1;
                region->tiles[i]->edge = true;
            }
        }
        return total;
    }

    void growEdge(Region* region, Tile* tile, Edge* edge, Neighbours neighbour){
        if(tile == nullptr){
            return;
        }
        if(!tile->edge){
            return;
        }
        if(getRegion(tile) != region){
            return;
        }
        if(region->getEdge(tile, neighbour) != nullptr){
            return;
        }
        bool add = false;
        switch(neighbour){
            case TOP:
                if(!hasSameneighbourTop(tile)){
                    add = true;
                }
                break;
            case RIGHT:
                if(!hasSameneighbourRight(tile)){
                    add = true;
                }
                break;
            case BOTTOM:
                if(!hasSameneighbourBottom(tile)){
                    add = true;
                }
                break;
            case LEFT:
                if(!hasSameneighbourLeft(tile)){
                    add = true;
                }
                break;
        }
        if(!add){
            return;
        }
        if(edge == nullptr){
            edge = new Edge(neighbour);
            region->edges.push_back(edge);
        }
        //std::cout << tile->x << " " << tile->y << " " << neighbour << std::endl;
        edge->tiles.push_back(tile);
        Tile* n1;
        Tile* n2;
        switch(neighbour){
            case TOP:
            case BOTTOM:
                n1 = getNeighbour(tile, LEFT);
                n2 = getNeighbour(tile, RIGHT);
                break;
            case RIGHT:
            case LEFT:
                n1 = getNeighbour(tile, TOP);
                n2 = getNeighbour(tile, BOTTOM);
                break;
        }
        growEdge(region, n1, edge, neighbour);
        growEdge(region, n2, edge, neighbour);
    }

    void parseEdges(Region* region){
        for(int i = 0; i < region->tiles.size(); i++){
            if(region->tiles[i]->edge == false){
                continue;
            }
            //std::cout << "edge" << std::endl;
            if(region->getEdge(region->tiles[i], TOP) == nullptr){
                growEdge(region, region->tiles[i], nullptr, TOP);
            }
            if(region->getEdge(region->tiles[i], RIGHT) == nullptr){
                growEdge(region, region->tiles[i], nullptr, RIGHT);
            }
            if (region->getEdge(region->tiles[i], BOTTOM) == nullptr)
            {
                growEdge(region, region->tiles[i], nullptr, BOTTOM);
            }
            if(region->getEdge(region->tiles[i], LEFT) == nullptr){
                growEdge(region, region->tiles[i], nullptr, LEFT);
            }
            
        }
        //std::cout << region->edges.size() << std::endl;
    }

    long newPerimeter(Region* region){
        return region->edges.size();
    }

    long price(Region* region){
        long per = perimeter(region);
        long area = region->area();
        return per * area;
    }

    long newPrice(Region* region){
        long per = newPerimeter(region);
        long area = region->area();
        return per * area;
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

    while (std::getline(file, line))
    {
        map.parseTiles(line);
    }

    map.printTiles();
    map.parseRegions();
    
    //std::cout << "grew" << std::endl;
    
    for(int i = 0; i < map.regions.size(); i++){
        /*
        for(int j = 0; j < map.regions[i]->tiles.size(); j++){
            std::cout << map.regions[i]->tiles[j]->x << " " << map.regions[i]->tiles[j]->y << " " << map.regions[i]->tiles[j]->plant << std::endl;
        }
        std::cout << map.price(map.regions[i]) << std::endl;
        std::cout << std::endl;*/
        
        total += map.price(map.regions[i]);
        map.parseEdges(map.regions[i]);
        xTotal += map.newPrice(map.regions[i]);

    }

    

    

    std::cout << total << std::endl;
    std::cout << xTotal << std::endl;

    return 0;
}