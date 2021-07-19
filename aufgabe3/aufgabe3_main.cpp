//
// Created by Lilli Schuckert and Charlotte Seehagen on 30.11.20.
//

#include "Alignment.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

//argc:number of arguments passed via terminal
//argv:arguments

int main(int argc, char* argv[])
{
    std::string a1{}, a2{}, gaps{};
    
    if (argc == 3)
    {
        Alignment align(argv[1],argv[2]);
        align.compute(3,-1,-2);
        align.getAlignment(a1, gaps, a2);
        int s = align.score();
        
        std::cout<< a1 <<'\n' << gaps << '\n' << a2 << '\n' << s << '\n';
        return 0;
    }
    else if (argc == 6)
    {
        Alignment align(argv[1], argv[2]);
        align.compute(std::stoi(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]));
        align.getAlignment(a1,gaps,a2);
        int s = align.score();
        
        std::cout<< a1 <<'\n' << gaps << '\n' << a2 << '\n' << s << '\n';
        return 0;
    }
   
    //Smith Waterman
    if (argc == 7) //./aufgabe3_main seqV seqH match mismatch gap irgendwas
    {
        Alignment align(argv[1], argv[2]);
        align.compute(std::stoi(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]), std::stoi(argv[6]));
        align.getAlignment(a1,gaps,a2);
        int s = align.score();
        
        std::cout<< a1 <<'\n' << gaps << '\n' << a2 << '\n' << s << '\n';
        return 0;
    }
    
    else {std::cout<< "unexpected input\n"; return 1;}
}
