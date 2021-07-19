#include "BLAST.hpp"
#include "a4_util.h"


std::vector<std:string> BLAST_Neighborhood::generateInfixes(std::string query, int word_size)
{
    std::string infix{};
    std::vector<std::string> infixes{};
    
    for (unsigned i=0; i<query.size()-word_size; ++i)
    {
        infix = query.substr(i,word_size);
        infixes.push_back(infix);
    }
    return infixes;
}

std::vector<std::string> BLAST_Neighborhood::generatePermutations(int counter, int word_size, std::string word)
{
    if (counter==word_size)
    {
        for (unsigned i=0; i<AminoAcidTranslationTable::ALPHABET_SIZE; ++i)
        {
            word.push_back(toAminoAcid(i));
            permutations.push_back(word);
        }
    }
    else
    {
        for (unsigned i=0; i<AminoAcidTranslationTable::ALPHABET_SIZE; ++i)
        {
            word.push_back(toAminoAcid(i));
            std::vector<std::string> permutations = generatePermutations(counter+1, word_size, word);
        }
    }
    return permutations;
}

///sort Funktion für Neighborhood?


std::vector<NHResult> BLAST_Neighborhood::generateNeighborhood(const std::string& query,
                                           const ScoreMatrix& matrix,
                                           const int word_size,
                                           const int score_threshold,
                                           const int threads)
{
    if (word_size<1 || threads != 1) throw "ERROR";
    
    std::vector<NHResult> result{};
    std::vector<std::string> infixes = generateInfixes(query, word_size);
    std::vector<std::string> permutations = generatePermutations(0, word_size, "");
    
    for (unsigned i=0; i<infixes.size(); ++i)
    {
        std::string infix = infixes[i];
        result.infix = infix;
        
        for (unsigned j=0; j<permutations.size() ; ++j)
        {
            std::string permutation = permutations[j];
                            
            for (unsigned c=0; c<infix.size(); ++c)
            {
                int score += matrix.score(infix[c], permutation[c]);
            }
            
            if (score>=score_threshold)
            {
                result.neighbors.push_back(std::make_pair(permutation, score));
                ///sort?
            }
        }
    }
    return result;
}
