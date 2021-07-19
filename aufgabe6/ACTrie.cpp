//
// Created by Lilli Schuckert and Charlotte Seehagen
//

#include "ACTrie.hpp"
#include <stdexcept>
#include <iostream>

void ACTrie::buildTree(ACNode root, std::vector<ACNode> trie, const std::vector<std::string>& needles)
{
    trie.push_back(root);

    for (unsigned i=0; i<needles.size(); ++i)
    {
        Index current;
        current.pos() = 0;

        for (unsigned j=0; j<needles[i].size(); ++j)
        {
            if (int(needles[i][j]) < 65 || int(needles[i][j]) > 90) {throw std::invalid_argument ("Only capital letters from A to Z allowed");}

            ///search for children
            Index nodechild;
            for (Index c : trie[current.pos()].children) {if(needles[i][j]==trie[c.pos()].character) {nodechild = c;}}
            

            ///if child exists
            if(!(nodechild.isNull()))
            {
                current.pos() = nodechild.pos();

                if(needles[i].size()-1 == j) {trie[nodechild.pos()].needle_indices.push_back(i);} ///mark as terminal
            }

            ///if there is no child (nodechild = maxsize) -> create new node
            else
            {
                ACNode newnode;
                newnode.character = needles[i][j];
                newnode.parent_link.pos() = current.pos();
                newnode.depth = j+1;

                if(needles[i].size()-1 == j) {newnode.needle_indices.push_back(i);} ///mark as terminal

                trie.push_back(newnode);
                
                trie[current.pos()].children.push_back(trie.size()-1); ///child of current is latest newnode
                current.pos() = trie.size()-1; ///continue at created newnode
            }
        }
    }
    this->trie=trie;
  
    ///create queue in breadth first search order
    std::vector<Index> queue{};
    for(unsigned i=0; i<trie.size(); ++i)
    {
        for(Index it : trie[i].children) {queue.push_back(it.pos());}
    }

    ///suffix links
    for(unsigned i=0; i<queue.size(); ++i) ///root does not have any links
    {
        if(trie[queue[i].pos()].parent_link.pos() == 0) {trie[queue[i].pos()].suffix_link.pos() = 0;} ///root´s children get suffix link to root

        else
        {
            Index followmyjourney = trie[trie[queue[i].pos()].parent_link.pos()].suffix_link; ///follow parents suffix link

            ///search for children at parents suffix link
            Index suffix;
            for (Index c : trie[followmyjourney.pos()].children)
            {
                if(trie[queue[i].pos()].character==trie[c.pos()].character) {suffix = c;} ///if a character matches current nodes character -> finds Index
                else {trie[queue[i].pos()].suffix_link.pos() = 0;} ///if not -> link to root
            }

            if (!(suffix.isNull())) {trie[queue[i].pos()].suffix_link = suffix;} ///if Index is valid -> create link
        }
    }

    ///output links
    for(unsigned i=0; i<queue.size(); ++i)
    {
        Index outputjourney = trie[queue[i].pos()].suffix_link; ///follow currents suffix link

        if(outputjourney.pos() != 0) ///not at root
        {
            if(trie[outputjourney.pos()].needle_indices.size() != 0) {trie[queue[i].pos()].output_link = outputjourney;} ///terminal node -> output

            else
            {
                ///follow suffix link until terminal node or root is reached
                while(outputjourney.pos() != 0)
                {
                    outputjourney = trie[outputjourney.pos()].suffix_link;

                    if(trie[outputjourney.pos()].needle_indices.size() != 0) {trie[queue[i].pos()].output_link = outputjourney;} ///output
                }
            }
        }
    }
    this->trie=trie;
}


void ACTrie::setQuery(const std::string& haystack)
{
    this->haystack = haystack;
    this->currindex.pos() = 0; ///Index for position in trie
    this->queryindex.pos() = 0; ///Index for position in haystack/query
}


bool ACTrie::next(std::vector<Hit>& hits)
{
    hits.clear();

    for(unsigned i=queryindex.pos(); i<haystack.size(); ++i)
    {
        ///search for children
        Index nodechild;
        for (Index c : trie[currindex.pos()].children) {if(haystack[i]==trie[c.pos()].character) {nodechild = c;}}

        if(!(nodechild.isNull())) ///nodechild exists
        {
            ///node is terminal
            if(trie[nodechild.pos()].needle_indices.size() != 0)
            {
                ///create hit for every needle that´s found in node
                for(unsigned j=0; j<trie[nodechild.pos()].needle_indices.size(); ++j)
                {
                    Hit h;
                    h.index = trie[nodechild.pos()].needle_indices[j];
                    h.pos = i+1 - trie[nodechild.pos()].depth;
                    hits.push_back(h);
                }
               
                ///node is terminal and also has valid output link
                if(!(trie[nodechild.pos()].output_link.isNull()))
                {
                    ///create hit for every needle that´s found in node
                    for(unsigned j=0; j<trie[trie[nodechild.pos()].output_link.pos()].needle_indices.size(); ++j)
                    {
                        Hit h;
                        h.index = trie[trie[nodechild.pos()].output_link.pos()].needle_indices[j];
                        h.pos = i+1 - trie[trie[nodechild.pos()].output_link.pos()].depth;
                        hits.push_back(h);
                    }
                }
                
                ///node is terminal and has no children -> set currindex back to root
                if (trie[nodechild.pos()].children.size() == 0)
                {
                    currindex.pos()=0;
                    queryindex.pos() = i+1;
                    return true;
                }
                
                ///node is terminal but still has children -> continue at current node
                else
                {
                    currindex.pos() = nodechild.pos();
                    queryindex.pos() = i+1;
                    return true;
                }
            }
            
            ///node is not terminal but has output link
            else
            {
                if(!(trie[nodechild.pos()].output_link.isNull())) ///output link is valid
                {
                    Hit h;
                    h.index = trie[nodechild.pos()].output_link.pos();
                    h.pos = trie[nodechild.pos()].depth;
                    hits.push_back(h);
                    currindex.pos()=nodechild.pos();
                    queryindex.pos()=i+1;
                    return true;
                }
                
                ///node is not terminal and has no output link -> continue search at current node in next loop
                currindex.pos()=nodechild.pos();
            }
        }
        ///no child found that matches -> start again at root in next loop
        else
        {
            currindex.pos()=0;
        }
    }
    
    hits.clear();
    return false;
}


ACTrie::ACTrie(const std::vector<std::string>& needles)
        //:    needles{needles}
{

    if (needles.empty()) {throw std::logic_error("At least 1 needle required");}

    ACNode root;
    root.character = '0';
    std::vector<ACNode> trie{};

    buildTree(root, trie, needles);
}


std::string ACTrie::getTree() const
{
    std::string h;
    h += '0';

    for(unsigned i=1; i<trie.size(); ++i) ///starting at first child
    {
        h += '(';
        h += trie[i].character;

        if(trie[i].children.size() == 0) ///closing bracets if current does not have children to add to string
        {
            h += ')';
            
            if(trie[i].needle_indices.size() != 0)
            {
                for(unsigned j=0; j<trie[i].depth-1; ++j) {h += ')';} ///adds as many closing brackets as there are open ones
            }
        }
    }
    return h;
}
