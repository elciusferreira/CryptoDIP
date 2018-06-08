#ifndef __simple_bus_master_com_h
#define __simple_bus_master_com_h

#include <systemc.h>

#include "simple_bus_types.h"
#include "simple_bus_slave_if.h"
#include "simple_bus_direct_if.h"
#include <vector>
#include <opencv2/highgui.hpp>
#include <openssl/sha.h>
#include <sstream>

class simple_bus_master_com : public simple_bus_slave_if, public sc_module {
private:
    unsigned int m_address_start;
    unsigned int m_address_end;
    unsigned int m_address_cryption;
    unsigned int m_address_graphics;
    unsigned int m_address_width;
    unsigned int m_address_height;
    unsigned int m_start_address_intern_memory;
    unsigned int m_end_address_intern_memory;
    unsigned int m_opflag;
    unsigned int m_end_opflag;
    int m_timeout;
    int m_timeout_com;
    bool m_verbose;
    int *MEM;

    std::string crc_generator(int red, int green, int blue, int alpha);

public:
    // ports
    sc_in_clk clock;
    sc_port<simple_bus_direct_if> bus_port;

    SC_HAS_PROCESS(simple_bus_master_com);

    // constructor
    simple_bus_master_com(sc_module_name name_,
                          unsigned int address_start,
                          unsigned int address_end,
                          unsigned int address_cryption,
                          unsigned int address_graphics,
                          unsigned int address_width,
                          unsigned int address_height,
                          unsigned int start_address_intern_mem,
                          unsigned int end_address_intern_mem,
                          unsigned int opflag,
                          unsigned int end_opflag,
                          int timeout,
                          int com_timeout,
                          bool verbose)
            : sc_module(name_),
              m_address_start(address_start),
              m_address_end(address_end),
              m_address_cryption(address_cryption),
              m_address_graphics(address_graphics),
              m_address_width(address_width),
              m_address_height(address_height),
              m_start_address_intern_memory(start_address_intern_mem),
              m_end_address_intern_memory(end_address_intern_mem),
              m_opflag(opflag),
              m_end_opflag(end_opflag),
              m_timeout(timeout),
              m_timeout_com(com_timeout),
              m_verbose(verbose) {
        // process declaration
        sc_assert(m_start_address_intern_memory <= m_end_address_intern_memory);
        sc_assert((m_end_address_intern_memory - m_start_address_intern_memory + 1) % 4 == 0);

        unsigned int size = (m_start_address_intern_memory - m_start_address_intern_memory + 1) / 4;
        MEM = new int[size];
        for (unsigned int i = 0; i < size; ++i)
            MEM[i] = 0;

        SC_THREAD(main_action);
    }

    // process
    void main_action();
    bool check_crc(std::vector<std::string> pckt_data);

    // direct Slave Interface
    bool direct_read(int *data, unsigned int address);
    bool direct_write(int *data, unsigned int address);

    // Slave Interface
    simple_bus_status read(int *data, unsigned int address);
    simple_bus_status write(int *data, unsigned int address);
    unsigned int start_address() const;
    unsigned int end_address() const;

    // destructor
    ~simple_bus_master_com();

}; // end class simple_bus_master_direct

#endif
