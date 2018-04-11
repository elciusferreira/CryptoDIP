#ifndef __simple_bus_test_h
#define __simple_bus_test_h

#include <systemc.h>

#include "simple_bus.h"
#include "simple_bus_fast_mem.h"
#include "simple_bus_arbiter.h"
#include "simple_bus_gpu.h"

SC_MODULE(simple_bus_test) {
    // channels
    sc_clock C1;

    // module instances
    //simple_bus_my_block      *my_master;
    simple_bus_gpu *gpu;
    simple_bus *bus;
    simple_bus_fast_mem *fast_mem;
    simple_bus_arbiter *arbiter;

    // constructor
    SC_CTOR(simple_bus_test)
            : C1("C1") {
        // create instances
        //gpu = new simple_bus_gpu("mygpu", 0x80, 0x1090, 4096, 100, true, true);
        gpu = new simple_bus_gpu("mygpu");
        gpu->toggleVerbose();
        gpu->toggleTest();

        fast_mem = new simple_bus_fast_mem("fast_mem", 0x00, 0x7f);
        bus = new simple_bus("bus");
        arbiter = new simple_bus_arbiter("arbiter");

        // connect instances
        bus->clock(C1);
        gpu->clock(C1);
        gpu->bus_port(*bus);
        bus->arbiter_port(*arbiter);
        bus->slave_port(*fast_mem);
    }

    // destructor
    ~simple_bus_test() {
        if (gpu) {
            delete gpu;
            gpu = 0;
        }
        if (fast_mem) {
            delete fast_mem;
            fast_mem = 0;
        }
        if (bus) {
            delete bus;
            bus = 0;
        }
        if (arbiter) {
            delete arbiter;
            arbiter = 0;
        }
    }

};

#endif
