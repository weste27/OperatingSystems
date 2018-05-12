/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PageTableManager.h
 * Author: crjoyce
 *
 * Created on May 9, 2018, 4:20 PM
 */

#ifndef PAGETABLEMANAGER_H
#define PAGETABLEMANAGER_H

#include <MMU.h>
#include <cstdint>
#include <vector>
#include "MemoryAllocator.h"

class PageTableManager {
public:
    /*in the constructor: use memory allocator
     to allocate a page on the MMU to store a kernel
     * page table. Initialize pmcb to use that page table
     * set the pmcb of the MMU
    */
    PageTableManager(mem::MMU *m, MemoryAllocator *a);
    PageTableManager(const PageTableManager& orig);
    virtual ~PageTableManager();
    mem::Addr CreateProcessPT(mem::MMU *m, MemoryAllocator *a, 
    mem::PageTable *pPT, mem::PMCB *new_pmcb); 
    void Map(mem::MMU *m, int count, mem::Addr Vaddr); 
    void Permission(mem::MMU *m, int count, int state, mem::Addr Vaddr); 
private:
    std::vector<uint32_t> closed_list; 
    mem::PMCB *pmcb; 
    mem::PageTable *kPT;
    MemoryAllocator *alloc; 

};

#endif /* PAGETABLEMANAGER_H */

