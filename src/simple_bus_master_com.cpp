#include "simple_bus_master_com.h"
#include "simple_bus_types.h"
#include <sstream>

std::string itos(const int num) {
    std::ostringstream convert;
    convert << num;
    return convert.str();
}

// Convert string to int
int inline stoi(const std::string str) {
    int num;
    std::istringstream(str) >> num;
    return num;
}

// Calculate packet CRC
int simple_bus_master_com::crc_generator(std::vector<int> stream) {
    std::string crc;
    std::string pixels;

    for (std::vector<int>::iterator it = stream.begin(); it != stream.end(); ++it) {
        pixels += itos(((*it) >> 24) & 0xff);
        pixels += itos(((*it) >> 16) & 0xff);
        pixels += itos(((*it) >> 8) & 0xff);
        pixels += itos((*it) & 0xff);
    }

    const char *components = pixels.c_str();
    unsigned char digest[SHA256_DIGEST_LENGTH];

    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, components, strlen(components));
    SHA256_Final(digest, &ctx);

    char *SHAString = new char[SHA256_DIGEST_LENGTH * 2 + 1];
    for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(&SHAString[i * 2], "%02x", (unsigned int) digest[i]);

    std::string aux = SHAString;
    for (unsigned int j = 56; j < 64; j++)
        crc += aux[j];

    return stoi(crc);
}


// Verify data integrity.
bool simple_bus_master_com::check_crc(std::vector<int> pckt_data) {
    int crc = pckt_data[0];
    pckt_data.erase(pckt_data.begin());
    std::vector<int> pixels = pckt_data;
    int expected_crc = crc_generator(pixels);

    if (m_verbose)
        sb_fprintf(stdout,
                   "[COMUNICATION][CHECK] -> CRC READ: %u, CRC EXPECTED: %u, TIME: %s\n",
                   crc, expected_crc, sc_time_stamp().to_string().c_str());

    return expected_crc == crc;
}


void simple_bus_master_com::main_action() {
    int mydata;
    int read_en;
    unsigned int memory_idx = 16;
    //unsigned int m_controller = 4;
    std::vector<int> packet;
    bool isDimensions = true;

    // Cant Work
    mydata = 0;
    bus_port->direct_write(&mydata, m_address_cryption);

    // Cant Work
    mydata = 0;
    bus_port->direct_write(&mydata, m_address_graphics);

    while (true) {
        read(&read_en, m_end_opflag);
        wait(m_timeout_com, SC_NS);
        //sb_fprintf(stdout, "[COMUNICATION] TIME: %s VALUE: %d ", sc_time_stamp().to_string().c_str(), read_en);
        if (read_en == 0) { // check if received all packets
            read(&read_en, m_opflag);
            wait(m_timeout_com, SC_NS);

            if (read_en == 1) { // check if can the module can work
                //sb_fprintf(stdout, "[COMUNICATION] ENTRY");
                if (isDimensions) {
                    for (unsigned int i = m_start_address_intern_memory + 8; i <= m_start_address_intern_memory + 16; i += 4) {
                        read(&mydata, i);
                        wait(m_timeout_com, SC_NS);
                        packet.push_back(mydata);

                        if (m_verbose)
                            sb_fprintf(stdout, "[COMUNICATION] -> I: %i, TIME: %s, VALUE: %u\n",
                                       i, sc_time_stamp().to_string().c_str(), mydata);
                    }
                }
                else {
                    for (unsigned int i = m_start_address_intern_memory + 8; i <= m_end_address_intern_memory; i += 4) {
                        read(&mydata, i);
                        wait(m_timeout_com, SC_NS);
                        packet.push_back(mydata);

                        if (m_verbose)
                            sb_fprintf(stdout, "[COMUNICATION] -> I: %i, TIME: %s, VALUE: %u\n",
                                       i, sc_time_stamp().to_string().c_str(), mydata);


                    }
                }
                // get crc from packets
                // check crc

                if (check_crc(packet)) {
                    // save pixel in memory global (m_controller set position)
                    if (m_verbose)
                        sb_fprintf(stdout, "[COMUNICATION] -> CRC @@@@@@@@@@@, TIME: %s\n",
                                   sc_time_stamp().to_string().c_str());

                    unsigned int end;

                    if (isDimensions)
                        end = m_start_address_intern_memory + 16;
                    else
                        end = m_end_address_intern_memory;

                    packet.erase(packet.begin());
                    for (unsigned int i = m_start_address_intern_memory + 12; i <= end; i += 4) {
                        if (!packet.empty()) {
                            mydata = packet.at(0);
                            packet.erase(packet.begin());
                        }
                        else
                            break;

                        bus_port->direct_write(&mydata, memory_idx);
                        wait(m_timeout, SC_NS);
                        memory_idx += 4;
                    }
                }
                else {
                    if (m_verbose)
                        sb_fprintf(stdout, "[COMUNICATION] -> CRC NNNNNNNNNNNNNNNNNNN, TIME: %s\n",
                                   sc_time_stamp().to_string().c_str());
                }

                // save pixel in memory global (m_controller set position)

                // Generator can work
                mydata = 0;
                write(&mydata, m_opflag);
                wait(m_timeout_com, SC_NS);

                packet.clear();
                isDimensions = false;
            } else {
                if (m_verbose)
                    sb_fprintf(stdout, "[COMUNICATION] TIME: %s, WAITING\n",
                               sc_time_stamp().to_string().c_str());
            }
        }
        else {
            if (m_verbose)
                sb_fprintf(stdout, "[COMUNICATION] TIME: %s, FINISHED\n",
                           sc_time_stamp().to_string().c_str());

            // Stores the initial position of the image data in global memory
            mydata = m_start_address_intern_memory + 24;
            bus_port->direct_write(&mydata, m_opflag);
            wait(m_timeout, SC_NS);


            if (m_verbose)
                sb_fprintf(stdout, "[COMUNICATION] SAVE BEGIN MEMORY: TIME: %s READ FROM: %d VALUE: %d\n",
                           sc_time_stamp().to_string().c_str(),
                           m_opflag,
                           mydata);

            // Stores the final position of the image data in global memory
            mydata = memory_idx;
            bus_port->direct_write(&mydata, m_end_opflag);
            wait(m_timeout, SC_NS);

            if (m_verbose)
                sb_fprintf(stdout, "[COMUNICATION] SAVE END MEMORY: TIME: %s READ FROM: %d VALUE: %d\n",
                           sc_time_stamp().to_string().c_str(),
                           m_end_opflag,
                           mydata);

            mydata = 1;
            bus_port->direct_write(&mydata, m_address_cryption);
            wait(m_timeout, SC_NS);

            if (m_verbose)
                sb_fprintf(stdout, "[COMUNICATION] WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n",
                           sc_time_stamp().to_string().c_str(),
                           m_address_cryption,
                           mydata);

            read_en = 0;
            write(&read_en, m_end_opflag);

            wait(m_timeout_com, SC_NS);
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
