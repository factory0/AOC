#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

std::vector<int> firstList;
std::vector<int> secondList;

long sum = 0;

unsigned long long similarity = 0;

bool comp(int a, int b) {
    return a <= b;
}

int main() {
    std::ifstream file("example.txt"); // Open the file
    if (!file.is_open()) {
        std::cerr << "Failed to open the file.\n";
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) { // Read the file line by line
        //std::cout << "Line: " << line << "\n";
        //std::cout << "Line size: " << line.size() << "\n";

        firstList.push_back(std::stoi(line.substr(0, 5)));
        secondList.push_back(std::stoi(line.substr(8, 5)));
    }

    std::sort(firstList.begin(), firstList.end(), comp);
    std::sort(secondList.begin(), secondList.end(), comp);

    std::cout << firstList[0] << "\n";
    std::cout << secondList[0] << "\n";

    for(int i = 0; i < firstList.size(); i++) {
        sum += std::abs(firstList[i] - secondList[i]);
    }

    for(int i = 0; i < firstList.size(); i++) {
        int occurences = 0;
        for(int j = 0; j < secondList.size(); j++) {
            if(firstList[i] == secondList[j]) {
                occurences++;
            }
            if(firstList[i] < secondList[j]) {
                break;
            }
        }
        similarity += occurences*firstList[i];
    }

    std::cout << sum << "\n";
    std::cout << similarity << "\n";

    file.close();
    return 0;
}