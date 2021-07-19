//
// Created by Lilli Schuckert and Charlotte Seehagen on 30.11.20.
//

#include "Alignment.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

Alignment::Alignment(const std::string& seq_v, const std::string& seq_h)
{
    this->seq_v = seq_v;
    this->seq_h = seq_h;

}

void Alignment::compute (const int match, const int mismatch, const int gap, const bool local_align)
{
    
    this->matrix={};
    this->traceback={};
    
    std::vector<int64_t> rows(seq_h.size()+1, 0);
    std::vector<std::vector<int64_t>> matrix(seq_v.size()+1, rows);

    std::vector<Traceback> trows(seq_h.size()+1, Traceback::NONE);
    std::vector<std::vector<Traceback>> traceback(seq_v.size()+1, trows);
    
    ///needleman wunsch algorithm
    
    if (local_align==false)
    {
        if (seq_v!="" && seq_h!="") ///initialization
        {
            for(i = 0; i <= seq_h.size(); ++i){matrix[0][i] = i*(gap);}
            for(j = 0; j <= seq_v.size(); ++j){matrix[j][0] = j*(gap);}
      

            for (i = 1; i <= seq_h.size(); ++i) ///recurrence
            {
                for (j = 1; j <= seq_v.size(); ++j)
                {
                    int diagonalmatch = matrix[j - 1][i - 1] + match;
                    int diagonalmismatch = matrix[j - 1][i - 1] + mismatch;
                    int horizontalgap = matrix[j][i-1] + gap;
                    int verticalgap = matrix[j-1][i] + gap;

                    if (seq_h[i-1] == seq_v[j-1])    ///match
                    {
                        matrix[j][i] = std::max(diagonalmatch, (std::max(horizontalgap, verticalgap)));
                       
                        if (matrix[j][i] == diagonalmatch){traceback[j][i] = Traceback::DIAGONALMATCH;}
                        else if (matrix[j][i] == verticalgap){traceback[j][i] = Traceback::HORIZONTAL;}
                        else {traceback[j][i] = Traceback::VERTICAL;}
                    }
                    else    ///mismatch
                    {
                        matrix[j][i] = std::max(diagonalmismatch, (std::max(horizontalgap, verticalgap)));

                        if (matrix[j][i] == diagonalmismatch){traceback[j][i] = Traceback::DIAGONALMISMATCH;}
                        else if (matrix[j][i] == verticalgap){traceback[j][i] = Traceback::HORIZONTAL;}
                        else {traceback[j][i] = Traceback::VERTICAL;}
                    }
                }
                this->matrix=matrix; this->traceback=traceback; this->check=true; ///check=true bc compute has been called
            }
        }
        else ///empty sequences
        {
            matrix[0][0]=0;
            traceback[0][0]=Traceback::NONE;
            
            this->matrix=matrix; this->traceback=traceback; this->check=true;
        }
    }
    
    ///smith waterman algorithm
    
    else if (local_align==true)
    {
        if (seq_v!="" && seq_h!="") ///initialization
        {
            for(i = 0; i <= seq_h.size(); ++i){matrix[0][i] = 0; traceback[0][i] = Traceback::NONE;}
            for(j = 0; j <= seq_v.size(); ++j){matrix[j][0] = 0; traceback[j][0] = Traceback::NONE;}
  
            for (i = 1; i <= seq_h.size(); ++i) ///recurrence
            {
                for (j = 1; j <= seq_v.size(); ++j)
                {
                    int diagonalmatch = matrix[j - 1][i - 1] + match;
                    int diagonalmismatch = matrix[j - 1][i - 1] + mismatch;
                    int horizontalgap = matrix[j][i-1] + gap;
                    int verticalgap = matrix[j-1][i] + gap;
                    int zero = 0;
                    

                    if (seq_h[i-1] == seq_v[j-1])    ///match
                    {
                        matrix[j][i] = std::max((std::max(zero, diagonalmatch)), (std::max(horizontalgap, verticalgap)));

                        if (matrix[j][i] == zero){traceback[j][i] = Traceback::NONE;}
                        if (matrix[j][i] == diagonalmatch){traceback[j][i] = Traceback::DIAGONALMATCH;}
                        else if (matrix[j][i] == verticalgap){traceback[j][i] = Traceback::HORIZONTAL;}
                        else {traceback[j][i] = Traceback::VERTICAL;}
                    }
                    else    ///mismatch
                    {
                        matrix[j][i] = std::max((std::max(zero, diagonalmismatch)), (std::max(horizontalgap, verticalgap)));
                                                                                                                        
                        if (matrix[j][i] == zero){traceback[j][i] = Traceback::NONE;}                                                                                          
                        if (matrix[j][i] == diagonalmismatch){traceback[j][i] = Traceback::DIAGONALMISMATCH;}
                        else if (matrix[j][i] == verticalgap){traceback[j][i] = Traceback::HORIZONTAL;}
                        else {traceback[j][i] = Traceback::VERTICAL;}
                    }
                    if (matrix[maxl][maxk] <= matrix[j][i]){this->maxl=j; this->maxk=i;} ///tracks highest value
                }
                this->matrix=matrix; this->traceback=traceback; this->check=true; this->sw=true; ///sm=true bc smith waterman is used
            }
        }
        else ///empty sequences
        {
            matrix[0][0]=0;
            traceback[0][0]=Traceback::NONE;
        
            this->matrix=matrix; this->traceback=traceback; this->check=true; this->sw=true;
        }
    }
}

