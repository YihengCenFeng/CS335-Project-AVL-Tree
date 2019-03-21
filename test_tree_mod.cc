// Author: Yiheng Cen Feng
// Date of Creation: March 5th, 2019
// File's Title: test_tree_mod.cc
// Description: use the parser to create an AVL tree and then test the modified tree.
// Main file for Part2(c) of Homework 2.

#include "avl_tree_modified.h"
#include "sequence_map.h"

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

namespace {

// Check the opening of file.
// If failed, exist.
void CheckFile(const string &filename){
    ifstream in_file(filename);
    if(in_file.fail()){
        cerr<<"File opening failed!"<<endl;
        exit(1);
    }
}

// @db_line: a line from an input database.
// Return a string of extracted portion from the db_line.
string ExtractFromLine(string &db_line){
    size_t break_point = db_line.find('/');
    string extract = db_line.substr(0,break_point);
    db_line = db_line.substr(break_point+1, db_line.length());
    return extract;
}

// @db_filename: an input database filename.
// @a_tree: an input tree of the type TreeType. It is assumed to be empty.
// Create an AVL tree.
template <typename TreeType>
void ConstructTree(const string &db_filename,TreeType &a_tree){
    CheckFile(db_filename);
    ifstream in_file(db_filename);
    
    string db_line, enz_acro, reco_seq;
    
    //skip over the header
    for(size_t i = 0; i < 10; ++i){
        getline(in_file, db_line);
    }
    
    while(
          getline(in_file, db_line)){
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
}

// @seq_filename: an input sequences filename.
// @a_tree: an input tree of the type TreeType. It is assumed to be empty.
// Test the function find().
template <typename TreeType>
void TestFind(const string &seq_filename, TreeType &a_tree){
    CheckFile(seq_filename);
    ifstream seq_file(seq_filename);
    string seq_line;
    int find_recursive_call = 0;
    int successful_query = 0;
    float number_of_query = 0;
    while(getline(seq_file, seq_line)){
        successful_query += a_tree.find(seq_line, find_recursive_call);
        ++number_of_query;
    }
    seq_file.close();
    
    cout<<"4a: "<<successful_query<<endl;
    cout<<"4b: "<<find_recursive_call/number_of_query<<endl;
}

// @seq_filename: an input sequences filename.
// @a_tree: an input tree of the type TreeType. It is assumed to be empty.
// Test the function remove().
template <typename TreeType>
void TestRemove(const string &seq_filename, TreeType &a_tree){
    CheckFile(seq_filename);
    ifstream sequence_file(seq_filename);
    string seq_line;
    int remove_recursive_call = 0;
    int successful_remove = 0;
    int counter = 0;
    float remove_call = 0;
    while(getline(sequence_file, seq_line)){
        if(counter % 2 == 0){
            successful_remove += a_tree.remove(seq_line, remove_recursive_call);
            ++remove_call;
        }
        ++counter;
    }
    sequence_file.close();
    
    cout<<"5a: "<<successful_remove<<endl;
    cout<<"5b: "<<remove_recursive_call/remove_call<<endl;
}
    
// @db_filename: an input database filename.
// @seq_filename: an input sequences filename.
// @a_tree: an input tree of the type TreeType. It is assumed to be
//  empty.
template <typename TreeType>
void TestTree(const string &db_filename, const string &seq_filename, TreeType &a_tree) {
    
    ConstructTree(db_filename, a_tree);
    
    cout<<"2: "<<a_tree.numberOfNodes()<<endl;
    cout<<"3a: "<<a_tree.averageDepth()<<endl;
    cout<<"3b: "<<a_tree.averageDepthRatio()<<endl;
    TestFind(seq_filename, a_tree);
    TestRemove(seq_filename, a_tree);
    cout<<"6a: "<<a_tree.numberOfNodes()<<endl;
    cout<<"6b: "<<a_tree.averageDepth()<<endl;
    cout<<"6c: "<<a_tree.averageDepthRatio()<<endl;
}

}  // namespace

int
main(int argc, char **argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <databasefilename> <queryfilename>" << endl;
    return 0;
  }
  const string db_filename(argv[1]);
  const string seq_filename(argv[2]);
  cout << "Input file is " << db_filename << ", and sequences file is " << seq_filename << endl;
  AvlTree<SequenceMap> a_tree;
  TestTree(db_filename, seq_filename, a_tree);

  return 0;
}
