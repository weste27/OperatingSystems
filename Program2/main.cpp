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
#include <MMU.h>
#include "Process.h"
#include "MemoryAllocator.h"
#include "PageTableManager.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    mem::MMU *mem = new mem::MMU(64); 
    MemoryAllocator *ma = new MemoryAllocator(64, mem);
   PageTableManager ptm(mem, ma); 
   
    Process p("/media/sf_Ubuntu_Files/Program2/trace1v.txt", *mem, *ma, ptm);
    p.Run(); 
    
    return 0;
}

