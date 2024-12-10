#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>

class Cubes{
public:
    int red;
    int green;
    int blue;
    Cubes(int r, int g, int b){
        red = r;
        green = g;
        blue = b;
    }
};


class Game{
public:
    std::vector<Cubes*> rounds;
    int idx;
    Game(){
        idx = 0;
    }

    void parse(std::string line){
        int sep = line.find(":");
        idx = std::stoi(line.substr(line.find(" ") + 1, sep - line.find(" ") - 1));

        int pos = line.find(";");
        int start = sep;
        while(pos != std::string::npos){
            Cubes* cube = new Cubes(0, 0, 0);
            std::string s = line.substr(start + 1, pos - start - 1);
            
            int e = s.find(",");
            int se = -1;
            while(e != std::string::npos){
                std::string sub = s.substr(se + 1, e - se - 1);
                int fr = -1;
                int br;
                for(int i = 0; i < sub.length(); i++){
                    if(sub[i] != ' '){
                        fr = i;
                        break;
                    }
                }
                for(int i = sub.length() - 1; i >= 0; i--){
                    if(sub[i] != ' '){
                        br = i;
                        break;
                    }
                }
                sub = sub.substr(fr, br - fr + 1);

                int mid = sub.find(" ");

                int num = std::stoi(sub.substr(0, mid));

                if(sub.find("red") != std::string::npos){
                    cube->red = num;
                }
                else if(sub.find("green") != std::string::npos){
                    cube->green = num;
                }
                else if(sub.find("blue") != std::string::npos){
                    cube->blue = num;
                }

                //std::cout << sub << std::endl;

                se = e;
                e = s.find(",", e+1);    
            }
            std::string sub = s.substr(se + 1, s.length() - se - 1);
                int fr = -1;
                int br;
                for(int i = 0; i < sub.length(); i++){
                    if(sub[i] != ' '){
                        fr = i;
                        break;
                    }
                }
                for(int i = sub.length() - 1; i >= 0; i--){
                    if(sub[i] != ' '){
                        br = i;
                        break;
                    }
                }
                sub = sub.substr(fr, br - fr + 1);

                int mid = sub.find(" ");

                int num = std::stoi(sub.substr(0, mid));

                if(sub.find("red") != std::string::npos){
                    cube->red = num;
                }
                else if(sub.find("green") != std::string::npos){
                    cube->green = num;
                }
                else if(sub.find("blue") != std::string::npos){
                    cube->blue = num;
                }


            //std::cout << cube->red << " " << cube->green << " " << cube->blue << std::endl;
            start = pos + 1;
            rounds.push_back(cube);
            pos = line.find(";", pos + 1);
        }

        Cubes* cube = new Cubes(0, 0, 0);
            std::string s = line.substr(start + 1, line.length() - start - 1);
            
            int e = s.find(",");
            int se = -1;
            while(e != std::string::npos){
                std::string sub = s.substr(se + 1, e - se - 1);
                int fr = -1;
                int br;
                for(int i = 0; i < sub.length(); i++){
                    if(sub[i] != ' '){
                        fr = i;
                        break;
                    }
                }
                for(int i = sub.length() - 1; i >= 0; i--){
                    if(sub[i] != ' '){
                        br = i;
                        break;
                    }
                }
                sub = sub.substr(fr, br - fr + 1);

                int mid = sub.find(" ");

                int num = std::stoi(sub.substr(0, mid));

                if(sub.find("red") != std::string::npos){
                    cube->red = num;
                }
                else if(sub.find("green") != std::string::npos){
                    cube->green = num;
                }
                else if(sub.find("blue") != std::string::npos){
                    cube->blue = num;
                }

                //std::cout << sub << std::endl;

                se = e;
                e = s.find(",", e+1);    
            }
            std::string sub = s.substr(se + 1, s.length() - se - 1);
                int fr = -1;
                int br;
                for(int i = 0; i < sub.length(); i++){
                    if(sub[i] != ' '){
                        fr = i;
                        break;
                    }
                }
                for(int i = sub.length() - 1; i >= 0; i--){
                    if(sub[i] != ' '){
                        br = i;
                        break;
                    }
                }
                sub = sub.substr(fr, br - fr + 1);

                int mid = sub.find(" ");

                int num = std::stoi(sub.substr(0, mid));

                if(sub.find("red") != std::string::npos){
                    cube->red = num;
                }
                else if(sub.find("green") != std::string::npos){
                    cube->green = num;
                }
                else if(sub.find("blue") != std::string::npos){
                    cube->blue = num;
                }


            //std::cout << cube->red << " " << cube->green << " " << cube->blue << std::endl;
            start = pos + 1;
            rounds.push_back(cube);
        
    }

    int valid(){
        for(int i = 0; i < rounds.size(); i++){
            if(rounds[i]->red > 12){
                return 0;
            }
            if(rounds[i]->green > 13){
                return 0;
            }
            if(rounds[i]->blue > 14){
                return 0;
            }
        }
        return idx;
    }

    long score(){
        long ret = 0;
        int minred = rounds[0]->red;
        int mingreen = rounds[0]->green;
        int minblue = rounds[0]->blue;
        for(int i = 0; i < rounds.size(); i++){
            if(rounds[i]->red > minred){
                minred = rounds[i]->red;
            }
            if(rounds[i]->green > mingreen){
                mingreen = rounds[i]->green;
            }
            if(rounds[i]->blue > minblue){
                minblue = rounds[i]->blue;
            }
        }
        return minred * mingreen * minblue;
    }
};



int main()
{

    std::ifstream file("list.txt");
    //std::cout << "hello" << std::endl;
    std::string line;
    long long total = 0;


long long mastery = 0;
    std::vector<Game*> games;

    while (std::getline(file, line))
    {
        Game* game = new Game();
        game->parse(line);
        games.push_back(game);
        //std::cout << game->idx << std::endl;
        
    }

    for(int i = 0; i < games.size(); i++){
        total += games[i]->valid();
        mastery += games[i]->score();
    }

    std::cout << total << std::endl;
    std::cout << mastery << std::endl;

    return 0;
}

