//
// Created by Lilli Schuckert and Charlotte Seehagen on 30.12.20.
//

#include <iostream>
#include "BLAST_Neighborhood.hpp"
#include "a4_util.h"
#include <omp.h>

int main (int argc, const char* argv[]){

    if(argc != 6)
    {
        std::cerr << "Please enter five arguments." << '\n';
        return 1;
    }

    ScoreMatrix matrix = ScoreMatrix(); ///object to call function on
    matrix.load(argv[2]);  ///load matrix
    BLAST_Neighborhood nh; ///object to call function on

    ///measrure time for generateNeighborhood function
    double start = omp_get_wtime();
    std::vector<NHResult> result = nh.generateNeighborhood(argv[1], matrix, std::stoi(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]));
    double end = omp_get_wtime();

    ///output
    for (unsigned i = 0; i<result.size(); ++i)
    {
        std::string infix = result[i].infix;
        std::vector<std::pair<std::string,int>> neighbors = result[i].neighbors;
        std::cout<<infix<<":";

        for(unsigned j = 0; j<neighbors.size(); j++)
        {
            std::cout<<" ("<<neighbors[j].first<<", "<<neighbors[j].second<<")";
        }
        std::cout<<"\n";
    }

    std::cout<<"time: "<<(end-start)<<std::endl;
    return 0;
}