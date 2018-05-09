#include "simple_bus_master_gerad.h"
#include "simple_bus_types.h"

void simple_bus_master_gerad::main_action() {
    sb_fprintf(stdout, "GENERATOR START!\n");
    int newValue;
    unsigned int newAddress;

    newValue = 0;
    bus_port->direct_write(&newValue, 0);

    // READ TXT!!!!






    // SAVE MEMORY
    for (unsigned int i = m_address_start + 4; i < m_address_end; i += 4) {
        newValue = 1;
        newAddress = i;

        bus_port->direct_write(&newValue, newAddress);

        sb_fprintf(stdout, "[GENERATOR] WRITE-> TIME: %s READ FROM: %d VALUE: %d\n",
                   sc_time_stamp().to_string().c_str(),
                   newAddress,
                   newValue);

        wait(m_timeout, SC_NS);
    }
    newValue = 1;
    bus_port->direct_write(&newValue, 0);
}


