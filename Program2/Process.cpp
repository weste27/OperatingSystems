/*
 * Process implementation 
 * 
 * File:   Process.cpp
 * Author: Mike Goss <mikegoss@cs.du.edu>
 * 
 */

#include "Process.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>


using std::cin;
using std::cout;
using std::cerr;
using std::getline;
using std::istringstream;
using std::string;
using std::vector;

#include <iostream>

//using mem; 

Process::Process(string file_name_, mem::MMU &mem, MemoryAllocator &alloc,
        PageTableManager &ptm)  
: file_name(file_name_), line_number(0), memory(&mem), all(&alloc) {
    mem::Addr test = ptm.CreateProcessPT(memory, all, pt, pmcb);
    //std::cout << "working"; 
    memory->set_PMCB(*pmcb);
    this->ptm = &ptm; 
    //use page table manager to create a process page table
    //set the pmcb of mem to one that uses that page table
    
  // Open the trace file.  Abort program if can't open.
  trace.open(file_name, std::ios_base::in);
  
  if (!trace.is_open()) {
    cerr << "ERROR: failed to open trace file: " << file_name << "\n";
    exit(2);
  }
}

Process::~Process() {
//    delete pt; 
//    delete pmcb; 
//    delete mem; 
//    mem = nullptr; 
//    pt = nullptr; 
//    pmcb = nullptr; 
  trace.close();
}

void Process::Run(void) {
  // Read and process commands
  string line;                // text line read
  string cmd;                   // command from line
  vector<uint32_t> cmdArgs;

//  while(cmd != "memsize"){
//      ParseCommand(line, cmd, cmdArgs);
//      if(cmd == "map"){
//          size = CmdMap(line, cmd, cmdArgs); 
//      }
//
//  }
   // arguments from line
  //mem::MMU memory->size); 
  // Select the command to execute
  while (ParseCommand(line, cmd, cmdArgs)) {
      if(cmd == "map"){
          //std::cout << line; 
          
          //map is throwing a page fault exception
          //linked to creation of the page table 
          CmdMap(line, cmd, cmdArgs); 
      }
      else if (cmd == "diff") {
      CmdDiff(line, cmd, cmdArgs);  // get and compare multiple bytes
    } else if (cmd == "store") {
      CmdStore(line, cmd, cmdArgs);      // put bytes
    } else if (cmd == "replicate") {
      CmdRepl(line, cmd, cmdArgs);     // fill bytes with value
    } else if (cmd == "duplicate") {
      CmdDupl(line, cmd, cmdArgs);     // copy bytes to dest from source
    } else if (cmd == "print") {
      CmdPrint(line, cmd, cmdArgs);     // dump byte values to output
    } else if(cmd == "permission"){
        CmdPermiss(line, cmd, cmdArgs); 
    }else if (cmd != "#") {
      cerr << "ERROR: invalid command\n";
      exit(2);
    }
  }
}

bool Process::ParseCommand(
    string &line, string &cmd, vector<uint32_t> &cmdArgs) {
  cmdArgs.clear();
  line.clear();
  
  // Read next line
  if (std::getline(trace, line)) {
    ++line_number;
    cout << std::dec << line_number << ":" << line << "\n";
    
    // No further processing if comment or empty line
    if (line.size() == 0 || line[0] == '#') {
      cmd = "#";
      return true;
    }
    
    // Make a string stream from command line
    istringstream lineStream(line);
    
    // Get command
    if (!(lineStream >> cmd)) {
      // Blank line, treat as comment
      cmd = "#";
      return true;
    }
    
    // Get arguments
    uint32_t arg;
    while (lineStream >> std::hex >> arg) {
      cmdArgs.push_back(arg);
    }
    return true;
  } else if (trace.eof()) {
      return false;
  } else {
    cerr << "ERROR: getline failed on trace file: " << file_name 
            << "at line " << line_number << "\n";
    exit(2);
  }
}

void Process::CmdMap(const string &line, 
                         const string &cmd, 
                         const vector<uint32_t> &cmdArgs) {
  // Allocate the specified memory->size
    uint32_t count = cmdArgs.at(0); 
    mem::Addr Vaddr = cmdArgs.at(1);

    ptm->Map(memory, count, Vaddr); 
    memory->set_PMCB(*pmcb); 
   
}

