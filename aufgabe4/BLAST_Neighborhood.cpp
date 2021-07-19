//
// Created by Lilli Schuckert and Charlotte Seehagen on 30.12.20.
//

#include "a4_util.h"
#include "BLAST_Neighborhood.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "BLAST_Neighborhood.hpp"
#include <omp.h>

void BLAST_Neighborhood::sortNeighborhood(NHResult &neighborhood) const
{
    ///sort function to sort the neighborhood strings lexicographically
    std::sort(neighborhood.neighbors.begin(), neighborhood.neighbors.end(), [] (std::pair<std::string,int> a,std::pair<std::string,int> b)
    {
        std::string pair1 = a.first;
        std::string pair2 = b.first;
        unsigned length = pair1.length();
        unsigned i = 0;

        while (pair1[i]==pair2[i] && i<length) {++i;}

        return pair1[i]<pair2[i];
    });
}

NHResult BLAST_Neighborhood::infixRecursion(const std::string infix, const int score_threshold, const std::vector<std::vector<std::pair<std::string,int>>> & matrix) const
{
    NHResult neighborhood; ///object to call function on
    neighborhood.infix = infix;
    unsigned index = toId(infix[0]); ///index of first letter in infix

    ///recursion anchor-----------------------------------------------------------------------------------
    if(infix.length()==1)
    {
        for (unsigned i = 0; i<20; ++i)
        {
            int score = matrix[index][i].second;
            if (score >= score_threshold) ///only in neighborhood if score is greater than score_threshold
            {
                neighborhood.neighbors.push_back(matrix[index][i]);
            }else i=20;
        }
    }///recursion------------------------------------------------------------------------------------------
    else
    {
        for (unsigned i = 0; i<20; ++i)
        {
            int score = matrix[index][i].second;

            ///to eventually reach the anchor; e.g. infix = AAH --> infix.erase(0,1)--> newinfix = AH -->infix.length()=2
            std::string newinfix = infix;
            newinfix.erase(0,1);

            ///function call with "new" infix
            NHResult rec = infixRecursion(newinfix, score_threshold-score, matrix);

            ///save results in vector neighborhood
            if (rec.neighbors.size()>0)
            {
                for (unsigned j = 0; j<(rec.neighbors.size()); ++j)
                {
                    neighborhood.neighbors.push_back(std::make_pair(matrix[index][i].first + rec.neighbors[j].first, score + rec.neighbors[j].second));
                }
            }
            else i =20;
        }
    }
    return neighborhood;
}


void BLAST_Neighborhood::buildMatrix(const ScoreMatrix& m, std::vector<std::vector<std::pair<std::string,int>>> & matrix)
{
    ///one has to sort the matrix to not generate every k-mere
    ///not possible because variable in which the matrix is stored is private(util.h)
    ///solution: build a copy of original matrix in order to be able to sort the matrix and eventually only generate needed k-meres
    matrix.resize(20);

    for (unsigned i = 0; i<20; ++i)
    {
        for (unsigned j = 0; j<20; ++j)
        {
            std::pair<std::string,int> converter{};
            converter.first.push_back(toAminoAcid(j));
            converter.second = m.score(toAminoAcid(i), toAminoAcid(j));
            matrix[i].push_back(converter);
        }
    }
}


void BLAST_Neighborhood::sortMatrix(std::vector<std::vector<std::pair<std::string,int>>>& matrix)
{
    ///sort scores in matrix
    for(unsigned i = 0; i<20; ++i)
    {
        std::sort(matrix[i].begin(), matrix[i].end(), [] (std::pair<std::string,int> a, std::pair<std::string,int> b) {return (a.second > b.second);});
    }
}


std::vector<NHResult> BLAST_Neighborhood::generateNeighborhood(const std::string& query, const ScoreMatrix& m, const int word_size, const int score_threshold, const int threads)
{
    ///minimum 1 thread is required
    if (word_size<1 || threads <1) throw "ERROR";

    std::vector<NHResult> result{};
    std::vector<std::vector<std::pair<std::string, int>>> matrix{};

    int length = query.length(); ///otherwise Warning comparison
    if (word_size<=length)
    {
        buildMatrix(m, matrix);
        sortMatrix(matrix);

        int count = (query.length()-word_size+1); ///q-w+1
        result.resize(count);

    ///directive "parallel" starts team of threads
    ///directive "for" divides iterations to team members: in this case divided to #number of threads
    #pragma omp parallel for num_threads(threads)
        for(int i = 0; i < count; ++i)
        {
            std::string infix = query.substr(i, word_size); ///generate infix
            NHResult nh = infixRecursion(infix, score_threshold, matrix);
            sortNeighborhood(nh);

            result[i]=nh;
        }
    }
    return result;
}
