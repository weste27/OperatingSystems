/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PageTableManager.cpp
 * Author: crjoyce
 * 
 * Created on May 9, 2018, 4:20 PM
 */

#include "PageTableManager.h"
#include "MemoryAllocator.h"
#include <iostream>

PageTableManager::PageTableManager(mem::MMU *&m, MemoryAllocator *&a) {
    //allocate page for the kernel page table 
    mem::Addr loc = a->get_free_list_head();
    a->AllocatePageFrames(1, closed_list, 0);
     
    alloc = a; 
    //get physical address of the page table
    //create new pmcb on the heap, point pmcb to it
    kPT = new mem::PageTable();
    //a->
    for (mem::Addr i = 0; i < m->get_frame_count(); ++i) {
      kPT->at(i) = 
              (i << mem::kPageSizeBits) |mem:: kPTE_PresentMask | mem::kPTE_WritableMask;
    }
    m->put_bytes(loc, mem::kPageTableSizeBytes, kPT); 
    pmcb = new mem::PMCB(loc); 
    //set the pmcb of the given mmu to that pmcb 
    m->set_PMCB(*pmcb); 
    m->enter_virtual_mode(); 
}

PageTableManager::PageTableManager(const PageTableManager& orig) {
}

PageTableManager::~PageTableManager() {
    delete pmcb; 
    delete kPT; 
    pmcb = nullptr;
    kPT = nullptr; 
}

mem::Addr PageTableManager::CreateProcessPT(mem::MMU *&m, MemoryAllocator *&a, 
        mem::PageTable *&pPT, mem::PMCB *&new_pmcb){
    m->set_PMCB(*pmcb); 
    //allocate a page in the MMU

    mem::Addr hail_mary = 0; 
    mem::Addr loc = a->get_free_list_head(); 
    a->AllocatePageFrames(1, closed_list, loc); 
    pPT = new mem::PageTable();
    hail_mary = pmcb->next_vaddress; 
    mem::Addr iter_loc = loc; 
    mem::Addr pt_offset = (hail_mary >> mem::kPageSizeBits) & mem::kPageTableIndexMask;
    for(int i = 0; i<m->get_frame_count(); i++){
        pPT->at(pt_offset+i) = iter_loc | mem::kPTE_PresentMask | mem::kPTE_WritableMask;
        iter_loc += mem::kPageSize; 
    }
    m->put_bytes(loc, mem::kPageTableSizeBytes, pPT);
    new_pmcb = new mem::PMCB(loc);
    return loc; 
    
   
    
}

void PageTableManager::Map(mem::MMU *&m, int count, mem::Addr Vaddr){
    mem::Addr Paddr = 0; 
    //std::cout << "1\n"; 
    m->set_PMCB(*pmcb); 
    //std::cout << "2\n"; 
    //m->ToPhysical(Vaddr, Paddr, true); 
    //std::cout << "3\n"; 
    alloc->AllocatePageFrames(count, closed_list, Vaddr); 
    //std::cout << "4\n"; 
    //allocate count pages at the physical address
    
}

void PageTableManager::Permission(mem::MMU *&m, int count, int state, mem::Addr Vaddr){
    m->set_PMCB(*pmcb); 
    mem::Addr Paddr = 0; 
    m->ToPhysical(Vaddr, Paddr, false); 
    std::vector<uint32_t> page; 
    page.resize(mem::kPTE_Writable); 
    for(int i = 0; i<count; i++){
        m->get_bytes(&page, Vaddr, mem::kPTE_Writable); 
        page.at(mem::kPTE_Writable-1) = state; 
        m->put_bytes(Vaddr, mem::kPTE_Writable, &page); 
        Vaddr += 0x10000; 
    }
}

