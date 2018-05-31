#ifndef __simple_bus_communication_h
#define __simple_bus_communication_h

#include <systemc.h>

#include "simple_bus_communication.h"
#include "simple_bus_direct_if.h"


SC_MODULE(simple_bus_communication)
{
  // ports
  sc_in_clk clock;
  sc_port<simple_bus_direct_if> bus_port;

  SC_HAS_PROCESS(simple_bus_communication);

  // constructor
  simple_bus_communication(sc_module_name name_
                           , unsigned int address
                           , int timeout
                           , bool verbose = true)
    : sc_module(name_)
    , m_address(address)
    , m_timeout(timeout)
    , m_verbose(verbose)
  {
    // process declaration
    SC_THREAD(main_action);
  }

  // process
  void main_action();
  // communication_module methods
  int* generate_rgba();
  bool check_crc(int red, int green, int blue, int alpha, int crc);
  void set_rgba(int red, int green, int blue, int alpha);
  int get_red();
  int get_green();
  int get_blue();
  int get_alpha();
  int get_crc();

private:
  unsigned int m_address;
  int m_timeout;
  bool m_verbose;
  // communication_module: package attributes
  int crc;
  int r;
  int g;
  int b;
  int a;

}; // end class simple_bus_communication

#endif
