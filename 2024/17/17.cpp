#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <bitset>

unsigned long long pow2(unsigned int x) {
    unsigned int res = 1;
    for (int i = 0; i < x; i++) {
        res *= 2;
    }
    return res;
}

class Procesor {
   public:
    unsigned long long A;
    unsigned long long B;
    unsigned long long C;

    int ip;

    std::string output;

    std::vector<unsigned int> instructions;

    Procesor() {
        A = 0;
        B = 0;
        C = 0;
        ip = 0;
    }

    unsigned long long getCombo(int combo) {
        if (combo >= 0 && combo <= 3) {
            return combo;
        }

        if (combo == 4) {
            return A;
        }

        if (combo == 5) {
            return B;
        }

        if (combo == 6) {
            return C;
        }
        return 0;
    }

    void adv() {
        unsigned long long combo = getCombo(instructions[ip + 1]);
        unsigned long long nom = A;
        unsigned long long den = pow2(combo);
        unsigned long long res = nom / den;
        A = res;
        ip += 2;
    }

    void bxl() {
        unsigned long long lit = instructions[ip + 1];
        unsigned long long BT = B;

        unsigned long long res = BT ^ lit;
        B = res;
        ip += 2;
    }

    void bst() {
        unsigned long long combo = getCombo(instructions[ip + 1]);
        unsigned long long res = combo % 8;
        B = res;
        ip += 2;
    }

    void jnz() {
        if (A == 0) {
            ip += 2;
            return;
        }
        unsigned int lit = instructions[ip + 1];
        ip = lit;
    }

    void bxc() {
        unsigned long long res = B ^ C;
        B = res;
        ip += 2;
    }

    void out() {
        unsigned long long combo = getCombo(instructions[ip + 1]);
        combo = combo % 8;
        std::stringstream ss;
        ss << combo;
        output += ss.str();
        output += ",";
        // std::cout << combo << ",";
        ip += 2;
    }

    void bdv() {
        unsigned long long combo = getCombo(instructions[ip + 1]);
        unsigned long long nom = A;
        unsigned long long den = pow2(combo);
        unsigned long long res = nom / den;
        B = res;
        ip += 2;
    }

    void cdv() {
        unsigned long long combo = getCombo(instructions[ip + 1]);
        unsigned long long nom = A;
        unsigned long long den = pow2(combo);
        unsigned long long res = nom / den;
        C = res;
        ip += 2;
    }

    void doInst() {
        // std::cout << "Doing instruction " << instructions[ip] << " " << ip <<
        // std::endl;
        switch (instructions[ip]) {
            case 0:
                adv();
                break;
            case 1:
                bxl();
                break;
            case 2:
                bst();
                break;
            case 3:
                jnz();
                break;
            case 4:
                bxc();
                break;
            case 5:
                out();
                break;
            case 6:
                bdv();
                break;
            case 7:
                cdv();
                break;
        }
    }

    void doPrg() {
        while (ip < instructions.size()) {
            doInst();
        }
    }

    void parseIp(std::string line) {
        int st = line.find(":") + 2;
        int pos = line.find(",");
        // std::cout << line.substr(st + 1, pos - st - 1) << std::endl;
        while (pos != std::string::npos) {
            instructions.push_back(atoi(line.substr(st, pos - st).c_str()));
            st = pos + 1;
            pos = line.find(",", st);
        }
        instructions.push_back(atoi(line.substr(st + 1).c_str()));
        /* for(int i = 0; i < instructions.size(); i++){
            std::cout << instructions[i] << " ";
        }
        std::cout << std::endl; */
    }

    void parseReg(std::string line) {
        unsigned int num = atol(line.substr(12).c_str());
        if (line.find("A") != std::string::npos) {
            A = num;
        }
        if (line.find("B") != std::string::npos) {
            B = num;
        }
        if (line.find("C") != std::string::npos) {
            C = num;
        }
    }
};

Procesor* deepCopy(Procesor* proc) {
    Procesor* copy = new Procesor();
    copy->A = proc->A;
    copy->B = proc->B;
    copy->C = proc->C;
    copy->ip = proc->ip;
    for (int i = 0; i < proc->instructions.size(); i++) {
        copy->instructions.push_back(proc->instructions[i]);
    }
    return copy;
}

