//
// Created by Lilli Schuckert and Charlotte Seehagen on 17.11.20.
//

#include "aufgabe2.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

int main(int argc, const char* argv[]) //Zeiger auf char arguments
{
    std::vector<uint32_t> sa;
    std::vector<uint32_t> hits;
    if (argc == 2) {	///2 arguments are passed which is why one only constructs sa
        construct(sa, argv[1]);
        for (uint i = 0; i < sa.size(); ++i) {
            std::cout << sa[i] << "\n";
        }

    }
    else if (argc > 2)	///more than two arguments are passed 
    {
        construct(sa, argv[1]);
        for (int i = 2; i < argc; ++i)
        {
            find(argv[i], sa, argv[1], hits);
            std::cout << argv[i] << ": ";
            for (uint i = 0; i < hits.size(); ++i) {
                std::cout << hits[i] << " ";
            }
            std::cout << "\n";
        }
    }
    else{std::cout << "Unexpected input \n";} ///argc == 1
    return 0;
}
