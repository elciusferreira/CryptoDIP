#ifndef __simple_bus_master_gerad_h
#define __simple_bus_master_gerad_h

#include <systemc.h>

#include "simple_bus_direct_if.h"
#include <iostream>
#include <fstream>


SC_MODULE(simple_bus_master_gerad) {
    // ports
    sc_in_clk clock;
    sc_port<simple_bus_direct_if> bus_port;

    SC_HAS_PROCESS(simple_bus_master_gerad);

    // constructor
    simple_bus_master_gerad(sc_module_name name_, unsigned int address_start, unsigned int address_end, int timeout,
                            bool verbose = true)
            : sc_module(name_), m_address_start(address_start), m_address_end(address_end), m_timeout(timeout),
              m_verbose(verbose) {
        // process declaration
        SC_THREAD(main_action);
    }

    // process
    void main_action();

private:
    unsigned int m_address_start, m_address_end;
    int m_timeout;
    bool m_verbose;

}; // end class simple_bus_master_gerad

#endif