int Alignment::score() const
{
    if (sw==false && check==true){return matrix[seq_v.size()][seq_h.size()];} ///nw -> score is at the bottom right of matrix
    else if (sw==true && check==true){return matrix[maxl][maxk];} ///sw -> score is the highest value in matrix
    else {throw "exeption: compute was not called first";}
}

void Alignment::getAlignment(std::string& a1, std::string& gaps, std::string& a2) const
{
    a1.clear();
    a2.clear();
    gaps.clear();
    
    ///nw -> starting traceback at bottom right

    if (sw==false && check==true &&  seq_v!="" && seq_h!="")
    {
        uint64_t m=seq_v.size();
        uint64_t n=seq_h.size();
    
        while (traceback[m][n]!=Traceback::NONE)
        {
                if (traceback[m][n]==Traceback::DIAGONALMATCH)
                {
                    a1.push_back(seq_v[m-1]);
                    a2.push_back(seq_h[n-1]);
                    gaps += "|";
                    --m;
                    --n;
                }
                else if (traceback[m][n]==Traceback::DIAGONALMISMATCH)
                {
                    a1.push_back(seq_v[m-1]);
                    a2.push_back(seq_h[n-1]);
                    gaps += " ";
                    --m;
                    --n;
                }
                    
                else if (traceback[m][n]==Traceback::HORIZONTAL)
                {
                    a1.push_back(seq_v[m-1]) ;
                    a2+= "-";
                    gaps += " ";
                    --m;
                }
                else
                {
                    a1 += "-";
                    a2.push_back(seq_h[n-1]) ;
                    gaps += " ";
                    --n;
                }
        }
        ///if traceback is none but matrix[0][0] is not reached yet
        while (n!=0) ///horizontal
        {
            a1 += "-";
            a2.push_back(seq_h[n-1]) ;
            gaps += " ";
            --n;
        }
    
        while (m!=0) ///vertical
        {
            a1.push_back(seq_v[m-1]) ;
            a2+= "-";
            gaps += " ";
            --m;
        }
        
        reverse(a1);
        reverse(a2);
        reverse(gaps);
    }
    
    ///sw -> starting traceback at highest value
    else if (sw==true && check==true &&  seq_h!="" && seq_h!="")
    {
        uint64_t m=maxl;
        uint64_t n=maxk;
           
           while (matrix[m][n]!=0) ///stopping at end of local alignment aka if value reached is 0
           {
               if (traceback[m][n]==Traceback::DIAGONALMATCH)
               {
                   a1.push_back(seq_v[m-1]);
                   a2.push_back(seq_h[n-1]);
                   gaps += "|";
                   --m;
                   --n;
               }
               else if (traceback[m][n]==Traceback::DIAGONALMISMATCH)
               {
                   a1.push_back(seq_v[m-1]);
                   a2.push_back(seq_h[n-1]);
                   gaps += " ";
                   --m;
                   --n;
               }
                   
               else if (traceback[m][n]==Traceback::HORIZONTAL)
               {
                   a1.push_back(seq_v[m-1]) ;
                   a2+= "-";
                   gaps += " ";
                   --m;
               }
               else
               {
                   a1 += "-";
                   a2.push_back(seq_h[n-1]) ;
                   gaps += " ";
                   --n;
               }
           }
        
           reverse(a1);
           reverse(a2);
           reverse(gaps);

    }
    else if ((check==true && sw==false && seq_h=="" && seq_v=="") || (check==true && sw==true && seq_h=="" && seq_v=="")) ///empty strings
    {
        a1="";
        a2="";
        gaps="";
    }
    
    else {throw "exeption: compute was not called first";}
}