void Process::CmdDiff(const string &line,
                              const string &cmd,
                              const vector<uint32_t> &cmdArgs) {
  mem::Addr Vaddr = cmdArgs.back();
  //mem::Addr Paddr = 0; 
  //memory->ToPhysical(Vaddr, Paddr, false); 
  //std::cout << addr << " address\n"; 
  // Compare specified byte values
  int count = cmdArgs.size() - 1;
  for (int i = 0; i < count; ++i) {
      uint8_t temp = 0; 
      uint8_t *ptr = &temp; 
      memory->get_byte(ptr, Vaddr); 
    if(temp != cmdArgs.at(i)) {
      cout << "diff error at address " << std::hex << Vaddr
              << ", expected " << static_cast<uint32_t>(cmdArgs.at(i))
              << ", actual is " << static_cast<uint32_t>(temp) << "\n";
    }
    ++Vaddr;
  }
}

void Process::CmdStore(const string &line,
                       const string &cmd,
                       const vector<uint32_t> &cmdArgs) {
  // Store multiple bytes starting at specified address
  uint32_t Vaddr = cmdArgs.back();
  //mem::Addr Paddr = 0; 
  //memory->ToPhysical(Vaddr, Paddr, false); 
  int count = cmdArgs.size() - 1;
  for (int i = 0; i < count; ++i) {
      unsigned int temp = cmdArgs.at(i); 
     unsigned int *ptr = &temp;
     memory->put_byte(Vaddr+i, ptr);
  }
}

void Process::CmdDupl(const string &line,
                      const string &cmd,
                      const vector<uint32_t> &cmdArgs) {
  // Duplicate specified number of bytes to destination from source
//  auto dst = mem.begin() + cmdArgs.at(2);
//  auto src = mem.begin() + cmdArgs.at(1);
//  auto src_end = src + cmdArgs.at(0);
//  std::copy(src, src_end, dst);
    
   //uint8_t val = 0; 
   //uint8_t *temp = &val; 
  //memory->get_byte(temp, cmdArgs.at(1)); 
  
  //uint32_t count = cmdArgs.at(2) - cmdArgs.at(1); 
    mem::Addr Vstart = cmdArgs.at(1); 
    mem::Addr Vend = cmdArgs.at(2); 
    
    mem::Addr Pstart = 0; 
    mem::Addr Pend = 0; 
    
    //memory->ToPhysical(Vstart, Pstart, false); 
    //memory->ToPhysical(Vend, Pend, false); 
    
  uint8_t vals[cmdArgs.at(0)]; 
  memory->get_bytes(vals, Vstart, sizeof(vals)); 
  memory->put_bytes(Vend, sizeof(vals), vals); 
}

void Process::CmdRepl(const string &line,
                      const string &cmd,
                      const vector<uint32_t> &cmdArgs) {
  // Replicate specified value in destination range
  uint32_t temp = cmdArgs.at(0);
  uint32_t *value = &temp;  
  mem::Addr count = cmdArgs.at(1);
  mem::Addr Vaddr = cmdArgs.at(2);
  mem::Addr Paddr = 0; 
  //memory->ToPhysical(Vaddr, Paddr, false); 
  
  //std::cout << addr << " : " << count << " : " << *temp << " \n"; 
  for(int i = 0; i<count; i++){
        memory->put_byte(Vaddr+i, value);
  }
}

void Process::CmdPrint(const string &line,
                       const string &cmd,
                       const vector<uint32_t> &cmdArgs) {
  uint32_t Vaddr = cmdArgs.at(1);
  mem::Addr Paddr = 0; 
  //memory->ToPhysical(Vaddr, Paddr, false); 
  uint32_t count = cmdArgs.at(0);

  // Output the address
  cout << std::hex << Vaddr;

  // Output the specified number of bytes starting at the address
  for(int i = 0; i < count; ++i) {
    if((i % 16) == 0) {  // line break every 16 bytes
      cout << "\n";
    }
    unsigned int val = 0; 
    unsigned int *temp = &val; 
    memory->get_byte(temp, Vaddr++); 
    cout << " " << std::setfill('0') << std::setw(2)
            << static_cast<uint32_t> (*temp);
  }
  cout << "\n";
}

void Process::CmdPermiss(const string &line,
                       const string &cmd, 
        const std::vector<uint32_t> &cmdArgs){
    uint64_t count = cmdArgs.at(0); 
    uint8_t status = cmdArgs.at(1); 
    mem::Addr Vaddr = cmdArgs.at(2); 
    ptm->Permission(memory, count, status, Vaddr); 
    memory->set_PMCB(*pmcb); 
    
    
}
