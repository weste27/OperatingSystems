/*  MemoryAllocator - allocate pages in memory
 * 
 * File:   MemoryAllocator.cpp
 * Author: Mike Goss <mikegoss@cs.du.edu>
 */

#include "MemoryAllocator.h"

#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>


MemoryAllocator::MemoryAllocator(uint32_t page_frame_count, mem::MMU *mem) 
{
    memory = mem; 
  if (page_frame_count <= 1) {
    throw std::runtime_error("page_frame_count must be > 1");
  }
  
  // Set free list empty
  uint32_t free_list_head = kEndList;
  
  // Add all page frames except 0 to free list
  uint32_t frame = memory->get_frame_count() - 1;
  
  while (frame > 0) {
    //memcpy(&memory[frame], &free_list_head, sizeof(uint32_t));
    free_list_head = frame * 0x10000;
    frame -= 1;
  }
  
  // Initialize list info in page 0
  set_free_list_head(free_list_head);
  set_page_frames_free(page_frame_count - 1);
  set_page_frames_total(page_frame_count);
}

bool MemoryAllocator::AllocatePageFrames(uint32_t count, 
                                         std::vector<uint32_t> &page_frames, 
        uint32_t free_list_head) {
  // Fetch free list info
    if(free_list_head == 0){
        free_list_head = get_free_list_head(); 
    }
  uint32_t page_frames_free = get_page_frames_free();
  //uint32_t free_list_head = get_free_list_head();
  
  // If enough pages available, allocate to caller
  if (count <= page_frames_free) {  // if enough to allocate
    while (count-- > 0) {
      // Return next free frame to caller
      uint32_t frame = free_list_head;
      page_frames.push_back(frame);
      
      // De-link frame from head of free list
      memory->get_bytes(&free_list_head, free_list_head, sizeof(uint32_t));
      --page_frames_free;
      
      // Clear page frame to all 0
      //memset(&memory[frame], 0, kPageSize);
      int src = 0; 
      int *src_pointer = &src; 

      //memory->put_bytes(free_list_head, 0x10000, src_pointer); 
    }
    
    // Update free list info
    set_free_list_head(free_list_head);
    set_page_frames_free(page_frames_free);

    return true;
  } else {
    return false;  // do nothing and return error
  }
}

bool MemoryAllocator::FreePageFrames(uint32_t count,
                                     std::vector<uint32_t> &page_frames) {
  // Fetch free list info
  uint32_t page_frames_free = get_page_frames_free();
  uint32_t free_list_head = get_free_list_head();

  // If enough to deallocate
  if(count <= page_frames.size()) {
    while(count-- > 0) {
      // Return next frame to head of free list
      uint32_t frame = page_frames.back();
      page_frames.pop_back();
      //memcpy(&memory[frame], &free_list_head, sizeof(uint32_t));
      //memory->get_bytes(frame, sizeof(uint32_t), &free_list_head);
      memory->get_bytes(&free_list_head, frame, sizeof(uint32_t)); 
      free_list_head = frame;
      ++page_frames_free;
    }

    // Update free list info
    set_free_list_head(free_list_head);
    set_page_frames_free(page_frames_free);

    return true;
  } else {
    return false; // do nothing and return error
  }
}

std::string MemoryAllocator::FreeListToString(void) const {
  std::ostringstream out_string;
  
  uint32_t next_free = get_free_list_head();
  
  while (next_free != kEndList) {
    out_string << " " << std::hex << next_free;
    memory->get_bytes(&next_free, next_free, sizeof(uint32_t)); 
    //memcpy(&next_free, &memory[next_free], sizeof(uint32_t));
  }
  
  return out_string.str();
}

uint32_t MemoryAllocator::get_page_frames_free() const {
  uint32_t page_frames_free;
  memory->get_bytes(&page_frames_free, kPageFramesFree, sizeof(uint32_t)); 
  //memcpy(&page_frames_free, &memory[kPageFramesFree], sizeof(uint32_t));
  return page_frames_free;
}

uint32_t MemoryAllocator::get_page_frames_total() const {
  uint32_t page_frames_total;
  //memcpy(&page_frames_total, &memory[kPageFramesTotal], sizeof(uint32_t));
  memory->get_bytes(&page_frames_total, kPageFramesTotal, sizeof(uint32_t)); 
  return page_frames_total;
}

uint32_t MemoryAllocator::get_free_list_head() const{
  uint32_t free_list_head;
  //memcpy(&free_list_head, &memory[kFreeListHead], sizeof(uint32_t));
  memory->get_bytes(&free_list_head, kFreeListHead, sizeof(uint32_t)); 
  return free_list_head;
}

void MemoryAllocator::set_page_frames_free(uint32_t page_frames_free) {
  //memcpy(&memory[kPageFramesFree], &page_frames_free, sizeof(uint32_t));
  memory->put_bytes(kPageFramesFree, sizeof(uint32_t), &page_frames_free); 
}

void MemoryAllocator::set_page_frames_total(uint32_t page_frames_total) {
  //memcpy(&memory[kPageFramesTotal], &page_frames_total, sizeof(uint32_t));
  memory->put_bytes(kPageFramesTotal, sizeof(uint32_t), &page_frames_total);  
}

void MemoryAllocator::set_free_list_head(uint32_t free_list_head) {
  //memcpy(&memory[kFreeListHead], &free_list_head, sizeof(uint32_t));
  memory->put_bytes(kFreeListHead, sizeof(uint32_t), &free_list_head); 
}
