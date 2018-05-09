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

//using mem; 

Process::Process(string file_name_) 
: file_name(file_name_), line_number(0) {
  // Open the trace file.  Abort program if can't open.
  trace.open(file_name, std::ios_base::in);
  if (!trace.is_open()) {
    cerr << "ERROR: failed to open trace file: " << file_name << "\n";
    exit(2);
  }
}

Process::~Process() {
  trace.close();
}

void Process::Run(void) {
  // Read and process commands
  string line;                // text line read
  string cmd;                   // command from line
  vector<uint32_t> cmdArgs;
  uint32_t size = 0; 
  while(cmd != "memsize"){
      ParseCommand(line, cmd, cmdArgs);
      if(cmd == "memsize"){
          size = CmdMemSize(line, cmd, cmdArgs); 
      }

  }
   // arguments from line
  mem::MMU memory(size); 
  // Select the command to execute
  while (ParseCommand(line, cmd, cmdArgs)) {
     if (cmd == "diff") {
      CmdDiff(line, cmd, cmdArgs, memory);  // get and compare multiple bytes
    } else if (cmd == "store") {
      CmdStore(line, cmd, cmdArgs, memory);      // put bytes
    } else if (cmd == "replicate") {
      CmdRepl(line, cmd, cmdArgs, memory);     // fill bytes with value
    } else if (cmd == "duplicate") {
      CmdDupl(line, cmd, cmdArgs, memory);     // copy bytes to dest from source
    } else if (cmd == "print") {
      CmdPrint(line, cmd, cmdArgs, memory);     // dump byte values to output
    } else if (cmd != "#") {
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

uint32_t Process::CmdMemSize(const string &line, 
                         const string &cmd, 
                         const vector<uint32_t> &cmdArgs) {
  // Allocate the specified memory size
  //mem.resize(cmdArgs.at(0), 0);
    //TODO: find a way to round up to the next multiple
    uint32_t no_of_frames = 0; 
    uint32_t rem = cmdArgs.at(0)%mem::kPageSize; 
    if(rem != 0){
        rem = mem::kPageSize - rem; 
    }
    no_of_frames = rem + cmdArgs.at(0); 
    no_of_frames = no_of_frames/mem::kPageSize; 
    //memory(no_of_frames); 
    return no_of_frames; 
}

void Process::CmdDiff(const string &line,
                              const string &cmd,
                              const vector<uint32_t> &cmdArgs, mem::MMU &memory) {
  mem::Addr addr = cmdArgs.back();
  //std::cout << addr << " address\n"; 
  // Compare specified byte values
  int count = cmdArgs.size() - 1;
  for (int i = 0; i < count; ++i) {
      uint8_t temp = 0; 
      uint8_t *ptr = &temp; 
      memory.get_byte(ptr, addr); 
    if(temp != cmdArgs.at(i)) {
      cout << "diff error at address " << std::hex << addr
              << ", expected " << static_cast<uint32_t>(cmdArgs.at(i))
              << ", actual is " << static_cast<uint32_t>(temp) << "\n";
    }
    ++addr;
  }
}

void Process::CmdStore(const string &line,
                       const string &cmd,
                       const vector<uint32_t> &cmdArgs, mem::MMU &memory) {
  // Store multiple bytes starting at specified address
  uint32_t addr = cmdArgs.back();
  int count = cmdArgs.size() - 1;
  for (int i = 0; i < count; ++i) {
      unsigned int temp = cmdArgs.at(i); 
     unsigned int *ptr = &temp;
     memory.put_byte(addr+i, ptr);
  }
}

void Process::CmdDupl(const string &line,
                      const string &cmd,
                      const vector<uint32_t> &cmdArgs, mem::MMU &memory) {
  // Duplicate specified number of bytes to destination from source
//  auto dst = mem.begin() + cmdArgs.at(2);
//  auto src = mem.begin() + cmdArgs.at(1);
//  auto src_end = src + cmdArgs.at(0);
//  std::copy(src, src_end, dst);
    
   //uint8_t val = 0; 
   //uint8_t *temp = &val; 
  //memory.get_byte(temp, cmdArgs.at(1)); 
  
  //uint32_t count = cmdArgs.at(2) - cmdArgs.at(1); 
    mem::Addr start = cmdArgs.at(1); 
    mem::Addr end = cmdArgs.at(2); 
  uint8_t vals[cmdArgs.at(0)]; 
  memory.get_bytes(vals, start, sizeof(vals)); 
  memory.put_bytes(end, sizeof(vals), vals); 
}

void Process::CmdRepl(const string &line,
                      const string &cmd,
                      const vector<uint32_t> &cmdArgs, mem::MMU &memory) {
  // Replicate specified value in destination range
  uint32_t temp = cmdArgs.at(0);
  uint32_t *value = &temp;  
  mem::Addr count = cmdArgs.at(1);
  mem::Addr addr = cmdArgs.at(2);
  
  //std::cout << addr << " : " << count << " : " << *temp << " \n"; 
  for(int i = 0; i<count; i++){
        memory.put_byte(addr+i, value);
  }
}

void Process::CmdPrint(const string &line,
                       const string &cmd,
                       const vector<uint32_t> &cmdArgs, mem::MMU &memory) {
  uint32_t addr = cmdArgs.at(1);
  uint32_t count = cmdArgs.at(0);

  // Output the address
  cout << std::hex << addr;

  // Output the specified number of bytes starting at the address
  for(int i = 0; i < count; ++i) {
    if((i % 16) == 0) {  // line break every 16 bytes
      cout << "\n";
    }
    unsigned int val = 0; 
    unsigned int *temp = &val; 
    memory.get_byte(temp, addr++); 
    cout << " " << std::setfill('0') << std::setw(2)
            << static_cast<uint32_t> (*temp);
  }
  cout << "\n";
}
