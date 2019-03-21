// Author: Yiheng Cen Feng
// Date of Creation: March 5th, 2019
// File's Title: sequence_map.h
// Description: a class that creates object named SequenceMap.

#ifndef SEQUENCE_MAP_H
#define SEQUENCE_MAP_H

#include <iostream>
#include <string>
#include <vector>

class SequenceMap{
  public:
    //copy-constructor
    SequenceMap(const SequenceMap &rhs) = default;
    
    //copy-assignment
    SequenceMap& operator=(const SequenceMap &rhs) = default;
   
    //move-constructor
    SequenceMap(SequenceMap &&rhs) = default;

    //move-assignment
    SequenceMap& operator=(SequenceMap &&rhs) = default;
    
    //destructor
    ~SequenceMap() = default;

	//End of Big Five
    
    
    //Two parameters constructor
    SequenceMap(const std::string &a_rec_seq, const std::string &an_enz_acro){
        recognition_sequence_ = a_rec_seq;
        enzyme_acronym_.push_back(an_enz_acro);
    }
    
    //String comparison between two recognition sequences
    //Post-condition: return true if the object's recognition sequence < rhs's recognition sequence, false otherwise
    bool operator<(const SequenceMap &rhs) const {
        return (recognition_sequence_ < rhs.recognition_sequence_);
    }
    
    //Overload operator <<
    //Allow user to print the contents of a sequence_map object
    friend std::ostream &operator<<(std::ostream &out, const SequenceMap &seq_map){
        out << seq_map.recognition_sequence_ << " ";
        for(size_t i = 0; i < seq_map.enzyme_acronym_.size(); ++i)
            out << seq_map.enzyme_acronym_[i] << " ";
        return out;
    }


    //Merges the other_sequence.enzyme_acronym_ with the object’s enzyme_acronym_
    //Pre-condition: the object's recognition_seuqnece_ and the other_sequence's recognition_ sequence_ are equal
    //Post-condition: the other_sequence.enzyme_acronym_ is appended to the object’s enzyme_acronym_
    void Merge(const SequenceMap &other_sequence){
        for(size_t i = 0; i < other_sequence.enzyme_acronym_.size(); ++i)
            enzyme_acronym_.push_back(other_sequence.enzyme_acronym_[i]);
    }
    
    // return recognition_sequence_
    std::string getRecognitionSequence(){
        return recognition_sequence_;
    }
    
    // Print the associated enzyme acronym
    void printEnzymeAcronym(){
        for(size_t i = 0; i < enzyme_acronym_.size(); ++i)
            std::cout<<enzyme_acronym_[i]<<" ";
        std::cout<<std::endl;
    }

  private:
    std::string recognition_sequence_;
    std::vector<std::string> enzyme_acronym_;
};
	
#endif
