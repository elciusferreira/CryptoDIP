#ifndef __simple_bus_encryption_h
#define __simple_bus_encryption_h

#include <systemc.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "simple_bus_direct_if.h"

SC_MODULE(simple_bus_encryption)
{
  // ports
  sc_in_clk clock;
  sc_port<simple_bus_direct_if> bus_port;

  SC_HAS_PROCESS(simple_bus_encryption);

  // constructor
  simple_bus_encryption(sc_module_name name_
                           , unsigned int address_start
                           , unsigned int address_end
                           , unsigned int address_reserved
                           , unsigned int address_graphs
                           , int timeout
                           , bool verbose)
    : sc_module(name_)
    , m_address_start(address_start)
    , m_address_end(address_end)
    , m_address_reserved(address_reserved)
    , m_address_graphs(address_graphs)
    , m_timeout(timeout)
    , m_verbose(verbose)
  {
    // process declaration
    key_c[0] = 99;
    key_c[1] = 111;
    key_c[2] = 112;
    key_c[3] = 114;
    key_c[4] = 111;
    key_c[5] = 106;
    key_c[6] = 101;
    key_c[7] = 116;
    key_c[8] = 111;
    size_key = 9;


    SC_THREAD(main_action);
  }

  // process
  void main_action();

private:
  unsigned int m_address_start, m_address_end, m_address_reserved, m_address_graphs;
  unsigned int address_read_start, address_read_end;
  int m_timeout;
  bool m_verbose;
  int size_key;
  int key_c[9];
  int s[256];

  // private process
  void KSA();
  void PRGA();
  void change(int i, int j);
  void getRange();
  void seeMemory();
  void openFileAndSaveMemory();
  void compareResult();
  void saveFile(std::string, std::vector<int>, int);


}; // end class simple_bus_master_direct

#endif
