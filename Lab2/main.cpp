/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: crjoyce
 *
 * Created on April 6, 2018, 2:42 PM
 */

#include <cstdlib>

#include "Process.h"
using namespace std;
#include <iostream>

/*
 * 
 */
int main(int argc, char** argv) {
    
    //string s = "/home/crjoyce/Desktop/trace1.txt";
    string s(argv[1]); 
    Process p(s); 
    return 0;
}

