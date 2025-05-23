#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <limits>
#include <any>
#include <iomanip>

unsigned long long kernel_seed;
unsigned int rolls = 0;

unsigned long long get_seed_from_urandom() {
    unsigned long long random_value = 0;
    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
    if (urandom) {
        urandom.read(reinterpret_cast<char*>(&random_value), sizeof(random_value));
        if (!urandom) {
             std::cerr << "Warning: Failed to read " << sizeof(random_value) << " bytes from /dev/urandom. Seeding might be weak.\n";
        }
        urandom.close();
    } else {
        std::cerr << "Warning: Failed to open /dev/urandom. Seeding might be weak or predictable.\n";
    }
    return random_value;
}

template<typename T>
bool validated_input(T& value) {
    std::cout << "> ";
    if (!(std::cin >> value)) {
        std::cerr << "\nError: Invalid input type., please enter an integer. \n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

std::any lobby(std::mt19937_64& rng) {
    int choice;

    std::cout << "\n\n\n";
    std::cout << "Seeding PRNG with value: " << kernel_seed << std::endl;
    rolls = rolls + 1;
    std::cout << "total roll count : " << rolls << std::endl;

    std::cout << "\n--- Randomization Menu ---\n";
    std::cout << "1 : Coinflip (1 or 2)\n";
    std::cout << "2 : Range Randomization (1 to Max)\n";
    std::cout << "3 : Password Generator\n";
    std::cout << "4 : Percentage Chance Roll\n";
    std::cout << "5 : Exit Program\n";
    std::cout << "--------------------------\n";

    if (!validated_input(choice)) {
        return std::any();
    }

    switch (choice) {
        case 1: {
            std::bernoulli_distribution bernoulli_dist;
            bool random_bool_value = bernoulli_dist(rng);
            return random_bool_value;
        }
        case 2: {
            int max_val;
            std::cout << "Enter Max number (positive integer): ";
            if (!validated_input(max_val) || max_val <= 0) {
                 if (max_val <= 0 && !std::cin.fail()) {
                     std::cerr << "Error: Max number must be positive.\n";
                 }
                return NULL;
            }

            std::uniform_int_distribution<int> dist(1, max_val);
            return dist(rng);
        }
        case 3: {
            int length;
            std::cout << "Enter length of password (positive integer): ";
             if (!validated_input(length) || length <= 0) {
                if (length <= 0 && !std::cin.fail()) {
                    std::cerr << "Error: Password length must be positive.\n";
                }
                return NULL;
             }

            const char ctable[] = {
                'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
                'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                '0','1','2','3','4','5','6','7','8','9',
                '!','@','#','$','%','^','&','*','(',')','-','_','+','=','{','}','[',']','|',';',':',',','.','<','>','/','?','~'
            };
            const int char_count = sizeof(ctable) / sizeof(ctable[0]);

            std::uniform_int_distribution<int> dist(0, char_count - 1);

            std::string password = "";
            password.reserve(length);

            for (int i = 0; i < length; ++i) {
                password += ctable[dist(rng)];
            }
            return password;
        }
        case 4: {
            double percentage;
            std::cout << "Enter desired success percentage (0.0 to 100.0): ";
            if (!validated_input(percentage) || percentage < 0.0 || percentage > 100.0) {
                if ((percentage < 0.0 || percentage > 100.0) && !std::cin.fail()) {
                    std::cerr << "Error: Percentage must be between 0.0 and 100.0.\n";
                }
                return NULL;
            }

            std::uniform_real_distribution<double> dist(0.0, 100.0);
            double roll_result = dist(rng);

            if (roll_result < percentage) {
                std::cout << std::fixed << std::setprecision(2);

                std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
                std::cout << "Rolled " << roll_result << " - Success! (" << percentage << "% chance)" << std::endl;
                std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";

                return roll_result;
            } else {
                std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
                std::cout << std::fixed << std::setprecision(2);
                std::cout << "Rolled " << roll_result << " - Failure! (" << percentage << "% chance)" << std::endl;
                std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";

                return roll_result;
            }
            break;
        }
        case 5:
            std::cout << "Exiting program.\n";
            return std::string("exit");
        default:
            std::cerr << "Error: Incorrect menu choice.\n";
            return std::string("loop-back-to-lobby");
    }
}

int main() {
    kernel_seed = get_seed_from_urandom();
    std::mt19937_64 random_engine(kernel_seed);
    std::any result;

    bool running = true;
    while (running) {
        result = lobby(random_engine);

        if (result.type() == typeid(std::string)) {
            std::string str_result = std::any_cast<std::string>(result);
            if (str_result == "loop-back-to-lobby") {
                continue;
            }
            else if (str_result == "exit") {
                running = false;
                std::cout << "Exiting..." << std::endl;
            }
        }

        else if (result.type() == typeid(bool)) {
            bool coin_result = std::any_cast<bool>(result);
            std::cout << "Coinflip result: " << (coin_result ? "2" : "1") << std::endl;
        }
        else if (result.type() == typeid(int)) {
            int range_result = std::any_cast<int>(result);
            if (range_result == 0) { // catch NULL returns from lobby()
                running = false;
                std::cout << "Exiting..." << std::endl;
            } else {
                std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
                std::cout << "Random number result: " << range_result << std::endl;
                std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
            }
        }
        else if (result.type() == typeid(std::string)) {
            std::string str_result = std::any_cast<std::string>(result);
            std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
            std::cout << "Generated password: " << str_result << std::endl;
            std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
        }
        else if (result.type() == typeid(double)) {
            double chance_result = std::any_cast<double>(result);
        }

        if (result.has_value() && result.type() != typeid(int)) {
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }
    }

    return 0;
}
