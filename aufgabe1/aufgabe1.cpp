//
// Created by Lilli Schuckert and Charlotte Seehagen on 04.11.20.
// AlDaBi Praktikum Aufgabe 1
//
#include "aufgabe1.hpp"
#include <iostream>
#include <string>
#include <fstream> //reading and writing FASTA entry
#include <utility> //for pair

char complement(char const x)
{
    char a{};       //new char, because x is const
    switch (x)
    {
        case 'A':
            return 'T';
        case 'T':
            return 'A';
        case 'G':
            return 'C';
        case 'C':
            return 'G';
        default:
            std::cout << "Wrong input. \n";
    }
    return a;       //otherwise "control reaches end of non-void function"
}

std::string reverseComplement(std::string const& input)
{
    std::string dna{};      //new string because input is passed const and by reference
    int64_t i = input.size() - 1;       //indices start at 0, e.g. string with 6 bases has indices from 0 to 5
    while(i >= 0){dna.push_back(complement(input[i--]));}       //push backs the complementary bases to new string dna
                                                                //starting from the last indice and doing so until
                                                                //the while loop reaches indice 0
    return dna;         //returns the reversed dna string
}

std::pair<std::string, std::string> readFasta(std::string const& in_file)
{
    std::ifstream infile;       //ifstream is used to read files
    infile.open(in_file);
    std::pair<std::string,std::string>fastapair;        //either filled with meta and sequence or empty at the end

    if (infile.good())      //tests if the state of the stream is good
    {
        std::string meta;
        std::string seq;
        std::string sequence;
        getline(infile, meta);      //getline gets the first line which is in this case the meta data
                                          //and saves it in string meta
        while (getline (infile, seq)){sequence += seq;}     //concatenates lines of sequence(without linebreaks)
        fastapair = std::make_pair (meta, sequence);        //returns pair with meta data and sequence
    }
    else{fastapair = std::make_pair ("","");}       //returns pair with empty strings if infile is not good
    infile.close();     //file needs to be closed after finishing accessing it
    return fastapair;

}

bool writeFasta(std::string const& out_file,
                std::string const& meta,
                std::string const& seq)
{
    std::ofstream outfile;      //ofstream is used to write files
    outfile.open(out_file);

    if (outfile.is_open())
    {
        outfile << meta;        // '<<' is used to write something to a file
        for(uint64_t i=0; i <seq.length(); ++i)
        {
            if((i%80) == 0){outfile << '\n' << seq[i];      //writes a linebreak and then 80 characters of sequence
            }else{outfile << seq[i];}       //writes remaining sequence (shorter than 80 characters) to outfile
        }
        outfile.close();
        return true;
    }
    else {return false;}        //outfile was not open
}

bool reverseComplementFASTA(std::string const& input_file, std::string const& output_file)
{
    std::string in{}, out{};
    in = input_file;
    out = output_file;

    std::pair <std::string, std::string> pair = readFasta(in);

    bool erg = writeFasta(out, pair.first, reverseComplement(pair.second));     //accesses the second element of the pair
                                                                                //with meta data and reverses second
                                                                                //element of the pair

    return erg;
}
