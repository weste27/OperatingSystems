/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Process.h
 * Author: crjoyce
 *
 * Created on April 6, 2018, 2:43 PM
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <string> 
#include <fstream>
#include <vector>
#include <stdint.h>

class Process {
public:
    //constructor - takes in a string, file path, open a filestream, call the run method
    Process(std::string &file);
    
    //copy/move constructor - do not allow 
    Process(const Process& orig) = delete;
    Process(const Process&& orig) = delete; 
    Process& operator=(const Process&) = delete; 
    Process&& operator=(const Process&&) = delete; 
    
    //destructor - close the filestream
    virtual ~Process();
    
    //run - loops through the file, printing out each line and line number. Takes the first word of each line
        //and sends it to the proper function, print to print, diff to diff, replicate to replicate, duplicate to duplicate, and store to store 
    void run();
    //duplicate - inserts the first argument into the uint8_t vector, the second argument times, at the third arguments location
    void duplicate(std::istringstream &); 
    //print - prints out the as many elements as the first arg from the uint8_t array, starting at the given memory location from the string stream. 
        //Loops and adds \n's when the loop has gone a multiple of 16 times. 
    void print(std::istringstream &); 
    //replicate - fills a vector with the number given in the line, as many times as the number that follows it, and then loops through the uint8_t vector starting at the
        //given memory location, replacing whatever element is in the vector with the number given by the command
    void replicate(std::istringstream &); 
    //diff - fills a vector with the given numbers from the string, then locates the given memory and checks to see if the elements starting 
        //at that memory are the same as the given values 
    void diff(std::istringstream &); 
    //store - takes the first string from the istringstream, turns it into an unsigned int, and puts it into the uint8_t vector at the given memory location
    void store(std::istringstream &); 
private:
    //input - file stream that opens the given filepath from the constructor
    std::ifstream input; 
    //mem - vector that stores the given values during the program. 
    std::vector<uint8_t> mem; 
    

};

#endif /* PROCESS_H */

