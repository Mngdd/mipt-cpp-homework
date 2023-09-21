#include <bitset>
#include <iostream>
#include <vector>
#include <string>
#include <random>


// получить XOR сумму кол-ва камней от каждой кучки
int get_xor(const std::vector<int>& lst) {
    int x = lst[0];
    for (size_t i = 1; i < lst.size(); ++i) {
        x ^= lst[i];
    }
    return x;
}


// просто общее кол-во камней
int get_sum(const std::vector<int>& lst) {
    int x = 0;
    for (int i : lst) {
        x += i;
    }
    return x;
}


// ищем первую попавшуюся цифру 1
size_t find_first_one(std::bitset<16> num) {
    size_t d = 0; // индекс
    std::string s_num = num.to_string();
    for (char c : s_num) {
        if (c == '1') { break; } // нашли, уходим
        ++d;
    }
    return d;
}


void print_stacks(std::vector<int>& s) {
    for (size_t i = 0; i < s.size(); ++i) {
        std::cout << (i + 1) << ")";
        for (int stone = 0; stone < s[i]; ++stone) {
            std::cout << " #";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}


int rand_int(int a, int b) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(a,b); // distribution in range [1, 6]

    return dist(rng);
}


int correct_input(std::string hi, std::string err, std::string tip, bool upper = false, int up_bound=-1, int low = 0) {
    bool not_first = false;
    std::cout << hi;
    int s_a = -1;
    do {
        std::string S_mode = "";
        s_a = 0; // обнуляем всё и перезаписываем
                
        if (std::cin.fail()) { // если с прошл цикла фейл, чиним
            std::cin.clear();
        }
        
        if (not_first) { // если true - уже ошиблись, можем писать
            std::cout << err;
        } else {
            not_first = true;
        }

        std::cout << tip; std::cin >> S_mode; // 1ая перем
        try {
            s_a = std::stoi(S_mode);  // если не число, улетаем
        } catch (...) {
            std::cout << "\n";
        }
    } while (std::cin.fail() || s_a <= low || (s_a > up_bound && upper));
    return s_a;
}


int main() {
    int player_id = 1;  // 0 = игрок, иначе BOT (тк всегда %2 делаем), но тк меняем очередь на 1м ходе, то тут наоборот

    int mode = correct_input("choose mode:\n1-Player starts first\n2-Bot "
                             "starts first\n3-Random choice\n", "\033[91mwrong input\033[0m, "
                                                                "possible values -> \033[92m[1, 3]\033[0m\n",
                                                      "\033[96m>>\033[0mEnter the mode number\n", true,
                                                      3);
    switch (mode) {
        case 1:
            player_id = 0;
            break;
        case 0:
            player_id = 1;
            break;
        case 3: player_id = rand_int(1, 2);
    }
    int s_a = correct_input("choose stacks amount\n", "\033[91mwrong input\033[0m, possible values -> "
                                                      "\033[92mmore than 1\033[0m\n",
                                                      "\033[96m>>\033[0mEnter stacks amount\n", false,
                                                      1, 1);
    int mx = correct_input("choose max value in stack\n", "\033[91mwrong input\033[0m, possible values -> "
                                                      "\033[92mmore than 0\033[0m\n",
                                                      "\033[96m>>\033[0mEnter max value\n", false);
    std::vector<int> stacks;
    for (int i = 0; i != s_a; ++i) {
        stacks.push_back(rand_int(1, mx));
    }
    
    print_stacks(stacks);
    while (get_sum(stacks) != 0) {
        player_id = (player_id + 1) % 2; // чередуем ходы игроков
        size_t index = 204863; // номер кучки из которой взяли, пока тут мусор
        int amount = -1; // скок взяли из кучки, пока тут мусор

        if (player_id != 0) { // ход машыны
            std::cout << "\033[91m>>\033[0mBot's move...\n";
            int s = get_xor(stacks); // XOR сум всех элементов
            size_t s_first = find_first_one(std::bitset<16>(s)); // двоичное представление S
            
            if (s != 0) { // победное положение
                for (size_t i = 0; i != stacks.size(); ++i) {
                    if (std::bitset<16>(stacks[i]).to_string()[s_first] == '1') {
                        // нашли элем, при котором S ^ [элем] = 0
                        // такой элем обязательно есть.
                        index = i+1; amount = stacks[i] - (stacks[i] ^ s);
                        stacks[i] = stacks[i] ^ s;
                        break;
                    }
                }
            } else { // пока что проигрываем
                for (size_t i = 0; i != stacks.size(); ++i) {
                    if (stacks[i] > 0) {
                        --stacks[i];
                        index = i+1; amount = 1;
                        break;
                    }
                }
            }
        } else { // ход гей мера
            bool not_first = false;  // вводим не первый раз?
            do {
                std::string S_index = "", S_amount = "";
                index = 0; amount = 0; // обнуляем всё и перезаписываем
                
                if (std::cin.fail()) { // если с прошл цикла фейл, чиним
                    std::cin.clear();
                }

                if (not_first) { // если true - уже ошиблись, можем писать
                    std::cout << "\033[91mwrong input\033[0m, possible values -> number: \033[92m[1, " << stacks.size() <<
                       "]\033[0m, and amount: \033[92m[1, all stones]\033[0m\n";
                } else {
                    not_first = true;
                }

                if (std::cin.fail()) { // если 1й ввод провальный
                    continue;
                }

                std::cout << "\033[96m>>\033[0mEnter the stack number\n"; std::cin >> S_index; // 1ая перем
                try {
                    index = std::stoi(S_index);  // если не число, улетаем
                } catch (...) {
                    std::cout << "\n";
                    continue;
                }
                
                std::cout << "\033[96m>>\033[0mAnd amount of stones\n"; std::cin >> S_amount;
                std::cout << "\n";
                try {
                    amount = std::stoi(S_amount);
                } catch (...) {
                    // а ничего и не надо
                }
            } while (std::cin.fail() || index <= 0 || index > stacks.size() || amount <= 0 || amount > stacks[index-1]);
            
            stacks[index-1] -= amount;
        }

        if (player_id == 0) {
            std::cout << ">>\033[93mPlayer\033[0m ";
        } else {
            std::cout << ">>\033[93mBot\033[0m ";
        }
        std::cout << "took " << amount << " stones from stack " << index << std::endl;
        print_stacks(stacks);

    }
    
    // конец игры, результаты
    if (player_id % 2 == 0) {
        std::cout << "\033[92mYou win!\n\033[0m";
    } else {
        std::cout << "\033[91mBot wins!\n\033[0m";
    }
    std::cout << "PRESS ENTER KEY TO EXIT\n";
    std::cin.get();
}
