/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MemoryAllocator.cpp
 * Author: crjoyce
 * 
 * Created on April 13, 2018, 1:50 PM
 */

#include "MemoryAllocator.h"
#include <list>

MemoryAllocator::MemoryAllocator(uint32_t number_of_frames) {
    free_list.resize(number_of_frames*0x10000);
    if(number_of_frames == 0){
        free_list.resize(6); 
        free_list.at(0) = 0xFF; 
        free_list.at(1) = 0xFF; 
        free_list.at(2) = 0xFF; 
        free_list.at(3) = 0xFF;
        free_list.at(4) = 0; 
        free_list.at(5) = 0; 
    }
    else{
        for(int i = 0; i<=(number_of_frames-1)*0x10000; i += 0x10000){
            uint32_t next = i + 0x10000;
            for(int j = 0; j<4; j++){
                uint32_t temp = (0x000000FF << j*8)&next;
                temp = temp >>j*8; 
                free_list.at(i+j) = temp; 
                
            }
            if(i == (number_of_frames-1)*0x10000){
                free_list.at(i) = 0xFF; 
                free_list.at(i+1) = 0xFF;
                free_list.at(i+2) = 0xFF;
                free_list.at(i+3) = 0xFF;

            }
           
             
        }
    }
    //page frames total
    //free_list.at(5) = number_of_frames -1; 
    set_address(4, number_of_frames-1);
    //page frames free
    //free_list.at(6) = number_of_frames -1; 
    set_address(8, number_of_frames-1); 
    
}

bool MemoryAllocator::AllocatePageFrames(uint32_t count, std::vector<uint32_t> &page_frames){
    uint32_t header_addr = 0;
    if(free_list.at(6) < count){ 
        return false; 
    }
    //set_address(8, get_address(8)-count); 
    free_list.at(6) -= count; 
    uint32_t i = get_free_list_head(); 
    uint32_t j = get_free_list_head(); 
    for(i; i<((count)*0x10000)+j; i += 0x10000){
        uint32_t addr = i; 
        page_frames.push_back(addr); 
        header_addr = get_address(addr); 
    }
    set_address(0, header_addr); 
    for(j; j<count*0x10000; j+= 0x10000){
        free_list.at(i) = 0; 
    }
    return true; 
}

bool MemoryAllocator::FreePageFrames(uint32_t count, std::vector<uint32_t> &page_frames){
    if(count > page_frames.size()){
        return false; 
    }
    //set_address(8, get_address(8)+count); 
    free_list.at(6) += count; 
    uint32_t addr = 0; 
    uint32_t part_addr = 0; 
    for(int i = 0; i<count; i++){
        addr = page_frames.back(); 
        page_frames.pop_back(); 
        set_address(addr, addr+0x10000); 
        if(addr == (get_address(4)*0x10000)){
            set_address(addr, 0xFFFFFFFF); 
        }
    }
    set_address(0, addr); 
    return true; 
}

uint32_t MemoryAllocator::get_free_list_head(){
    return get_address(0); 
}

std::string MemoryAllocator::print(){
    uint32_t start = get_free_list_head(); 
    std::ostringstream ret_stream;  
    while(start != 0xFFFFFFFF){
        ret_stream << std::hex << " " << start;  
        start = get_address(start); 
    }
    return ret_stream.str(); 
}

uint32_t MemoryAllocator::get_page_frames_free(){
    //return get_address(8); 
    return free_list.at(6); 
}

uint32_t MemoryAllocator::get_page_frames_total(){
    //return get_address(4); 
    return free_list.at(5); 
}

uint32_t MemoryAllocator::get_address(uint32_t start){
    uint32_t addr = 0; 
    for(int i = 0; i<4; i++){
        uint32_t temp = 0; 
        temp = free_list.at(start+i);
        addr = addr|(temp << i*8);
    }
    return addr; 
}

void MemoryAllocator::set_address(uint32_t addr, uint32_t set){
    uint32_t part_addr; 
    for(int j = 0; j<4; j++){
        part_addr = (set&(0xFF << 8*j)) >> 8*j;
        free_list.at(addr+j) = part_addr;  
    }
   
}
 



MemoryAllocator::~MemoryAllocator() {
}




