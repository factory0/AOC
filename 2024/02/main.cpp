#include <stdint.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Level {
   public:
    std::vector<int> data;
    bool valid;
    bool validAfterRemove = false;
    int levelToRemove = -1;
    void parse(std::string line) {
        int start = 0;
        int end = line.find(" ");
        while (end != std::string::npos) {
            // std::cout << line.substr(start, end - start) << std::endl;
            data.push_back(std::stoi(line.substr(start, end - start)));
            start = end + 1;
            end = line.find(" ", start);
        }
        data.push_back(std::stoi(line.substr(start, line.size() - start)));
        return;
    }

    bool isValid() {
        bool decreasing = false;
        bool valid = true;
        bool first = true;
        for (int i = 0; i < data.size() - 1; i++) {
            if (data[i] > data[i + 1]) {
                if (first) {
                    first = false;
                    decreasing = false;
                } else if (decreasing) {
                    valid = false;
                    this->valid = false;
                    return false;
                }
            } else {
                if (first) {
                    first = false;
                    decreasing = true;
                } else if (!decreasing) {
                    valid = false;
                    this->valid = false;
                    return false;
                }
            }
            if (!(std::abs(data[i] - data[i + 1]) > 0 &&
                  std::abs(data[i] - data[i + 1]) < 4)) {
                valid = false;
                this->valid = false;
                return false;
            }
        }
        this->valid = valid;
        if (valid) {
            for (int j = 0; j < data.size(); j++) {
                std::cout << data[j] << " ";
            }
            std::cout << std::endl;
        }
        return valid;
    }

    bool tryToValidate() {
        for (int i = 0; i < this->data.size(); i++) {
            Level* level = new Level();
            for (int j = 0; j < this->data.size(); j++) {
                if (i != j) {
                    level->data.push_back(this->data[j]);
                }
            }
            if (level->isValid()) {
                this->validAfterRemove = true;
                this->levelToRemove = i;
                return true;
            }
        }
        return false;
    }
};

bool comp(int a, int b) { return a <= b; }

//__int128 v1 = 155405054545005454121545545445;
//__int128 v2 = -45804404545454541545554787;

void print(__int128 x) {
    if (x < 0) {
        putchar('-');
        x = -x;
    }
    if (x > 9) print(x / 10);
    putchar(x % 10 + '0');
}

int main() {
    std::ifstream file("list.txt");  // Open the file
    if (!file.is_open()) {
        std::cerr << "Failed to open the file.\n";
        return 1;
    }

    std::string line;
    std::vector<Level*>* levels = new std::vector<Level*>();
    Level* level;
    while (std::getline(file, line)) {  // Read the file line by line
        // std::cout << "Line: " << line << "\n";
        // std::cout << "Line size: " << line.size() << "\n";
        level = new Level();
        level->parse(line);
        levels->push_back(level);
    }

    int valid = 0;
    for (int i = 0; i < levels->size(); i++) {
        if (levels->at(i)->isValid()) {
            valid++;
        }
    }
    std::cout << valid << std::endl;

    for (int i = 0; i < levels->size(); i++) {
        if (!levels->at(i)->valid) {
            levels->at(i)->tryToValidate();
            if (levels->at(i)->validAfterRemove) {
                for (int k = 0; k < levels->at(i)->data.size(); k++) {
                    std::cout << levels->at(i)->data[k] << " ";
                }
                std::cout << std::endl;
                std::cout << levels->at(i)->levelToRemove << std::endl;
            }
        }
    }

    int validAfter = 0;
    for (int i = 0; i < levels->size(); i++) {
        if (levels->at(i)->validAfterRemove || levels->at(i)->valid) {
            validAfter++;
        }
    }

    std::cout << validAfter << std::endl;

    file.close();
    return 0;
}