int main() {
    std::ifstream file("17.txt");
    // std::cout << "hello" << std::endl;
    std::string line;
    unsigned long long total = 0;
    long long xTotal = 0;
    std::vector<std::string> lines;

    Procesor proc;

    while (std::getline(file, line)) {
        if (line == "") {
            break;
        }
        proc.parseReg(line);
    }

    std::string target;
    while (std::getline(file, line)) {
        proc.parseIp(line);
        target = line.substr(9);
    }
    Procesor* proc2 = deepCopy(&proc);
    /* std::cout << proc.A << std::endl;
    std::cout << proc.B << std::endl;
    std::cout << proc.C << std::endl;
    std::cout << proc.instructions.size() << std::endl; */

    proc2->doPrg();

    std::cout << proc2->output.substr(0, proc2->output.size() - 1) << std::endl;

    std::cout << target << std::endl;

    bool doIt = true;
    unsigned long long reg = 0;  // 35184382000000;
    reg = ((unsigned long long)23) << 43;
    reg += ((unsigned long long)13) << 32;
    reg = 202367025818154;//202365974096426;
    unsigned long long pot = 1;

    /* Procesor* proc3 = deepCopy(&proc);

    proc3->A = 202367563475498;

    proc3->doPrg();
    std::cout << "test" << std::endl;
    std::cout << proc3->output.substr(0, proc3->output.size() - 1) << std::endl;

    for (int i = 0; i < 1000000; i++) {
        unsigned long long pott = pot + i;
        //pott <<= 40;

            Procesor* part2 = deepCopy(&proc);
            part2->A = pott;

            part2->doPrg();

         if (part2->output.find("7,5,4,7,1") != std::string::npos) {
            std::cout << pott << std::endl;
                std::bitset<sizeof(unsigned long long) * 8> binaryNumber(pott);
                std::cout << binaryNumber  << " " << pott << std::endl;
                std::cout << part2->output.substr(0, part2->output.size() - 1) << std::endl;
                for(int j = 0; j < 30; j++){
                    Procesor* part3 = deepCopy(&proc);
                    part3->A = pott << j;
                    part3->doPrg();
                    std::bitset<sizeof(unsigned long long) * 8> binaryNumber(pott << j);
                    std::cout << binaryNumber  << std::endl;
                    std::cout << part3->output.substr(0, part3->output.size() - 1) << std::endl;
                }
            }

    }

    return 0; */


    /* for (int i = 0; i < 100000000; i++) {
        unsigned long long pott = pot + i;
        //pott <<= 40;
        doIt = true;
        reg = 0;
        //std::cout << reg << std::endl; */
        while (doIt) {
            Procesor* part2 = deepCopy(&proc);
            part2->A = reg;

            part2->doPrg();

             if(reg > 1000000 && reg % 1000000 == 0){
                std::cout << reg << std::endl;
                std::cout << part2->output.substr(0, part2->output.size() - 1)
            << std::endl;
            } 

           /*  if (part2->output.find("7,5,4,7,1") != std::string::npos) {
                std::bitset<sizeof(unsigned long long) * 8> binaryNumber(pott);
                std::cout << binaryNumber  << " " << reg << std::endl;
                std::cout << part2->output.substr(0, part2->output.size() - 1) << std::endl;
            } */

            /* if(part2->output.size() < target.size()){
                //reg += 10000000;
            }  */
            /* std::cout << reg  << std::endl;
            std::cout << part2->output.substr(0, part2->output.size() - 1) <<
            std::endl;  */
            if (target.compare(
                    part2->output.substr(0, part2->output.size() - 1)) == 0) {
                        std::cout << reg  << std::endl;
            std::cout << part2->output.substr(0, part2->output.size() - 1) <<
            std::endl;
                doIt = false;
                break;
            }
            /* if(reg == 1){
                break;
            } */
            reg += (1 << 16);
            //pott <<= 3;
            delete part2;
        }
   //}

    

    std::cout << reg << std::endl;

    // std::cout << total << std::endl;
    // std::cout << xTotal << std::endl;

    return 0;
}