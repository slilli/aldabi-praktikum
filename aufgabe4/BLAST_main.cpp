#include <iostream>
#include "a4_util.h"
#include "BLAST.hpp"
#include <omp.h>
int main (int argc, const char* argv[]){

  if(argc != 6)
  {
    std::cerr << "Please enter five arguments." << '\n';
    return 1;
  }
    
    ScoreMatrix matrix = ScoreMatrix();
    matrix.load(argv[2]);
    
    BLAST_Neighborhood nh;

    double start = omp_get_wtime();
    std::vector<NHResult> result = nh.generateNeighborhood(argv[1], matrix, std::stoi(argv[3]), std::stoi(argv[4]);, std::stoi(argv[5]));
    double end = omp_get_wtime();

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


