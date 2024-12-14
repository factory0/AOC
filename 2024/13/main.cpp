#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <utility>

class Equation{
public:
    long long x1;
    long long x2;
    long long y1;
    long long y2;
    long long r1;
    long long r2;
    bool solved;
    long long x;
    long long y;

    Equation(){
        this->x1 = 0;
        this->x2 = 0;
        this->y1 = 0;
        this->y2 = 0;
        this->r1 = 0;
        this->r2 = 0;
        solved = false;
        x = 0;
        y = 0;
    }

    void print(){
        std::cout << x1 << "+" << y1 << "=" << r1 << std::endl << x2 << "+" << y2 << "=" << r2 << std::endl << std::endl;
    }

    void solve(){

        long long midY = y2*x1 - y1*x2;
        long long midR = r2*x1 - r1*x2;

        //std::cout << midY << " " << midR << std::endl;

        if(midR % midY != 0){
            return;
        }
        y = midR/midY;
        long long midX = r1-y1*y;
        if(midX % x1 != 0){
            return;
        }
        x = midX/x1;
        //std::cout << x << " " << y << std::endl;
        /*if(x > 0 && x <= 100 && y > 0 && y <= 100){
            solved = true;    
        }*/
       if(x > 0 && y > 0){
        solved = true;
       }
    }

    long long giveTokens(){
        if(!solved){
            return 0;
        }
        return 3*x+y;
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

    std::vector<Equation*> equations;

    Equation* equation = new Equation();
    equations.push_back(equation);

    while (std::getline(file, line))
    {
        if(line == ""){
            equation = new Equation();
            equations.push_back(equation);
            continue;
        }
        int xpos = line.find("X");
        int ypos = line.find("Y");
        int mid = line.find(",");
        long long n1 = atoll(line.substr(xpos+2, mid-xpos-2).c_str());
        long long n2 = atoll(line.substr(ypos+2, line.length()-ypos-2).c_str());

        if(line.find("Button A") != std::string::npos){
            equation->x1 = n1;
            equation->x2 = n2;
        }else if(line.find("Button B") != std::string::npos){
            equation->y1 = n1;
            equation->y2 = n2;
        }else{
            equation->r1 = n1+10000000000000;
            equation->r2 = n2+10000000000000;
        }

    }

    for(int i = 0; i < equations.size(); i++){
        //equations[i]->print();
        equations[i]->solve();

        total += equations[i]->giveTokens();

        //std::cout << equations[i]->solved << std::endl;
    }

    
    

    std::cout << total << std::endl;
    std::cout << xTotal << std::endl;

    return 0;
}