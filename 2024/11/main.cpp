#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <utility>

std::map<std::pair<unsigned long long, unsigned long long>, unsigned long long> memo;

unsigned long long pwr(unsigned long long a, unsigned long long b){
    unsigned long long tmp = a;
    if(b == 0){
        return 1;
    }
    for(int i = 0; i < b-1; i++){
        tmp *= a;
    }
    return tmp;
}

class Stone{
public:
    unsigned long long value;

    Stone(unsigned long long value){
        this->value = value;
    }

    int DigitsCount(){
        int dig = 0;
        unsigned long long tmp = value;
        while(tmp > 0){
            dig++;
            tmp /= 10;
        }
        return dig;
    }

    void leftRight(unsigned long long *left, unsigned long long *right){
        int digs = DigitsCount()/2;
        unsigned long long div = pwr(10, digs);
        *left = value / div;
        *right = value % div;
        //std::cout << *left << " " << *right << " " << div  << " " << digs << std::endl;
    }

    unsigned long long goDeep(int depth, int finalDepth){
        unsigned long long orig = value;
        if(memo.find(std::make_pair(value, depth)) != memo.end()){
            //std::cout << "found" << std::endl;
            //std::cout << value << " " << depth << " " << memo[std::make_pair(value, depth)] << std::endl;
            return memo[std::make_pair(value, depth)];
        }
        if(depth == finalDepth){
            //std::cout << "reached" << std::endl;
            return 1;
        }
        if(value == 0){
            value = 1;
            unsigned long long tot = goDeep(depth + 1, finalDepth);
            memo[std::make_pair(orig, depth)] = tot;
            return tot;
        }
        if(DigitsCount() % 2 == 0){
            unsigned long long left, right;
            leftRight(&left, &right);
            Stone* nw = new Stone(right);
            value = left;
            unsigned long long tot = nw->goDeep(depth + 1, finalDepth) + goDeep(depth + 1, finalDepth);
            memo[std::make_pair(orig, depth)] = tot;
            delete nw;
            return tot;
        }
        value = value * 2024;
        unsigned long long tot = goDeep(depth + 1, finalDepth);
        memo[std::make_pair(orig, depth)] = tot;
        return tot;
    }
};

class Line{
public:
    std::vector<Stone*> stones;

    Line(){

    }

    void parseLine(std::string line){
        int pos = line.find(" ");
        int start = 0;
        while(pos != std::string::npos){
            std::string s = line.substr(start, pos - start);
            start = pos + 1;
            pos = line.find(" ", pos + 1);
            Stone* stone = new Stone(atoll(s.c_str()));
            stones.push_back(stone);
        }
        std::string s = line.substr(start, line.size() - start);
        Stone* stone = new Stone(atoll(s.c_str()));
        stones.push_back(stone);
    }

    void printStones(){
        for(int i = 0; i < stones.size(); i++){
            std::cout << stones[i]->value << " ";
        }
        std::cout << std::endl;
    }

    void doRound(){
        for(int i = stones.size() - 1; i >= 0; i--){
            if(stones[i]->value == 0){
                stones[i]->value = 1;
                continue;
            }
            //std::cout << stones[i]->value << " " << stones[i]->DigitsCount() << std::endl;
            if(stones[i]->DigitsCount() % 2 == 0){
                unsigned long long left, right;
                stones[i]->leftRight(&left, &right);
                stones.push_back(new Stone(right));
                stones[i]->value = left;
                continue;
            }
            stones[i]->value = stones[i]->value * (unsigned long long)2024;
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

    //Line stoneLine;
    std::vector<Line*> stoneLines;

    while (std::getline(file, line))
    {
        Line* stoneLine = new Line();
        stoneLine->parseLine(line);
        stoneLines.push_back(stoneLine);
    }

    //stoneLine.printStones();
    /*for(int i = 0; i < 75; i++){
        unsigned long long total = 0;
        for(int j = 0; j < 1; j++){
            
            stoneLines[j]->doRound();
            total += stoneLines[j]->stones.size();
        }
        std::cout << i  << " " << total << std::endl;
        //stoneLine.printStones();
    }*/

    for(int i = 0; i < stoneLines.size(); i++){
        //std::cout << "stone " << i << std::endl;
        total += stoneLines[i]->stones[0]->goDeep(0, 75);
        //std::cout << total << std::endl;
    }
    
    //total = stoneLine.stones.size();

    // std::cout << guard->x << " " << guard->y << std::endl;
    std::cout << total << std::endl;
    std::cout << memo.size() << std::endl;

    return 0;
}