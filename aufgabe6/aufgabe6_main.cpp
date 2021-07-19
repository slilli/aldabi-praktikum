#include "ACTrie.hpp"
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
    if (argc < 3) {std::cerr <<"Please enter at least one needle!\n"; return 1;}
    
    else
    {
        ///create vector of all needles for constructor
        std::vector<std::string> needles{};
        for(unsigned i=2; i<argc; ++i) {needles.push_back(argv[i]);}
        
        ACTrie trie {needles};
        
        std::string haystack = argv[1];
        trie.setQuery(haystack);
        
        std::vector<Hit> hit;
        std::vector<Hit> out{};
        
        while (trie.next(hit))
        {
            for(unsigned i=0; i<hit.size(); ++i) {out.push_back(hit[i]);} ///push back hits after every loop
        }
        
        for(unsigned i=0; i<out.size(); ++i)
        {
            std::cout << "pos " << out[i].pos << ", " << needles[out[i].index] << '\n';
        }
    }
}
