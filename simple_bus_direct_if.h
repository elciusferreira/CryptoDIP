#ifndef __simple_bus_direct_if_h
#define __simple_bus_direct_if_h

#include <systemc.h>

class simple_bus_direct_if
        : public virtual sc_interface {
public:
    // direct BUS/Slave interface
    virtual bool direct_read(int *data, unsigned int address) = 0;
    virtual bool direct_write(int *data, unsigned int address) = 0;

}; // end class simple_bus_direct_if

#endif
