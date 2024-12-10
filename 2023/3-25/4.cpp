#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Card {
   public:
    int number;
    std::vector<int> winners;
    std::vector<int> all;
    int copies = 1;

    Card() { number = 0; }

    int win() {
        int win = 0;
        for (int i = 0; i < all.size(); i++) {
            for (int j = 0; j < winners.size(); j++) {
                if (all[i] == winners[j]) {
                    win++;
                }
            }
        }
        return win;
    }

    int score() {
        int sum = 0;
        for (int i = 0; i < all.size(); i++) {
            for (int j = 0; j < winners.size(); j++) {
                if (all[i] == winners[j]) {
                    if (sum == 0) {
                        sum = 1;
                    } else {
                        sum *= 2;
                    }
                }
            }
        }
        // std::cout << sum << std::endl;
        return sum;
    }

    void parse(std::string line) {
        bool foundNum = false;
        bool foundmid = false;

        int idx1;
        int idx2;
        bool started;
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ':') {
                foundNum = true;
                number = std::stoi(line.substr(idx1, i));
                started = false;
                continue;
            }
            if (line[i] == '|') {
                foundmid = true;
                continue;
            }
            if (line[i] >= '0' && line[i] <= '9') {
                if (!started) {
                    started = true;
                    idx1 = i;
                }
            } else {
                if (started) {
                    idx2 = i;
                    started = false;
                    int tmp = std::stoi(line.substr(idx1, idx2 - idx1));
                    if (foundmid) {
                        all.push_back(tmp);
                    } else {
                        winners.push_back(tmp);
                    }
                }
            }
        }
        int tmp = std::stoi(line.substr(idx1, line.length() - idx1));
        all.push_back(tmp);
        
        std::cout << number << std::endl;
        for(int i = 0; i < winners.size(); i++){
            std::cout << winners[i] << " ";
        }
        std::cout << std::endl;
        for(int i = 0; i < all.size(); i++){
            std::cout << all[i] << " ";
        }
        std::cout << std::endl;
        
    }
};

int main() {
    std::ifstream file("4.txt");
    // std::cout << "hello" << std::endl;
    std::string line;
    long long total = 0;
    long Xtotal = 0;
    std::vector<Card*> cards;

    while (std::getline(file, line)) {
        Card* card = new Card();
        card->parse(line);
        cards.push_back(card);
    }

    for (int i = 0; i < cards.size(); i++) {
        total += cards[i]->score();
    }

    for (int i = 0; i < cards.size(); i++) {
        for (int p = 0; p < cards[i]->copies; p++) {
            int w = cards[i]->win();
            for (int j = i + 1; j < cards.size(); j++) {
                if (w <= 0) {
                    break;
                }
                cards[j]->copies++;
                /* Card* tmp = new Card();
                tmp->winners = cards[j]->winners;
                tmp->all = cards[j]->all;
                tmp->number = cards[j]->number;
                cards.insert(cards.begin() + j + 1, tmp); */
                w--;
            }
        }
    }

    for(int i = 0; i < cards.size(); i++){
        std::cout << cards[i]->copies << std::endl;
        Xtotal += cards[i]->copies;
    }

    std::cout << total << std::endl;
    std::cout << Xtotal << std::endl;

    return 0;
}