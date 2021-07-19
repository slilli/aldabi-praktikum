//
// Created by lillischuckert on 11.01.21.
//

#include "QGramIndex.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    if (argc != 3) {throw "Please enter 2 arguments!";}

    else
    {
        std::ifstream file;
        file.open(argv[1]);

        if (file.good()) ///checks if file is readable
        {
            std::string text;
            getline(file, text); ///file is stored in text
            file.close();

            std::string query = argv[2];
            uint8_t q = query.size();

            QGramIndex qi {text, q};

            uint32_t h = qi.hash(query);
            std::vector<uint32_t> hit = qi.getHits(h);

            
            std::cout<< query << ": ";

            for (unsigned i=0; i<hit.size(); ++i)
            {
                std::cout<< hit[i] << " ";
            }
            std::cout<<std::endl;
        }
        else {throw "Could not open file";}
    }
}
