#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum Operator{
    ADD = 0,
    MUL,
    CON
};

class Equation{
public:
    long long result;
    std::vector<long long> terms;
    std::vector<Operator> ops;
    bool possible;
    long perm;
    long maxperms;

    Equation(){
        result = 0;
        possible = false;
        perm = 0;
        maxperms = 0;
    }

    void parse(std::string line){
        int pos = line.find(":");
        result = std::stoll(line.substr(0, pos));
        int start = pos + 2;
        pos = line.find(" ", start);
        while(pos != std::string::npos){
            terms.push_back(std::stoll(line.substr(start, pos - start)));
            start = pos + 1;
            pos = line.find(" ", start);
        }
        terms.push_back(std::stol(line.substr(start)));
        /*std::cout << result << " ";
        for(long t: terms){
            std::cout << t << " ";
        }
        std::cout << std::endl;*/
    }

    long long con(long long a, long long b){
        int dig = 0;
        long long tmp = b;
        while(tmp > 0){
            tmp /= 10;
            dig++;
        }
        //std::cout << a << " " << b << " " << a * pow(10, dig) + b << std::endl;
        return a * pow(10, dig) + b;
    }

    long long duo(long long a, long long b, Operator op){
        switch(op){
            case ADD:
                return a + b;
            case MUL:
                return a * b;
            case CON:
                return con(a, b);
        }
        return 0;
    }

    bool checkperm(){
        long long tmp = 0;
        long long x = terms[0];
        long long y = terms[1];
        tmp += duo(x, y, ops[0]);
        int p = ops[0];
        //std::cout << x << " " << y << " " << p << std::endl;
        for(int i = 1; i < terms.size() - 1; i++){
            //std::cout << tmp << " " << duo(tmp, terms[i + 1], ops[i]) << std::endl;
            if(tmp > result){
                return false;
            }
            tmp = duo(tmp, terms[i + 1], ops[i]); 
        }
        return tmp == result;
    }

    void nextperm(){
        perm++;
        long tmp = perm;
        for(int i = ops.size() - 1; i >= 0; i--){
            if((tmp % 3) == 1){
                ops[i] = MUL;
            }else if((tmp % 3) == 2){
                ops[i] = CON;
            }else{
                ops[i] = ADD;
            }
            tmp /= 3;
        }
    }

    void check(){
        for(int i = 0; i < terms.size() - 1; i++){
            ops.push_back(ADD);
        }
        maxperms = pow(3, ops.size());
        //std::cout << maxperms << std::endl;
        for(int i = 0; i < maxperms; i++){
            /*for(Operator op: ops){
                std::cout << op << " ";
            }
            std::cout << std::endl;*/
            if(checkperm()){
                possible = true;
                break;
            }
            nextperm();
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

    std::vector<Equation*> equations;

    while (std::getline(file, line)) {
        Equation* eq = new Equation();
        eq->parse(line);
        equations.push_back(eq);
    }

    for(Equation* eq: equations){
        eq->check();
        if(eq->possible){
            total += eq->result;
        }
    }


    // std::cout << guard->x << " " << guard->y << std::endl;
    std::cout << total << std::endl;
    std::cout << xtotal << std::endl;

    return 0;
}