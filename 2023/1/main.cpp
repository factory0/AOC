#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> numb = {"one", "two",   "three", "four", "five",
                                 "six", "seven", "eight", "nine"};

int main() {
    std::ifstream file("input.txt");
    std::string line;
    int total = 0;
    while (std::getline(file, line)) {
        int firstidx = -1;
        int lastidx = -1;
        bool firstnum = false;
        int firstchar = -1;
        bool lastnum = false;
        int lastchar = -1;
        for (int i = 0; i < line.length(); i++) {
            if (line[i] >= '0' && line[i] <= '9') {
                firstidx = i;
                break;
            }
        }
        for (int i = line.length() - 1; i >= 0; i--) {
            if (line[i] >= '0' && line[i] <= '9') {
                lastidx = i;
                break;
            }
        }

        for (int i = 0; i < numb.size(); i++) {
            int pos = line.find(numb[i]);
            if (pos != std::string::npos) {
                if (pos < firstidx || firstidx == -1) {
                    if (!firstnum) {
                        firstnum = true;
                        firstchar = i;
                    } else {
                        firstchar = i;
                    }
                    firstidx = pos;
                }
            }
        }

        for (int i = 0; i < numb.size(); i++) {
            int pos = -1;
            while (true) {
                int tmp = line.find(numb[i], pos + 1);
                if (tmp == std::string::npos) {
                    break;
                }
                pos = tmp;
            }
            if (pos > lastidx || lastidx == -1) {
                if (!lastnum) {
                    lastnum = true;
                    lastchar = i;
                } else {
                    lastchar = i;
                }
                lastidx = pos;
            }
        }

        int tmp = 0;
        if (firstnum) {
            tmp = (firstchar + 1) * 10;
        } else {
            tmp = std::stoi(line.substr(firstidx, 1)) * 10;
        }

        if (lastnum) {
            tmp += lastchar + 1;
        } else {
            tmp += std::stoi(line.substr(lastidx, 1));
        }

        total += tmp;
    }

    std::cout << total << std::endl;

    return 0;
}