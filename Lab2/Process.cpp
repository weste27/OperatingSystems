/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Process.cpp
 * Author: crjoyce
 * 
 * Created on April 6, 2018, 2:43 PM
 */

#include "Process.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdint.h>

Process::Process(std::string &file) {
    input.open(file, std::ios::in); 
    run(); 
}

Process::Process(const Process& orig) {
}

Process::~Process() {
    input.close(); 
}

void Process::run(){
    int lineNum = 1; 
    for(std::string line; getline(input, line);){
        std::cout << std::dec << lineNum << ": " << line << "\n"; 
        lineNum++; 
        std::istringstream linestream(line); 
        std::string first;
        linestream >> first; 
        if(first == "memsize"){
            uint64_t size; 
            linestream >> std::hex >> size;
            mem.resize(size, 0); 
            
        }
        else if(first == "#"){
            //do nothing, the line has already been printed
        }
        else if(first == "diff"){
            diff(linestream); 
        }
        else if(first == "store"){
            store(linestream); 
        }
        else if(first == "replicate"){
            replicate(linestream); 
        }
        else if(first == "print"){
            print(linestream);  
        }
        else if(first == "duplicate"){
            duplicate(linestream); 
        }
        else{
            
        }
        
        
    }
        
}

void Process::print(std::istringstream &linestream){
    uint32_t count;
    uint32_t address; 
    linestream >> std::hex >> count; 
    linestream >> std::hex >> address; 
    std::cout << std::hex << address << "\n"; 
    for(int i = 0; i<count; i++){
        std::cout << std::setw(2) << std::setfill('0') << std::hex << unsigned(mem[i+address]) << " ";
        if((i+1)%16 == 0){
            std::cout << "\n"; 
        }
    }
    std::cout << "\n"; 
    std::dec; 
}

void Process::replicate(std::istringstream &linestream){
    uint16_t value; 
    uint32_t count; 
    uint16_t address;

    linestream >> std::hex >> value; 
    linestream >> std::hex >> count; 
    linestream >> std::hex >> address;
    
    
    //std::cout << test << "\n"; 

    for(int i = 0; i<count; i++){
        mem[address+i] = value; 
    }
}

void Process::diff(std::istringstream &linestream){
    std::vector<uint32_t> args;
    uint32_t temp; 
    int size = 0; 
    while(linestream >> std::hex >> temp){
        args.push_back(temp);
        size++; 
    }
    uint32_t address = args.at(size-1); 
    for(int i = 0; i<size-1; i++){
        if(args[i] != mem[address]){
            //print error message
           std::cerr << "Expected value at " << std::hex << address << " was " << args[i] << ". "
                    << "Value found was " << mem[address] << "\n" << std::dec; 
        }
        address++; 
    }
}

void Process::store(std::istringstream &linestream){
    uint16_t temp; 
    std::vector<uint16_t> args; 
    int size = 0; 
    while(linestream >> std::hex >> temp){
        args.push_back(temp); 
        size++; 
    }
    uint16_t address = args.at(size-1);
    for(int i = 0; i<size-1; i++){
        mem[address] = args[i]; 
        address++; 
    }
}

void Process::duplicate(std::istringstream &linestream){
    uint16_t count; 
    uint32_t source; 
    uint32_t dest; 

    linestream >> std::hex >> count; 
    linestream >> std::hex >> source; 
    linestream >> std::hex >> dest; 

    std::vector<uint16_t> source_mem;
    for(int i = 0; i<count; i++){
        uint16_t temp = mem[source + i]; 
        source_mem.push_back(temp);
    }

    for(int i = 0; i<count; i++){
        mem[dest+i] = source_mem[i]; 
    }
}


