#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>



int main()
{

    std::ifstream file("list.txt");
    //std::cout << "hello" << std::endl;
    std::string line;
    long long total = 0;
    long Xtotal = 0;
    std::vector<std::string> lines;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }

    for(int i = 0; i < lines.size(); i++){
        for(int j = 0; j < lines[i].length(); j++){
            if(lines[i][j] == 'X'){
                bool enoughUp = false;
                bool enoughDown = false;
                bool enoughLeft = false;
                bool enoughRight = false;
                if(j > 2){
                    enoughLeft = true;
                }
                if(i > 2){
                    enoughUp = true;
                }
                if(j < lines[i].length() - 3){
                    enoughRight = true;
                }
                if(i < lines.size() - 3){
                    enoughDown = true;
                }
                if(enoughLeft){
                    if(lines[i][j-1] == 'M' && lines[i][j-2] == 'A' && lines[i][j-3] == 'S'){
                        total++;
                    }
                }
                if(enoughLeft && enoughUp){
                    if(lines[i-1][j-1] == 'M' && lines[i-2][j-2] == 'A' && lines[i-3][j-3] == 'S'){
                        total++;
                    }
                }
                if(enoughUp){
                    if(lines[i-1][j] == 'M' && lines[i-2][j] == 'A' && lines[i-3][j] == 'S'){
                        total++;
                    }
                }
                if(enoughUp && enoughRight){
                    if(lines[i-1][j+1] == 'M' && lines[i-2][j+2] == 'A' && lines[i-3][j+3] == 'S'){
                        total++;
                    }
                }
                if(enoughRight){
                    if(lines[i][j+1] == 'M' && lines[i][j+2] == 'A' && lines[i][j+3] == 'S'){
                        total++;
                    }
                }
                if(enoughRight && enoughDown){
                    if(lines[i+1][j+1] == 'M' && lines[i+2][j+2] == 'A' && lines[i+3][j+3] == 'S'){
                        total++;
                    }
                }
                if(enoughDown){
                    if(lines[i+1][j] == 'M' && lines[i+2][j] == 'A' && lines[i+3][j] == 'S'){
                        total++;
                    }
                }
                if(enoughDown && enoughLeft){
                    if(lines[i+1][j-1] == 'M' && lines[i+2][j-2] == 'A' && lines[i+3][j-3] == 'S'){
                        total++;
                    }
                }
            
            }
            if(lines[i][j] == 'A'){
                bool enoughUp = false;
                bool enoughDown = false;
                bool enoughLeft = false;
                bool enoughRight = false;
                if(j > 0){
                    enoughLeft = true;
                }
                if(i > 0){
                    enoughUp = true;
                }
                if(j < lines[i].length() - 1){
                    enoughRight = true;
                }
                if(i < lines.size() - 1){
                    enoughDown = true;
                }
                if(enoughLeft && enoughUp && enoughRight && enoughDown){
                    bool lefttop = false;
                    bool righttop = false;

                    if((lines[i-1][j-1] == 'M' && lines[i+1][j+1] == 'S') || (lines[i-1][j-1] == 'S' && lines[i+1][j+1] == 'M')){
                        lefttop = true;
                    }
                    if((lines[i-1][j+1] == 'M' && lines[i+1][j-1] == 'S') || (lines[i-1][j+1] == 'S' && lines[i+1][j-1] == 'M')){
                        righttop = true;
                    }
                    if(lefttop && righttop){
                        Xtotal++;
                    }
                }
            }
        }
    }

    std::cout << total << std::endl;
    std::cout << Xtotal << std::endl;

    return 0;
}