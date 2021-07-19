//
// Created by Lilli Schuckert and Charlotte Seehagen on 17.11.20.
//

#include "aufgabe2.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <string_view>

void construct(std::vector<uint32_t>& sa, const std::string& text){

    if (text!=""){
        sa.clear(); ///empties sa if not already empty

        for (unsigned i=0; i<text.size(); i++) ///fills sa with text.size() amount of numbers
        {
            sa.push_back(i);
        }

        std::sort(sa.begin(), sa.end(), [&text] (unsigned a, unsigned b) { ///sorts sa
            while ( (a < text.size()) || (b < text.size()))
            {
                if (text.at(a) < text.at(b)){return true;}
                else if (text.at(a) > text.at(b)){return false;}
                else if (text.at(a) == text.at(b))
                {
                    if ((a+1) == text.size()){return true;} ///substring a reaches end and is lexicographically smaller than b
                    if ((b+1) == text.size()) {return false;} ///substring b reaches end and is lexicographically smaller than a
                    ++a;
                    ++b;
                }
            }
            return true;
        });
    } else {
        std::cout<< "unexpected input\n";
    }
}

int32_t lcp (const std::string& text, const std::string& query, uint32_t t)
{
    uint32_t i = 0;		///counter
    for (uint32_t z = 0; z < query.size(); ++t)
    {
        if (text[t] == query[z]){++i;}		///increases counter if chars are equal -->needed to calculate l and r for mlr search
        else{return i;}
    }
    return i;
}

bool lp (const std::string& text, const std::string& query, uint32_t index, uint32_t mlr)
{
	index=index+mlr;	///Vergleich beginnt an Position mlr -->spart Vergleiche
	
	for(uint32_t j = mlr; j <query.length(); ++j)
	{
		if (query[j] < text[index]){return true;}	///if query <= suftab[] then true
		if (query[j] > text[index]){return false;}
		if(index+1 == text.length() && j+1 != query.length()){return false;}	
		++index;
	}
	return true;
}

bool rp (const std::string& text, const std::string& query, uint32_t index, uint32_t mlr)
{
	index=index+mlr;	///Vergleich beginnt an Position mlr -->spart Vergleiche
	
	for(uint32_t j = mlr; j <query.size(); ++j)
	{
		if (query[j] < text[index]){return false;}		///if query <= suftab[] then false
		if (query[j] > text[index]){return true;}
		if(index+1 == text.length()) {return true;}
		++index;
	}
	return true;
}

/// Search a string in a text via suffix array, which was constructed before using 'construct()'. If the query is empty, return empty hits.
/// Input: search for a 'query' string in a suffix array 'sa' build from 'text'.
/// Output: Results are returned in 'hits' (sorted ascending by position in text!). The variable is cleared before its filled and returned.
void find(const std::string& query, const std::vector<uint32_t>& sa, const std::string& text, std::vector<uint32_t>& hits)
{
	hits.clear();		///clears hits
    uint32_t Rp = 0;
    uint32_t Lp = 0;

    if (text.size() == 0) {return;}		/// case if text size is 0
    else if (query.size() == 0) {return;}		/// case if query size is 0
    else
    {
		///binsearch rp
		if (!rp(text,query,sa.at(0),0)) {Rp = 1;}		///query < suftab[0]
		else if (rp(text,query,sa.at(sa.size()-1),0)) {Rp = sa.size()-1;}	///query >= suftab[sa.size()-1]
		else
		{
			float L = 0;
			float R = sa.size() - 1;
			while ((R - L) > 1)
			{
				int32_t l = lcp(query, text, sa.at(L));
				int32_t r = lcp(query, text, sa.at(R));
				int mlr = std::min(l, r);		///mlr is the minimum of l and r
				int M = ceil((L + R) / 2);		
				if (rp(text,query,sa.at(M),mlr)) {L=M;}    ///if query at mlr <= suftab[M] at mlr
				else { R = M; }
			}
		Rp = L;
		}
		///binsearch lp
		if (!lp(text,query,sa.at(sa.size()-1),0)) { Lp = text.length();}	///if query > suftab[sa.size()-1]
		else if (lp(text,query,sa.at(0),0)) {Lp = 0;} 	///if query <= suftab[0]
		else
		{
			float L = 0;	///floats in order to use ceil
			float R = sa.size() - 1;
			while ((R - L) > 1)
			{
				int32_t l = lcp(query,text, sa.at(L)); 
				int32_t r = lcp(query,text, sa.at(R));
				int mlr = std::min(l, r);	///mlr is the minimum of l and r
				int M = ceil((L + R) / 2);
				if (lp(text,query,sa.at(M),mlr)) {R=M;}    ///if query at mlr <= suftab[M] at mlr
				else { L = M;}
			}
		Lp = R;	///returns R from pair (L,R) --> left boundary
		}
       
    ///iterates over all elements between Lp and Rp and push backs those elements into vector hits
    for (uint32_t i = Lp; i <= Rp; ++i)	
    {
		hits.push_back(sa.at(i));
	}
    sort(hits.begin(), hits.end());		///sorts hits
   }
}

