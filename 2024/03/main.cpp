#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>

long long processmul(std::string line){
    int pos = line.find(",");
    std::string first = line.substr(0, pos);
    std::string second = line.substr(pos + 1);
    //std::cout << first << " " << second << std::endl;
    return std::stoll(first) * std::stoll(second);
}

int main()
{

    std::ifstream file("list.txt");
    //std::cout << "hello" << std::endl;
    std::string line;
    long long total = 0;

    bool enable = true;

    while (std::getline(file, line))
    {
        //std::cout << line << std::endl;
        int start = 0;
        int end = line.find("mul");
        while (end != std::string::npos)
        {
            std::string sub = line.substr(start, end - start);
            std::size_t dos = std::string::npos, donts = std::string::npos, lastdos= std::string::npos, lastdonts= std::string::npos;
            int idx = 0;
            do{
                lastdos = dos;
                dos = sub.find("do()", idx);
                idx = dos + 1;
            }while(dos != std::string::npos);
            dos = lastdos;

            idx = 0;
            do{
                lastdonts = donts;
                donts = sub.find("don\'t()", idx);
                idx = donts + 1;
            }while(donts != std::string::npos);
            donts = lastdonts;

            //std::cout << dos << " " << donts << " " << end << std::endl;

            if(dos != std::string::npos && donts == std::string::npos){
                enable = true;
            }else if(dos == std::string::npos && donts != std::string::npos){
                enable = false;
            }else if(dos != std::string::npos && donts != std::string::npos){
                if(dos > donts){
                    enable = true;
                }else if(donts > dos){
                    enable = false;
                }
            }

            //std::cout << line.substr(start, end - start) << std::endl;
            bool foundFirstbracket = false;
            bool foundSep = false;
            bool foundSecondbracket = false;
            int stopidx = -1;
            for(int i = end+3; i < line.size(); i++){
                if(!foundFirstbracket){
                    if(line[i] == '('){
                        foundFirstbracket = true;
                    }else{
                        break;
                    }
                }else if(!foundSep){
                    if(line[i] >= '0' && line[i] <= '9'){
                        continue;
                    }else if(line[i] == ','){
                        foundSep = true;
                        continue;
                    }else{
                        break;
                    }
                }else if(!foundSecondbracket){
                    if(line[i] >= '0' && line[i] <= '9'){
                        continue;
                    }else if(line[i] == ')'){
                        foundSecondbracket = true;
                        stopidx = i;
                        break;
                    }else{
                        break;
                    }
                }
            }
            if(foundSecondbracket){
                if(enable){
                    std::cout << line.substr(end, stopidx - end+1) << std::endl;
                    std::string sub = line.substr(end + 4, stopidx - end - 4);
                
                    total += processmul(sub);
                }
                
            }
            start = end + 3;
            end = line.find("mul", start);
        }
    }

    std::cout << total << std::endl;

    return 0;
}

