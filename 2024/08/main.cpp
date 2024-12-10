#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


class Tile{
public:
    int x;
    int y;

    bool isAntenna;
    char freq;

    std::vector<char> antinodes;

    Tile(int x, int y, bool isAntenna, char freq){
        this->x = x;
        this->y = y;
        this->isAntenna = isAntenna;
        this->freq = freq;
    }

};


class Map{
public:
    std::vector<std::vector<Tile*>*> tiles;

    std::vector<char> freqCalculated;

    void parseLine(std::string line){
        std::vector<Tile*>* row = new std::vector<Tile*>();

        for(int i = 0; i < line.length(); i++){
            if((line[i] >= '0' && line[i] <= '9') || (line[i] >= 'A' && line[i] <= 'Z') || (line[i] >= 'a' && line[i] <= 'z')){
                Tile* tile = new Tile(i, tiles.size(), true, line[i]);
                row->push_back(tile);
            }else{
                Tile* tile = new Tile(i, tiles.size(), false, '.');
                row->push_back(tile);
            }
        }
        tiles.push_back(row);
    }

    void addAntinode(int x, int y, char antinode){
        Tile* tile = tiles[y]->at(x);
        for(int i = 0; i < tile->antinodes.size(); i++){
            if(tile->antinodes[i] == antinode){
                return;
            }
        }
        //std::cout << x << " " << y << " " << antinode << std::endl;
        tile->antinodes.push_back(antinode);
    }

    void distance(Tile* tile1, Tile* tile2, int* x, int* y){
        *x = tile2->x - tile1->x;
        *y = tile2->y - tile1->y;
    }

    bool checkIfOnMap(int x, int y, bool inv, Tile* tile){
        if(inv){
            x = -x;
            y = -y;
        }

        int xNew = tile->x + x;
        int yNew = tile->y + y;

        if(xNew < 0 || yNew < 0 || xNew >= tiles[0]->size() || yNew >= tiles.size()){
            return false;
        }
        //std::cout << xNew << " " << yNew << " " << tile->freq << std::endl;
        addAntinode(xNew, yNew, tile->freq);
        return true;
    }

    void findAllOthers(Tile* tile){
        /*for(int i = 0; i < freqCalculated.size(); i++){
            if(freqCalculated[i] == tile->freq){
                return;
            }
        }*/
        for(int y = 0; y < tiles.size(); y++){
            for(int x = 0; x < tiles[0]->size(); x++){
                Tile* tmp = tiles[y]->at(x);
                if(x == tile->x && y == tile->y){
                    continue;
                }
                if(!tmp->isAntenna){
                    continue;
                }
                if(tmp->freq != tile->freq){
                    continue;
                }
                int dx;
                int dy;
                distance(tile, tmp, &dx, &dy);
                /*p2 tower is also antiodote if multiple of them*/
                addAntinode(x, y, tile->freq);
                //std::cout << dx << " " << dy << " " << tile->freq << std::endl;
                /*p2 multiplier*/
                int mp = 1;
                while(checkIfOnMap(dx*mp, dy*mp, true, tile)){
                    mp++;
                }
                //checkIfOnMap(dx, dy, true, tmp);
            }
        }
        freqCalculated.push_back(tile->freq);
    }

    void calculate(){
        for(int y = 0; y < tiles.size(); y++){
            for(int x = 0; x < tiles[y]->size(); x++){
                Tile* tile = tiles[y]->at(x);
                if(tile->isAntenna){
                    findAllOthers(tile);
                }
            }
        }
    }

    int totalDisturbances(){
        int total = 0;
        for(int y = 0; y < tiles.size(); y++){
            for(int x = 0; x < tiles[y]->size(); x++){
                Tile* tile = tiles[y]->at(x);
                if(tile->antinodes.size() > 0){
                    total++;
                }
            }
        }
        return total;
    }

    void print(){
        for(int y = 0; y < tiles.size(); y++){
            for(int x = 0; x < tiles[y]->size(); x++){
                Tile* tile = tiles[y]->at(x);
                if(tile->isAntenna){
                    std::cout << tile->freq;
                }else{
                    if(tile->antinodes.size() > 0){
                        std::cout << "#";
                    }else{
                        std::cout << ".";
                    }
                }
            }
            std::cout << std::endl;
        }
    }

};


int main() {
    std::ifstream file("input.txt");
    // std::cout << "hello" << std::endl;
    std::string line;
    long long total = 0;
    long xtotal = 0;
    std::vector<std::string> lines;

    Map map;

    while (std::getline(file, line)) {
        map.parseLine(line);
    }

    map.calculate();
    total = map.totalDisturbances();
    map.print();



    // std::cout << guard->x << " " << guard->y << std::endl;
    std::cout << total << std::endl;
    std::cout << xtotal << std::endl;

    return 0;
}