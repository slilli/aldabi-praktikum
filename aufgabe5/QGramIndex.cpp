//
// Created by Lilli Schuckert and Charlotte Seehagen on 11.01.21.
//

#include "QGramIndex.hpp"
#include <iostream>
#include <stdexcept>

const std::string& QGramIndex::getText() const
{
    return this->text;
}

uint8_t QGramIndex::getQ() const
{
    return this->q;
}

uint32_t QGramIndex::hash(const std::string& qgram) const
{
    uint8_t q = getQ();

    if(q!=qgram.length()) {throw std::invalid_argument("Invalid q-gram. Wrong length!");}
    else
    {
        uint32_t h=0;

        for (unsigned i=0; i<q; ++i)
        {
            h *= 4;
            h += ordValue(qgram[i]);
        }
        return h;

    }

}

uint32_t QGramIndex::hashNext(const uint32_t prev_hash, const char new_pos) const
{
    uint8_t q = getQ();
    
    uint32_t hash = 0;
    uint32_t mask = ~0 << 2*q; ///all 1 shifting 2*q 0´s

    hash = ((prev_hash << 2) | ordValue(new_pos)) & ~mask; ///shift by 2 bc highest bit value is 3

    return hash;
}

std::vector<uint32_t> QGramIndex::getHits(const uint32_t h) const
{
    uint8_t q = getQ();
    
    if (h < 0 || h >= pow(4,q)) {throw std::invalid_argument("Invalid hash!");}

    else
    {
        std::vector<uint32_t> hits;

        for (uint32_t i=dir[h]; i<dir[h+1]; ++i)
        {
            hits.push_back(suftab[i]);
        }
        return hits;
    }

}

QGramIndex::QGramIndex(const std::string& text_, const uint8_t q_)
        : text{text_}, q{q_} ///constructor list
{
    //this->q = q_;
    //this->text = text_;

    if(q_<1 || q_>13) {throw std::invalid_argument("Invalid q!");}

    else
    {
        ///step 1 initialisation: fill constructor
        std::vector<uint32_t>dir(pow(4,q)+1);
        std::vector<uint32_t>suftab(text.size()-(q-1)+1);///only substrings of length q are in suftab

        ///step 2 counting qgrams
        if (q==1) ///no rolling hash
        {
            for (uint32_t i=0; i<text.size(); ++i)
            {
                uint32_t h = hash(text.substr(i,q));
                dir[h]++;
            }
        }
        else
        {
            uint32_t j=0;
            uint32_t h = hash(text.substr(0,q));
            dir[h]++;

            for (uint32_t i=1; i<=text.size()-q+1; ++i)
            {
                j = hashNext(h,text[i+q-1]);
                dir[j]++;
                h = j;
            }
        }


        ///step 3 build cumulative sum
        for (uint32_t i=1; i<dir.size(); ++i) {dir[i] += dir[i-1];}

        ///step 4 sort occurences into suftab

        if (q==1) ///no rolling hash
        {
            for(uint32_t i=0; i<text.size();++i)
            {
                uint32_t h = hash(text.substr(i,q));
                dir[h]--;
                suftab[dir[h]] = i;
            }
        }
        else
        {
            uint32_t j=0;
            uint32_t h = hash(text.substr(0,q));
            dir[h]--;
            suftab[dir[h]] = 0;

            for (uint32_t i=1; i<=text.size()-q+1; ++i)
            {
                j = hashNext(h,text[i+q-1]);
                dir[j]--;
                suftab[dir[j]] = i;
                h = j;
            }
        }

        this->dir=dir;
        this->suftab=suftab;

    }
}
