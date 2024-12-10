#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool iscorn(char c) {
    if ((c < '0' || c > '9') && c != '.') {
        return true;
    }
    return false;
}

class Part {
   public:
    int x;
    int y;
    int number;
    int length;
    bool active;

    Part() {
        x = 0;
        y = 0;
        number = 0;
        length = 0;
        active = false;
    }

    void evaluate(std::vector<std::string>* map) {
        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;
        if (x > 0) {
            left = true;
        }
        if (y > 0) {
            up = true;
        }
        if (x + length < map->at(y).length() - 2) {
            right = true;
        }
        if (y < map->size() - 2) {
            down = true;
        }
        if (up) {
            for (int i = 0; i < length; i++) {
                if (iscorn(map->at(y - 1)[x + i])) {
                    active = true;
                    return;
                }
            }
        }
        if (down) {
            for (int i = 0; i < length; i++) {
                if (iscorn(map->at(y + 1)[x + i])) {
                    active = true;
                    return;
                }
            }
        }
        if (left) {
            if (iscorn(map->at(y)[x - 1])) {
                active = true;
                return;
            }
            if (up) {
                if (iscorn(map->at(y - 1)[x - 1])) {
                    active = true;
                    return;
                }
            }
            if (down) {
                if (iscorn(map->at(y + 1)[x - 1])) {
                    active = true;
                    return;
                }
            }
        }
        if (right) {
            if (iscorn(map->at(y)[x + length])) {
                active = true;
                return;
            }
            if (up) {
                if (iscorn(map->at(y - 1)[x + length])) {
                    active = true;
                    return;
                }
            }
            if (down) {
                if (iscorn(map->at(y + 1)[x + length])) {
                    active = true;
                    return;
                }
            }
        }
    }

    bool nextto(int x, int y) {
        if (y >= this->y - 1 && y <= this->y + 1 && x >= this->x - 1 &&
            x <= this->x + this->length) {
            return true;
        }
        return false;
    }
};

void parseline(std::string line, std::vector<Part*>* parts, int y) {
    bool foundbeg = false;
    int start = 0;
    for (int i = 0; i < line.length(); i++) {
        if (line[i] >= '0' && line[i] <= '9') {
            if (!foundbeg) {
                start = i;
                foundbeg = true;
            }
            continue;
        } else {
            if (foundbeg) {
                int length = i - start;
                Part* part = new Part();
                part->x = start;
                part->y = y;
                part->number = std::stoi(line.substr(start, length));
                part->length = length;
                parts->push_back(part);
                foundbeg = false;
            }
        }
    }
    if (foundbeg) {
        int length = line.length() - start;
        Part* part = new Part();
        part->x = start;
        part->y = y;
        part->number = std::stoi(line.substr(start, length));
        part->length = length;
        parts->push_back(part);
    }
}

int main() {
    std::ifstream file("3.txt");
    // std::cout << "hello" << std::endl;
    std::string line;
    long long total = 0;
    long Xtotal = 0;
    std::vector<std::string> map;
    std::vector<Part*>* parts = new std::vector<Part*>();

    while (std::getline(file, line)) {
        map.push_back(line);
    }

    for (int i = 0; i < map.size(); i++) {
        parseline(map[i], parts, i);
    }

    for (int i = 0; i < parts->size(); i++) {
        parts->at(i)->evaluate(&map);
        if (parts->at(i)->active) {
            total += parts->at(i)->number;
        }
    }

    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[i].length(); j++) {
            if (map[i][j] == '*') {
                Part* first = nullptr;
                Part* second = nullptr;
                bool foundthird = false;
                for (int k = 0; k < parts->size(); k++) {
                    if (parts->at(k)->nextto(j, i)) {
                        if (first == nullptr) {
                            first = parts->at(k);
                        } else if (second == nullptr) {
                            if(first != parts->at(k)) {
                                second = parts->at(k);    
                            }
                        } else {
                            foundthird = true;
                            break;
                        }
                    }
                }
                if(!foundthird && first != nullptr && second != nullptr) {
                    Xtotal += first->number * (second->number);    
                }
                
            }
        }
    }

    std::cout << total << std::endl;
    std::cout << Xtotal << std::endl;

    return 0;
}