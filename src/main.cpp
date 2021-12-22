﻿// main.cpp

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cstdlib>
#include <stdexcept>
#include <cmath>
#include <ctime>
#include <cstddef>

#include <HashMap.hpp>


// Returns an integer number from the stdin
int get_int();
// Returns a float number from the stdin
float get_float();
// Returns a string from the stdin
std::string get_string();
// Returns a random string
std::string get_rand_string(size_t start_len = 1, size_t end_len = 7);


int main()
{
    constexpr unsigned short MAX_SIZE = 1024; // Number of maximum items in map

    HashMap<std::string, int> map;  // Using hash map
    size_t size = 0;                // Size of hash map
    float max_load_factor = 0.0f;   // Maximum load factor of hash map
    int menu_item = 0;                  

    std::cout << "Testing program of hash map.\n\n";

    std::cout << "Select the type of container initializing:" << std::endl;
    std::cout << "\t1: Empty container." << std::endl;
    std::cout << "\t2: Container with random values." << std::endl;
    menu_item = get_int();
    while (menu_item < 1 || menu_item > 2)
    {
        std::cout << "Error: this menu item does not exist." << std::endl;
        menu_item = get_int();
    }
    switch (menu_item)
    {
    case 1: break;
    case 2:
    {
        srand(static_cast<unsigned int>(time(nullptr)));

        std::cout << "Enter the number of items:" << std::endl;
        size = get_int();
        while (size < 0 || size > MAX_SIZE)
        {
            std::cout << "Error: the entered size is less than 0 or greater "
                "than " << MAX_SIZE << "." << std::endl;
            size = get_int();
        }
        for (size_t i = 0; i < size; i++)
            map.insert(std::make_pair(get_rand_string(), rand()));

        break;
    }
    }
    std::cout << "Hash map created.\n\n";

    bool isExit = false;
    while (!isExit)
    {
        std::cout << "Program menu:" << std::endl;
        std::cout << "\t1: Get the number of elements.\n";
        std::cout << "\t2: Get the number of buckets.\n";
        std::cout << "\t3: Get the current loading factor.\n";
        std::cout << "\t4: Get the maximum loading factor.\n";
        std::cout << "\t5: Set the maximum loading factor.\n";
        std::cout << "\t6: Get an element by key.\n";
        std::cout << "\t7: Set an element by key to new value.\n";
        std::cout << "\t8: Checking the hash map for emptiness.\n";
        std::cout << "\t9: Check the map item's accessories.\n";
        std::cout << "\t10: Clear the container.\n";
        std::cout << "\t11: Add an element to the hash map.\n";
        std::cout << "\t12: Remove an element from the hash map.\n";
        std::cout << "\t13: Set the number of buckets.\n";
        std::cout << "\t14: Set new capacity.\n";
        std::cout << "\t15: Print the hash map.\n";
        std::cout << "\t16: Go to the control section \"Iterator\".\n";
        std::cout << "\t17: Exit the program.\n";

        std::cout << "Select a menu item:\n";
        menu_item = get_int();
        while (menu_item < 1 || menu_item > 17)
        {
            std::cout << "Error: this menu item does not exist." << std::endl;
            menu_item = get_int();
        }
        std::cout << std::endl;

        switch (menu_item)
        {
        case 1:
        {
            std::cout << "Number of elements: " << map.size() << ".\n";
            break;
        }

        case 2:
        {
            std::cout << "Number of buckets: " << map.buckets_count()
                << ".\n";
            break;
        }

        case 3:
        {
            std::cout << "Current loading factor: " << map.load_factor()
                << ".\n";
            break;
        }

        case 4:
        {
            std::cout << "Maximum loading factor: " << map.max_load_factor()
                << ".\n";
            break;
        }

        case 5:
        {
            std::cout << "Enter the maximum hash table load factor:\n";
            max_load_factor = get_float();
            while (max_load_factor < 1.0f || max_load_factor > 10.0f)
            {
                std::cout << "Error: this value must not be less than 1 or "
                    "more than 10." << std::endl;
                max_load_factor = get_float();
            }
            map.max_load_factor(max_load_factor);

            std::cout << "Maximum loading factor was updated.\n";
            break;
        }

        case 6:
        {
            std::cout << "Enter the key:\n";
            std::string key = get_string();

            try
            {
                int value = map.at(key);
                std::cout << "Element with the that key: " << value << ".\n";
            }
            catch (const std::exception& e)
            {
                std::cout << "Error: " << e.what() << ".\n";
            }

            break;
        }

        case 7:
        {
            std::cout << "Enter the key:\n";
            std::string key = get_string();
            std::cout << "Enter the value:\n";
            int value = get_int();

            try
            {
                map.at(key) = value;
                std::cout << "The new value of the element is set.\n";
            }
            catch (const std::exception& e)
            {
                std::cout << "Error: " << e.what() << ".\n";
            }

            break;
        }

        case 8:
        {
            if (!map.empty())
                std::cout << "The hash map is not empty.\n";
            else
                std::cout << "The hash map is empty.\n";
            
            break;
        }

        case 9:
        {
            std::cout << "Enter the key:\n";
            std::string key = get_string();

            if (map.count(key) > 0)
                std::cout << "The element belongs to the container.\n";
            else
                std::cout << "The element does not belong to the container.\n";

            break;
        }

        case 10:
        {
            map.clear();
            std::cout << "Hash map cleared.\n";
            
            break;
        }

        case 11:
        {
            if (map.size() + 1 > MAX_SIZE)
            {
                std::cout << "Error: the maximum allowed number of elements "
                    "in the hash map has been exceeded.\n";
                std::cout << "Maximum number of elements: " << MAX_SIZE
                    << ".\n";

                break;
            }

            std::cout << "Enter the key:\n";
            std::string key = get_string();
            std::cout << "Enter the value:\n";
            int value = get_int();

            bool is_success = map.insert(std::make_pair(key, value)).second;
            
            if (!is_success)
                std::cout << "The element was not inserted.\n";
            else
                std::cout << "The element has been successfully inserted.\n";

            break;
        }

        case 12:
        {
            std::cout << "Enter the key:\n";
            std::string key = get_string();

            if (map.erase(key) > 0)
                std::cout << "The element was removed from the container.\n";
            else
                std::cout << "No element has been removed from the "
                    "container.\n";

            break;
        }

        case 13:
        {
            std::cout << "Enter the number of buckets:\n";
            size_t buckets_count = get_int();
            while (buckets_count < 0 || buckets_count > MAX_SIZE)
            {
                std::cout << "Error: the entered number of buckets "
                    "is less than 0 or greater than " << MAX_SIZE << ".\n";
                buckets_count = get_int();
            }

            map.rehash(buckets_count);
            std::cout << "The number of baskets has been updated.\n";

            break;
        }

        case 14:
        {
            std::cout << "Enter the value of capacity:\n";
            size_t capacity = get_int();
            while (capacity < 0 || capacity > MAX_SIZE)
            {
                std::cout << "Error: the entered value of capacity "
                    "is less than 0 or greater than " << MAX_SIZE << ".\n";
                capacity = get_int();
            }

            map.reverse(capacity);
            std::cout << "Container capacity updated.\n";

            break;
        }

        case 15:
        {
            std::cout << "Hash map:" << std::endl;
            map.print(std::cout);
            std::cout << std::endl;
            
            break;
        }
        case 16:
        {
            HashMap<std::string, int>::iterator iter = map.begin();

            std::cout << "Section \"Iterator\":\n\n";

            std::cout << "Select the type of iterator to create:\n";
            std::cout << "\t1: Set to start.\n";
            std::cout << "\t2: Set to an element by key.\n";
            menu_item = get_int();
            while (menu_item < 1 || menu_item > 2)
            {
                std::cout << "Error: this menu item does not exist.\n";
                menu_item = get_int();
            }
            switch (menu_item)
            {
            case 1: break;
            
            case 2:
            {
                std::cout << "Enter the key:\n";
                std::string key = get_string();

                iter = map.find(key);
                
                if (iter == map.end())
                    std::cout << "The iterator is in an unspecified state.\n";
                else
                    std::cout << "The iterator is set to this element.\n";
                
                break;
            }
            }
            std::cout << "Iterator created.\n\n";

            bool isExit = false;
            while (!isExit)
            {
                std::cout << "Menu \"Iterator\":\n";
                std::cout << "\t1: Set to an element by key.\n";
                std::cout << "\t2: Dereference iterator.\n";
                std::cout << "\t3: Increment iterator.\n";
                std::cout << "\t4: Exit this section.\n";

                std::cout << "Select a menu item:\n";
                menu_item = get_int();
                while (menu_item < 1 || menu_item > 4)
                {
                    std::cout << "Error: this menu item does not exist.\n";
                    menu_item = get_int();
                }
                std::cout << std::endl;

                switch (menu_item)
                {
                case 1:
                {
                    std::cout << "Enter the key:\n";
                    std::string key = get_string();

                    iter = map.find(key);
                    
                    if (iter == map.end())
                    {
                        std::cout << "The iterator is in an unspecified "
                            "state.\n";
                    }
                    else
                        std::cout << "The iterator is set to this element.\n";
                    
                    break;
                }

                case 2:
                {
                    if (iter != map.end())
                    {
                        auto item = *iter;
                        std::cout << "Iterator element:\n";
                        std::cout << "Key: " << item.first << ".\n";
                        std::cout << "Value: " << item.second << ".\n";
                    }
                    else
                        std::cout << "Error: iterator is not set.\n";

                    break;
                }

                case 3:
                {
                    iter++;
                        
                    if (iter == map.end())
                    {
                        std::cout << "The iterator is in an unspecified "
                            "state.\n";
                    }
                    else
                    {
                        std::cout << "The iterator is set to the next "
                            "element.\n";
                    }

                    break;
                }

                case 4:
                {
                    std::cout << "Exit from this section...\n";
                    isExit = true;
                    
                    break;
                }
                }

                std::cout << std::endl;
            }

            break;
        }

        case 17:
        {
            std::cout << "Exit the program..." << std::endl;
            isExit = true;
            
            break;
        }
        }

        std::cout << std::endl;
    }

    return 0;
}

