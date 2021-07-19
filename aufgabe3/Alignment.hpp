#pragma once
#include <string>
#include <vector>
#include <iostream>

// additional includes can go here:
// ...


class Alignment
{
public:
  // DO NOT CHANGE THE PUBLIC INTERFACE!
  
  /// This makes the default constructor private
  /// i.e. an object of the class can only be created with sequences (see below)
  Alignment() = delete;  // no NOT implement this function. Just leave it as is.
  
  /// Constructor with two sequences
  /// Makes an internal copy of the sequences.
  Alignment(const std::string& seq_v, const std::string& seq_h);
  
  /// compute the aligment (i.e. score and traceback)
  /// given the three alignment parameters match, mismatch and gap
  /// if local_align == true, compute the local Smith-Waterman (SW) alignment (extra points), or throw
  /// an exception if your implementation does not support SW.
  void compute(const int match, const int mismatch, const int gap, const bool local_align = false);
  
  /// return the score of the alignment;
  /// Throws an exception if compute(...) was not called first
  int score() const;
  
  /// output alignment into three strings.
  /// Gaps are denoted as '-' in sequences.
  /// The gap-string uses '|' (match) and ' ' (mismatch/gap).
  /// Note: all strings are equal in size
  /// e.g.
  /// a1:   "IMISSMISSIS-SIPPI-"
  /// gaps: " |   ||||||  |||| "
  /// a2:   "-M--YMISSISAHIPPIE"
  /// , where a1 corresponds to seq1, etc.
  /// Throws an exception if compute(...) was not called first
  void getAlignment(std::string& a1, std::string& gaps, std::string& a2) const;
  
private:
  // add your private functions and member variables here
  // ...

    uint64_t i;
    uint64_t j;
    
    std::string seq_v;
    std::string seq_h;

    std::vector<std::vector<int64_t>>matrix;

    enum class Traceback
    {
        DIAGONALMATCH, DIAGONALMISMATCH, HORIZONTAL, VERTICAL, NONE
    };

    std::vector<std::vector<Traceback>> traceback;
    
    int maxk=1;
    int maxl=1;

    bool check=false;
    bool sw=false;
    
    std::string a1;
    std::string a2;
    std::string gaps;
    
    
    void reverse (std::string& s1) const
    {
        int l = s1.size();
        for (int c = 0; c < l / 2; c++)
            std::swap(s1[c], s1[l-c-1]);
    }
    
};