/*
Lösung mit substr. mlr nicht ganz korrekt

int32_t lcp (std::string str1, std::string str2)
{
    uint32_t i = 0;
    while(i<std::min(str1.size(),str2.size()))
    {
        if (str1[i] == str2[i]){++i;}	///increases counter if chars are equal
        else{return i;}
    }
    return i;
}

///search for Lp
int32_t binsearchlp( const std::vector<uint32_t>& sa, const std::string& text, const std::string& query)
{
    int32_t Lp;

    if (query <= text.substr(sa.at(0))){return Lp = 0;}		///query<=Suftab[0]
    else if (query == text.substr(sa.at(0))){return Lp = 0;}
    else if (query > text.substr(sa.at(text.length()-1))){return Lp = text.length();}	///query >=suftab[n]
    float L = 0;	///floats in order to use ceil
    float R = sa.size() - 1;

    while ((R - L) > 1) {
        int32_t l = lcp(query, text.substr(sa.at(L))); ///lcp from query to sa[L]
        int32_t r = lcp(query, text.substr(sa.at(R)));
        int mlr = std::min(l, r);	///mlr is the minimum of l and r
        int M = ceil((L + R) / 2);
        std::cout << L << " " << M << " " << R << "\n";
        if (query.substr(mlr, query.size()) <= text.substr(sa.at(M)+mlr, query.size()))      ///if query+mlr <= suftab[M]+mlr
        {
            R = M;
        }
        else { L = M; }
    }
    return Lp = R;	///returns R from pair (L,R) --> left boundary

}

///search for Rp
int32_t binsearchrp( const std::vector<uint32_t>& sa, const std::string& text, const std::string& query)
{
    int32_t Rp;
    if (query < text.substr(sa.at(0))){return 1;}
    else if (query == text.substr(sa.at(0))) {return Rp = 0;}
    else if (query > text.substr(sa.at(text.length()-1))){return -1;}
    float L = 0;
    float R = sa.size() - 1;

    while ((R - L) > 1) {
        int32_t l = lcp(query, text.substr(sa.at(L)));
        int32_t r = lcp(query, text.substr(sa.at(R)));
        int mlr = std::min(l, r);
        int M = floor((L + R) / 2);		///rounds of (pseudocode aldabi)
        if (query.substr(mlr, query.size()) >= text.substr(sa.at(M)+mlr, query.size()))      ///if query <= suftab[M]
        {
            L = M;
        }
        else { R = M; }
    }
    return Rp = L;
}

/// Search a string in a text via suffix array, which was constructed before using 'construct()'. If the query is empty, return empty hits.
/// Input: search for a 'query' string in a suffix array 'sa' build from 'text'.
/// Output: Results are returned in 'hits' (sorted ascending by position in text!). The variable is cleared before its filled and returned.
void find(const std::string& query, const std::vector<uint32_t>& sa, const std::string& text, std::vector<uint32_t>& hits)
{
    uint32_t Rp = 0;
    uint32_t Lp = 0;

    hits.clear();		///clears hits

    if (text.size() == 0) {return;}		///case if text size is 0
    else if (query.size() == 0) {return;}		///case if query size is 0
    else
    {
        Lp = binsearchlp(sa, text, query);		///find Lp
        Rp = binsearchrp(sa, text, query);		///find Rp
 
            for (uint32_t i = Lp; i <= Rp; ++i) {		///iterates over all elements between Lp and Rp
                hits.push_back(sa.at(i));		///push backs those elements into vector hits
            }
            sort(hits.begin(), hits.end());		///sorts hits
        
    }
}
*/
