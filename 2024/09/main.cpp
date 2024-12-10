#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>

class Block{
public:
    long id;
    bool isSpace;


    Block(long id, bool isSpace){
        this->id = id;
        this->isSpace = isSpace;
    }
};

class EFS{
public:
    std::vector<Block*> blocks;

    void addBlock(long id, bool isSpace){
        Block* block = new Block(id, isSpace);
        blocks.push_back(block);
    }

    long findLastNonSpace(){
        for(long i = blocks.size() - 1; i >= 0; i--){
            if(!blocks[i]->isSpace){
                return i;
            }
        }
    }

    long findFirstSpace(){
        for(long i = 0; i < blocks.size(); i++){
            if(blocks[i]->isSpace){
                return i;
            }
        }
    }

    void swap(long id1, long id2){
        Block* temp = blocks[id1];
        blocks[id1] = blocks[id2];
        blocks[id2] = temp;
    }

    void doSwapparooooos(){
        long lastNonSpace = findLastNonSpace();
        long firstSpace = findFirstSpace();
        while(firstSpace < lastNonSpace){
            swap(firstSpace, lastNonSpace);
            lastNonSpace = findLastNonSpace();
            firstSpace = findFirstSpace();
        }
    }

    void print(){
        for(long i = 0; i < blocks.size(); i++){
            if(blocks[i]->isSpace){
                std::cout << ".";
            }else{
                std::cout << blocks[i]->id;
            }
            
        }
        std::cout << std::endl;
    }

    long long computeCheckSum(){
        long long ret = 0;
        for(int i = 0; i < blocks.size(); i++){
            if(!blocks[i]->isSpace){
                ret += blocks[i]->id*i;
            }
        }
        return ret;
    }

    int lengthOfFile(int id){
        int ret = 0;
        int idx = id;
        Block* b = blocks[id];
        while((idx >= 0) && (b->id == blocks[idx]->id) && (!blocks[idx]->isSpace)){
            ret++;
            idx--;
            //std::cout << "l " << idx << " " << ret << std::endl;
        }
        return ret;
    }

    int lengthOfSpace(int id){
        int ret = 0;
        int idx = id;
        while((idx < blocks.size()) && (blocks[idx]->isSpace)){
            ret++;
            idx++;
        }
        return ret;
    }

    long findNearestBigSpace(int length){
        for(int i = 0; i < blocks.size(); i++){
            if(blocks[i]->isSpace){
                if(lengthOfSpace(i) >= length){
                    return i;
                }
            }
        }
        return -1;
    }

    void fileSwaperoooo(){
        for(int i = blocks.size() - 1; i >= 0; i--){
            if(!blocks[i]->isSpace){
                int length = lengthOfFile(i);
                //std::cout << i << std::endl;
                //std::cout << "length " << length << std::endl;
                int idx = findNearestBigSpace(length);
                //std::cout << "space " << idx << std::endl;
                if(idx != -1 && (idx < i)){
                    for(int j = 0; j < length; j++){
                        swap(i - j, idx + j);
                        //std::cout << i-j << " " << idx + j << std::endl;
                    }
                    
                }
                i -= length - 1;
                //std::cout << length << " " << idx << " " << i << std::endl;
                //std::cout << i <<std::endl;
                //print();
            }
        }
    }
};

int main()
{

    std::ifstream file("input.txt");
    // std::cout << "hello" << std::endl;
    std::string line;
    long long total = 0;
    std::vector<std::string> lines;

    EFS efs = EFS();

    while (std::getline(file, line))
    {
        long id = 0;
        bool isSpace = false;
        for(int i = 0; i < line.length(); i++){
            for(int j = 0; j < line[i] - '0'; j++){
                efs.addBlock(id, isSpace);
            }
            if(!isSpace){
                id++;
            }
            isSpace = !isSpace;
        }
    }
    std::cout << efs.blocks.size() << std::endl;
    efs.print();
    //efs.doSwapparooooos();
    efs.fileSwaperoooo();
    efs.print();

    total = efs.computeCheckSum();

    // std::cout << guard->x << " " << guard->y << std::endl;
    std::cout << total << std::endl;

    return 0;
}