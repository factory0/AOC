#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

void swap(std::vector<int>* v, int i, int j) {
    int temp = v->at(i);
    v->at(i) = v->at(j);
    v->at(j) = temp;
}

class Rule {
   public:
    int first;
    int second;
};

class Update {
   public:
    std::vector<int> numbers;
    std::vector<Rule*> rules;
    bool good;
    bool shuffled;
    Update() {
        good = true;
        shuffled = false;
    }

    int retMid() { return numbers[numbers.size() / 2]; }

    bool check(Rule* r) {
        int pos1 = -1;
        int pos2 = -1;
        for (int i = 0; i < numbers.size(); i++) {
            if (numbers[i] == r->first) {
                pos1 = i;
            }
            if (numbers[i] == r->second) {
                pos2 = i;
            }
        }
        if (pos1 != -1 && pos2 != -1) {
            rules.push_back(r);
            if (pos1 > pos2) {
                good = false;
                
            }
        }
        return good;
    }

    bool checkandswap(Rule* r) {
        int pos1 = -1;
        int pos2 = -1;
        for (int i = 0; i < numbers.size(); i++) {
            if (numbers[i] == r->first) {
                pos1 = i;
            }
            if (numbers[i] == r->second) {
                pos2 = i;
            }
        }
        if (pos1 != -1 && pos2 != -1) {
            if (pos1 > pos2) {
                good = false;
                //std::cout << "swap" << std::endl;
                swap(&numbers, pos1, pos2);
            }
        }
        return good;
    }

    void fix() {
        bool fixing = true;
        while (fixing) {
            good = true;
            for (int i = 0; i < rules.size(); i++) {
                if (!checkandswap(rules[i])) {
                    break;
                }
            }
            if (good) {
                fixing = false;
            }
            // std::cout << "found solution" << std::endl;
        }
    }
};

Update* parseUpdate(std::string update) {
    Update* u = new Update();
    int start = 0;
    int pos = update.find(',');
    while (pos != std::string::npos) {
        u->numbers.push_back(atoi(update.substr(start, pos - start).c_str()));
        start = pos + 1;
        pos = update.find(',', start);
    }
    u->numbers.push_back(
        atoi(update.substr(start, update.length() - start).c_str()));
    /*for(int i = 0; i < u->numbers.size(); i++){
        std::cout << u->numbers[i] << " ";
    }
    std::cout << std::endl;*/
    // std::cout << u->retMid() << std::endl;
    return u;
}

Rule* parseRule(std::string rule) {
    Rule* r = new Rule;
    r->first = atoi(rule.substr(0, rule.find('|')).c_str());
    r->second = atoi(rule.substr(rule.find('|') + 1, rule.length()).c_str());
    // std::cout << r->first << " " << r->second << std::endl;
    return r;
}

int main() {
    std::ifstream file("input.txt");
    // std::cout << "hello" << std::endl;
    std::string line;
    long long total = 0;
    long xtotal = 0;
    std::vector<std::string> lines;
    std::vector<Rule*> rules;
    std::vector<Update*> updates;
    while (std::getline(file, line)) {
        if (line == "") {
            break;
        }
        rules.push_back(parseRule(line));
    }

    while (std::getline(file, line)) {
        updates.push_back(parseUpdate(line));
    }

    for (int i = 0; i < updates.size(); i++) {
        for (int j = 0; j < rules.size(); j++) {
            updates[i]->check(rules[j]);
        }
        if (updates[i]->good) {
            total += updates[i]->retMid();
        }
    }

    for (int i = 0; i < updates.size(); i++) {
        if (updates[i]->good) {
            continue;
        }
        std::cout << "doing idx " << i << std::endl;
        updates[i]->fix();
        if (updates[i]->good) {
            // updates[i]->numbers = nums;
            std::cout << "Found a solution" << std::endl;
            xtotal += updates[i]->retMid();
        } else {
            std::cout << "Couldnt find a solution" << std::endl;
        }
    }

    std::cout << total << std::endl;
    std::cout << xtotal << std::endl;

    return 0;
}