int get_int()
{
    std::string buffer;
    int result;

    std::cout << ">>> ";
    std::getline(std::cin, buffer);

    while (true)
    {
        try
        {
            result = std::stoi(buffer);
            break;
        }
        catch (const std::invalid_argument&)
        {
            std::cout << "invalid input data format." << std::endl;
        }
        catch (const std::out_of_range&)
        {
            std::cout << "the entered value has exceeded the acceptable "
                "limits." << std::endl;
        }

        std::cout << ">>> ";
        std::getline(std::cin, buffer);
    }

    return result;
}

float get_float()
{
    std::string buffer;
    float result;

    std::cout << ">>> ";
    std::getline(std::cin, buffer);

    while (true)
    {
        try
        {
            result = std::stof(buffer);
            break;
        }
        catch (const std::invalid_argument&)
        {
            std::cout << "invalid input data format." << std::endl;
        }
        catch (const std::out_of_range&)
        {
            std::cout << "the entered value has exceeded the acceptable "
                "limits." << std::endl;
        }

        std::cout << ">>> ";
        std::getline(std::cin, buffer);
    }

    return result;
}

std::string get_string()
{
    std::string result;

    std::cout << ">>> ";
    std::cin >> result;
    std::cin.ignore(32767, '\n');

    return result;
}

std::string get_rand_string(size_t start_len, size_t end_len)
{
    std::string result;                                                 
    size_t len = std::rand() % end_len + start_len;

    if (len > end_len)
        len = end_len;

    for (size_t i = 0; i < len; i++)
    {
        unsigned char let = 0;

        while
        (
            !(('A' <= let && let <= 'Z') || ('a' <= let && let <= 'z') ||
            ('0' <= let && let <= '9'))
        )
        {
            let = std::rand() % 256;
        }

        result.push_back(let);
    }

    return result;
}
