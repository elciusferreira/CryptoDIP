#include "simple_bus_master_com.h"
#include "simple_bus_types.h"
/*
// Verify data integrity.
bool check_crc(int red, int green, int blue, int alpha, std::string expected_crc) {
        crc = crc_generator(red, green, blue, alpha);
        if (expected_crc == crc)
            return true;
        else
            return false;
}

// Calculate packet CRC
std::string simple_bus_master_com::crc_generator(int red, int green, int blue, int alpha) {
    std::string crc;
    char components[4];
    components[0] = red;
    components[1] = green;
    components[2] = blue;
    components[3] = alpha;

    unsigned char digest[SHA256_DIGEST_LENGTH];

    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, components, strlen(components));
    SHA256_Final(digest, &ctx);

    char* SHAString = new char[SHA256_DIGEST_LENGTH*2+1];
    for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(&SHAString[i*2], "%02x", (unsigned int)digest[i]);

    crc += std::string(SHAString) + "," + std::to_string(red & 0xff) + "," + std::to_string(green & 0xff) + "," +
            std::to_string(blue & 0xff) + "," + std::to_string(alpha & 0xff) + "\n";

    return outputText;

}
*/
void simple_bus_master_com::main_action() {
    int mydata;
    int read_en;
    unsigned int memory_idx = 8;
    std::vector<int> packet;

    // Cant Work
    mydata = 0;
    bus_port->direct_write(&mydata, m_address_graphics);
    //sb_fprintf(stdout, "[COMUNICATION] WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n",
    //           sc_time_stamp().to_string().c_str(),
    //           m_address_graphics,
    //           mydata);
    //wait(m_timeout, SC_NS);

    mydata = 0;
    bus_port->direct_write(&mydata, m_address_cryption);
    //sb_fprintf(stdout, "[COMUNICATION] WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n",
    //           sc_time_stamp().to_string().c_str(),
    //           m_address_cryption,
    //           mydata);
    //wait(m_timeout, SC_NS);

    while (true) {
        wait(m_timeout, SC_NS);

        read(&read_en, 4);
        //sb_fprintf(stdout, "[COMUNICATION] TIME: %s VALUE: %d ", sc_time_stamp().to_string().c_str(), read_en);
        if (read_en == 0) { // check if received all packets
            read(&read_en, 0);
            if (read_en == 1) { // check if can the module mcan work

                //sb_fprintf(stdout, "[COMUNICATION] ENTRY");
                for (unsigned int i = m_start_address_intern_memory + 8; i <= m_end_address_intern_memory; i += 4) {
                    read(&mydata, i);
                    packet.push_back(mydata);

                    sb_fprintf(stdout, "[COMUNICATION] -> I: %i, TIME: %s, VALUE: %d\n",
                                i, sc_time_stamp().to_string().c_str(), mydata);

                    wait(m_timeout, SC_NS);
                }
                // get crc from packets

                // check crc
                //if (check_crc == true) {
                    // save pixel in memory global (m_controller set position)

                    mydata = 0;
                    bus_port->direct_write(&mydata, memory_idx);
                //}

                // Generator can work
                mydata = 0;
                write(&mydata, 0);
                wait(m_timeout, SC_NS);
            }
            else {
                sb_fprintf(stdout, "[COMUNICATION] TIME: %s, WAITING\n",
                           sc_time_stamp().to_string().c_str());
            }
        }
        else {
            sb_fprintf(stdout, "[COMUNICATION] TIME: %s, FINISHED\n",
                       sc_time_stamp().to_string().c_str());

            // Stores the initial position of the image data in global memory
            mydata = m_start_address_intern_memory + 20;
            bus_port->direct_write(&mydata, 0);
            wait(m_timeout, SC_NS);

            sb_fprintf(stdout, "[COMUNICATION] WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n",
                       sc_time_stamp().to_string().c_str(),
                       0,
                       mydata);

            // Stores the final position of the image data in global memory
            mydata = m_end_address_intern_memory + 20;
            bus_port->direct_write(&mydata, 4);
            wait(m_timeout, SC_NS);

            sb_fprintf(stdout, "[COMUNICATION] WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n",
                       sc_time_stamp().to_string().c_str(),
                       4,
                       mydata);


              mydata = 10;
              bus_port->direct_write(&mydata, m_address_width);
              sb_fprintf(stdout, "[COMUNICATION] WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n",
                          sc_time_stamp().to_string().c_str(),
                          m_address_width,
                          mydata);
              wait(m_timeout, SC_NS);


            mydata = 1;
            bus_port->direct_write(&mydata, m_address_cryption);
            sb_fprintf(stdout, "[COMUNICATION] WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n",
                       sc_time_stamp().to_string().c_str(),
                       m_address_cryption,
                       mydata);

            read_en = 0;
            write(&read_en, 4);

            wait(m_timeout, SC_NS);
        }

        //wait(m_timeout, SC_NS);
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
