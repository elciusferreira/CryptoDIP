#include "simple_bus_master_com.h"
#include "simple_bus_types.h"

void simple_bus_master_com::main_action() {
    int mydata;
    int read_en;

    while (true) {
        read(&read_en, 0);
        sb_fprintf(stdout, "[COMUNICATION] TIME: %s VALUE: %d ", sc_time_stamp().to_string().c_str(), read_en);

        if (read_en == 1) {
            for (unsigned int i = m_start_address_intern_memory +4; i < m_end_address_intern_memory; i += 4) {
                read(&mydata, i);
                bus_port->direct_write(&mydata, i + 20);
                sb_fprintf(stdout, "[COMUNICATION] Read/Write TIME: %s READ FROM: %d VALUE: %d\n",
                           sc_time_stamp().to_string().c_str(), i, mydata);

                wait(m_timeout, SC_NS);
            }

            mydata = m_start_address_intern_memory + 20;
            bus_port->direct_write(&mydata, 0);
            wait(m_timeout, SC_NS);

            sb_fprintf(stdout, "[COMUNICATION] WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n",
                       sc_time_stamp().to_string().c_str(),
                       0,
                       mydata);

            mydata = m_end_address_intern_memory + 20;
            bus_port->direct_write(&mydata, 4);
            wait(m_timeout, SC_NS);

            sb_fprintf(stdout, "[COMUNICATION] WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n",
                       sc_time_stamp().to_string().c_str(),
                       4,
                       mydata);

            // Can Work
            mydata = 0;
            bus_port->direct_write(&mydata, m_address_graphics);
            sb_fprintf(stdout, "[COMUNICATION] WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n",
                       sc_time_stamp().to_string().c_str(),
                       m_address_graphics,
                       mydata);
            wait(m_timeout, SC_NS);

            mydata = 10;
            bus_port->direct_write(&mydata, m_address_width);
            sb_fprintf(stdout, "[COMUNICATION] WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n",
                       sc_time_stamp().to_string().c_str(),
                       m_address_width,
                       mydata);
            wait(m_timeout, SC_NS);
            mydata = 0;
            write(&mydata, 0);

            mydata = 1;
            bus_port->direct_write(&mydata, m_address_cryption);
            sb_fprintf(stdout, "[COMUNICATION] WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n",
                       sc_time_stamp().to_string().c_str(),
                       m_address_cryption,
                       mydata);
            wait(m_timeout, SC_NS);
        }
        else {
            sb_fprintf(stdout, " CANT WORK!!!\n");
        }

        wait(m_timeout, SC_NS);
    }
}

inline bool simple_bus_master_com::direct_read(int *data, unsigned int address) {
    return (read(data, address) == SIMPLE_BUS_OK);
}

inline bool simple_bus_master_com::direct_write(int *data, unsigned int address) {
    return (write(data, address) == SIMPLE_BUS_OK);
}

inline simple_bus_status simple_bus_master_com::read(int *data, unsigned int address) {
    *data = MEM[(address - m_start_address_intern_memory) / 4];
    return SIMPLE_BUS_OK;
}

inline simple_bus_status simple_bus_master_com::write(int *data, unsigned int address) {
    MEM[(address - m_start_address_intern_memory) / 4] = *data;
    return SIMPLE_BUS_OK;
}

inline  simple_bus_master_com::~simple_bus_master_com() {
    if (MEM) delete[] MEM;
    MEM = (int *) 0;
}

inline unsigned int simple_bus_master_com::start_address() const {
    return m_start_address_intern_memory;
}

inline unsigned int simple_bus_master_com::end_address() const {
    return m_end_address_intern_memory;
}



