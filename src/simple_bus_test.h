#ifndef __simple_bus_test_h
#define __simple_bus_test_h

#include <systemc.h>

#include "simple_bus.h"
#include "simple_bus_master_blocking.h"
#include "simple_bus_master_non_blocking.h"
#include "simple_bus_master_gerad.h"
#include "simple_bus_encryption.h"
#include "simple_bus_gpu.h"
#include "simple_bus_arbiter.h"
#include "simple_bus_master_com.h"
#include "simple_bus_slow_mem.h"
#include "simple_bus_fast_mem.h"

SC_MODULE(simple_bus_test) {
    // channels
    sc_clock C1;

    // module instances
    simple_bus_encryption *master_encryption;
    simple_bus_master_com *master_communication;
    simple_bus_master_gerad *master_generator;
    simple_bus_gpu *master_gpu;
    simple_bus *bus;
    simple_bus *bus_communication;
    simple_bus_fast_mem *mem_fast;
    simple_bus_arbiter *arbiter;
    simple_bus_arbiter *arbiter_communication;


    // constructor
    SC_CTOR(simple_bus_test)
            : C1("C1") {
        // Memory Parameters
        const unsigned int memory_begin = 0;
        const unsigned int memory_end = 16777215;  // Maximum of 16777215 memory positions, able to save 4K (3840 × 2160) image.
        const unsigned int com_memory_begin = 0;
        const unsigned int com_memory_end = 75;
        const unsigned int image_begin_addr = 0;
        const unsigned int image_end_addr = 4;
        const unsigned int image_width_addr = 16;
        const unsigned int image_height_addr = 20;
        const unsigned int image_max_size = 8294400;    // 4K image


        // Flags
        const unsigned int crypto_opflag = 8;
        const unsigned int gpu_opflag = 12;
        const unsigned int com_opflag = 0;
        const unsigned int com_end_opflag = 4;
        const bool verbose = true;

        // Timeout (ns)
        const unsigned int timeout = 30;     // Global mem timeout
        const unsigned int com_timeout = 5;     // Global mem timeout

        // create instances
        master_generator = new simple_bus_master_gerad("master_generator",
                                                       memory_begin,
                                                       memory_end,
                                                       com_opflag,
                                                       com_end_opflag,
                                                       com_timeout,
                                                       verbose);

        master_communication = new simple_bus_master_com("master_communication",
                                                         memory_begin,
                                                         memory_end,
                                                         crypto_opflag,
                                                         gpu_opflag,
                                                         image_width_addr,
                                                         image_height_addr,
                                                         com_memory_begin,
                                                         com_memory_end,
                                                         com_opflag,
                                                         com_end_opflag,
                                                         timeout,
                                                         com_timeout,
                                                         verbose);

        master_encryption = new simple_bus_encryption("master_encryption",
                                                      image_begin_addr,
                                                      image_end_addr,
                                                      crypto_opflag,
                                                      gpu_opflag,
                                                      image_width_addr,
                                                      timeout,
                                                      verbose);

        master_gpu = new simple_bus_gpu("master_gpu",
                                        image_begin_addr,
                                        gpu_opflag,
                                        image_width_addr,
                                        image_height_addr,
                                        image_max_size,
                                        timeout,
                                        verbose);

        mem_fast = new simple_bus_fast_mem("mem_fast",
                                           memory_begin,
                                           memory_end);
        bus = new simple_bus("bus", true);
        bus_communication = new simple_bus("bus_communication", true);
        arbiter = new simple_bus_arbiter("arbiter", true);
        arbiter_communication = new simple_bus_arbiter("arbiter_communication", true);

        // connect instances
        master_encryption->clock(C1);
        master_communication->clock(C1);
        master_generator->clock(C1);
        master_gpu->clock(C1);
        bus->clock(C1);
        bus_communication->clock(C1);
        master_encryption->bus_port(*bus);
        master_communication->bus_port(*bus);
        master_generator->bus_port(*bus_communication);
        master_gpu->bus_port(*bus);
        bus->arbiter_port(*arbiter);
        bus->slave_port(*mem_fast);
        bus_communication->arbiter_port(*arbiter_communication);
        bus_communication->slave_port(*master_communication);
    }

    // destructor
    ~simple_bus_test() {
        if (master_encryption) {
            delete master_encryption;
            master_encryption = 0;
        }
        if (master_communication) {
            delete master_communication;
            master_communication = 0;
        }
        if (master_generator) {
            delete master_generator;
            master_generator = 0;
        }
        if (master_gpu) {
            delete master_gpu;
            master_gpu = 0;
        }
        if (bus) {
            delete bus;
            bus = 0;
        }
        if (bus_communication) {
            delete bus_communication;
            bus_communication = 0;
        }
        if (mem_fast) {
            delete mem_fast;
            mem_fast = 0;
        }
        if (arbiter) {
            delete arbiter;
            arbiter = 0;
        }
        if (arbiter_communication) {
            delete arbiter_communication;
            arbiter_communication = 0;
        }
    }

}; // end class simple_bus_test

#endif
