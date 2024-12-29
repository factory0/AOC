#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

class Towel {
   public:
    std::string pattern;

    std::size_t idx;
    std::size_t length;

    Towel(std::string pattern) {
        this->pattern = pattern;
        this->idx = std::string::npos;
        this->length = pattern.length();
    }
};

class Onsen {
   public:
    std::vector<Towel*> towels;
    std::map<std::size_t, unsigned long long> indices;
    unsigned long long options;

    void parseTowels(std::string line) {
        int pos = line.find(",");
        int start = 0;
        while (pos != std::string::npos) {
            std::string towel = line.substr(start, pos - start);
            towels.push_back(new Towel(towel));
            start = pos + 2;
            pos = line.find(",", start);
        }
        towels.push_back(new Towel(line.substr(start)));
        std::sort(towels.begin(), towels.end(),
                  [](Towel* a, Towel* b) { return a->length > b->length; });
    }

    bool checkPattern(std::string pattern, int pos) {
         if (indices.find(pos) != indices.end()) {
            if(indices[pos] == 0) {return false;}
            else {return true;}
            /* indices[pos] *= 2;
            return false; */
        } 
        // std::cout << "pattern: " << pattern << " pos: " << pos << std::endl;
        for (int i = 0; i < towels.size(); i++) {
            if (towels[i]->idx == std::string::npos) continue;
            // std::cout << "meow: " << towels[i]->idx << std::endl;
            if (towels[i]->idx > pos) continue;

            std::size_t tmp = towels[i]->idx;
            if (pos != 0) {
                tmp = pattern.find(towels[i]->pattern, pos);
            }
            // std::cout << "idx: " << towels[i]->idx << " pos: " << tmp << "
            // pattern: " << towels[i]->pattern << std::endl;
            if (tmp != pos) continue;
            // std::cout << "found " << towels[i]->pattern << std::endl;
            if (pattern.length() == towels[i]->pattern.length() + pos) {
                if (indices.find(pos) == indices.end()) {
                    indices[pos] = 1;
                } else {
                    indices[pos] += 1;
                }
            }

            bool check =
                checkPattern(pattern, pos + towels[i]->pattern.length());
                indices[pos] += indices[pos + towels[i]->pattern.length()];
             if (check) {
                //indices[pos] += indices[pos + towels[i]->pattern.length()];
                //return true;
            } 
        }
        if (indices.find(pos) == indices.end()) {
            indices[pos] = 0;
        }
        return false;
    }

    void printTowels() {
        for (int i = 0; i < towels.size(); i++) {
            std::cout << towels[i]->pattern << " " << towels[i]->idx
                      << std::endl;
        }
    }

    bool checkWrapper(std::string pattern) {
        indices.clear();
        options = 0;
        for (int i = 0; i < towels.size(); i++) {
            towels[i]->idx = pattern.find(towels[i]->pattern);
        }
        // printTowels();
        bool check = checkPattern(pattern, 0);
        /* for (auto it = indices.begin(); it != indices.end(); it++) {
            options += it->second;
            std::cout << it->second << " ";
        }
        std::cout << std::endl; */
        options = indices[0];
        
        return options > 0;
    }
};

int main() {
    std::ifstream file("input.txt");
    // std::cout << "hello" << std::endl;
    std::string line;
    unsigned long long total = 0;
    unsigned long long xTotal = 0;
    std::vector<std::string> lines;

    Onsen onsen;

    while (std::getline(file, line)) {
        if (line == "") break;
        onsen.parseTowels(line);
    }

    onsen.printTowels();

    while (std::getline(file, line)) {
        if (onsen.checkWrapper(line)) total++;
        xTotal += onsen.options;
        std::cout << "checked " << line << " " << total << " " << onsen.options
                  << std::endl;
    }

    // onsen.printTowels();

    std::cout << total << std::endl;
    std::cout << xTotal << std::endl;
    // 107468 for my input
    return 0;
}