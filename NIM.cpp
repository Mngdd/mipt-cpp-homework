#include <bitset>
#include <iostream>
#include <vector>
#include <string>


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


int main() {
    std::vector<int> stacks = {13, 1};
    int player_id = 1;  // 0 = игрок, иначе BOT (тк всегда %2 делаем)

    print_stacks(stacks);
    while (get_sum(stacks) != 0) {
        player_id = (player_id + 1) % 2; // чередуем ходы игроков
        size_t index = 204863; // номер кучки из которой взяли, пока тут мусор
        int amount = -1; // скок взяли из кучки, пока тут мусор

        if ((player_id % 2) != 0) { // ход машыны
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
                    }
                }
            }
        } else { // ход гей мера 220 220
            //TODO: ОБРАБОТКА ГОВНО-ВВОДА И CTRL+Z
            // пока что верим вводу игрока
            std::cout << ">>enter the stack number and amount of stones: ";
            std::cin >> index >> amount;
            stacks[index-1] -= amount;
            //todo: ГОВНО ВВОД ГОВНО ВВОД ГОВНО ВВОД ГОВНО ВВОД
        }
        
        if (player_id == 0) {
            std::cout << ">>Player ";
        } else {
            std::cout << ">>Bot ";
        }
        std::cout << "took " << amount << " stones from stack " << index << std::endl;
        print_stacks(stacks);

    }
    
    // конец игры, результаты
    if (player_id % 2 != 0) {
        std::cout << "\033[92mYou win!\n\033[0m";
    } else {
        std::cout << "\033[91mBot wins!\n\033[0m";
    }
    std::cout << "PRESS ENTER KEY TO EXIT\n";
    std::cin.get();
}
