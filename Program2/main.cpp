/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: crjoyce
 *
 * Created on May 9, 2018, 3:55 PM
 */

#include <cstdlib>
#include "Process.h"
#include "MemoryAllocator.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    mem::MMU mem(); 
    
    mem::PMCB pmcb(); 
    mem.set_PMCB(pmcb); 
    mem.enter_virtual_mode(); 
    
    Process p("string", mem); 
    
    return 0;
}

