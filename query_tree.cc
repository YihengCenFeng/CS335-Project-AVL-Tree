// Author: Yiheng Cen Feng
// Date of Creation: March 5th, 2019
// File's Title: query_tree.cc
// Description: use the parser to create an AVL tree and then allow the user to query it using a recognition sequence.
// Main file for Part2(a) of Homework 2.

#include "avl_tree.h"
#include "sequence_map.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

namespace {
    
// @db_line: a line from an input database
// Return a string of extracted portion from the db_line
string ExtractFromLine(string &db_line){
    size_t break_point = db_line.find('/');
    string extract = db_line.substr(0,break_point);
    db_line = db_line.substr(break_point+1, db_line.length());
    return extract;
}
    
// @db_filename: an input filename.
// @a_tree: an input tree of the type TreeType. It is assumed to be
//  empty.
// Construct a tree of the type Treetype
template <typename TreeType>
void QueryTree(const string &db_filename, TreeType &a_tree) {
    
    ifstream in_file(db_filename);
    if(in_file.fail()){
        cerr<<"File opening failed!"<<endl;
        exit(1);
    }
        
    string db_line, enz_acro, reco_seq;
        
    //skip over the header
    for(size_t i = 0; i < 10; ++i){
        getline(in_file, db_line);
    }
    
    while(getline(in_file, db_line)){
        if(db_line.empty())
            continue;
        enz_acro = ExtractFromLine(db_line);
        while(db_line.length() > 2){
            reco_seq = ExtractFromLine(db_line);
            SequenceMap new_sequence_map(reco_seq, enz_acro);
            a_tree.insert(new_sequence_map);
        }
    }
    in_file.close();
    
    vector<string> reco_seq_input;
    int i = 0;
    while(i < 3){
        cin>>reco_seq;
        reco_seq_input.push_back(reco_seq);
        i++;
    }
    
    for(int j = 0; j < 3; ++j)
        a_tree.findRecoSeq(reco_seq_input[j]);
}

}  // namespace

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <databasefilename>" << endl;
        return 0;
    }
    const string db_filename(argv[1]);
    cout << "Input filename is " << db_filename << endl;
    AvlTree<SequenceMap> a_tree;
    QueryTree(db_filename, a_tree);
    
    return 0;
}
