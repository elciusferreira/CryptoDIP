#include "simple_bus_master_com.h"
#include "simple_bus_types.h"
#include <sstream>

// e85e0680276cda04f0c8315c42f5bb6adcf4389920ceb46cc9858dc981d60abf
// ae5f2eab64c03d17939b8218239babf4a5da605061783eae985d11cc9471dbaf

std::string itos(const int num){
    std::ostringstream convert;
    convert << num;
    return convert.str();
}

// Convert string to int
int inline stoi(const std::string str){
    int num;
    std::istringstream(str) >> num;
    return num;
}

// Calculate packet CRC
std::string simple_bus_master_com::crc_generator(int red, int green, int blue, int alpha) {
    std::string crc;

    std::string pixels;
    pixels += char(red);
    pixels += char(green);
    pixels += char(blue);
    pixels += char(alpha);

    const char *components = pixels.c_str();

    unsigned char digest[SHA256_DIGEST_LENGTH];

    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, components, strlen(components));
    SHA256_Final(digest, &ctx);

    //sb_fprintf(stdout, "[COMUNICATION][CRC GENERATOR] STRLEN: %i\n", strlen(components));

    char* SHAString = new char[SHA256_DIGEST_LENGTH*2+1];
    for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(&SHAString[i*2], "%02x", (unsigned int)digest[i]);

        // sb_fprintf(stdout, "[COMUNICATION][CRC GENERATOR] -> CRC %s, RED: %i, GREEN: %i, BLUE: %i, ALPHA: %i, RED_CHAR: %u TIME: %s\n"
        //            , SHAString, red, green, blue, alpha, int(components[0]) & 0xff, sc_time_stamp().to_string().c_str());

   //sb_fprintf(stdout, "\n------> AQUI: ");

   // for (unsigned int b = 0; b < strlen(components); b++)
   //     sb_fprintf(stdout, "%c", components[b]);
   // sb_fprintf(stdout, "\n");


    return SHAString;
}
std::vector<std::string> decode(std::vector<int> code){
    std::string crc="", r="", g="", b="", a="";
    std::vector<std::string> ret;

    for (std::vector<int>::iterator it = code.begin(); it != code.end(); ++it) {
        if (it == code.end() - 1){
            r += itos(((*it) >> 24) & 0xff);
            g += itos(((*it) >> 16) & 0xff);
            b += itos(((*it) >> 8) & 0xff);
            a += itos((*it) & 0xff);
        }
        else{
            for (int i = 0; i < 4; ++i)
                crc += char((*it) >> ((3 - i) * 8));
        }
    }

    ret.push_back(crc);
    ret.push_back(r);
    ret.push_back(g);
    ret.push_back(b);
    ret.push_back(a);

    sb_fprintf(stdout, "[COMUNICATION][decode] -> CRC %s, RED: %s, GREEN: %s, BLUE: %s, ALPHA: %s, TIME: %s\n"
                , crc.c_str(), r.c_str(), g.c_str(), b.c_str(), a.c_str(), sc_time_stamp().to_string().c_str());

    return ret;
}

// Verify data integrity.
bool simple_bus_master_com::check_crc(std::vector<std::string> pckt_data) {
    std::string crc = pckt_data[0];
    int red = stoi(pckt_data[1]);
    int green = stoi(pckt_data[2]);
    int blue = stoi(pckt_data[3]);
    int alpha = stoi(pckt_data[4]);

    std::string expected_crc = crc_generator(red, green, blue, alpha);

    sb_fprintf(stdout, "[COMUNICATION][CHECK] -> CRC READ: %s, CRC EXPECTED: %s, RED: %i, GREEN: %i, BLUE: %i, ALPHA: %i, TIME: %s\n"
            , crc.c_str(), expected_crc.c_str(), red, green, blue, alpha, sc_time_stamp().to_string().c_str());


    if (expected_crc == crc){
        return true;
    }else
        return false;
}

void simple_bus_master_com::main_action() {
    int mydata;
    int read_en;
    unsigned int memory_idx = 20;
    //unsigned int m_controller = 4;
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

        read(&read_en, 4);
        wait(m_timeout, SC_NS);
        //sb_fprintf(stdout, "[COMUNICATION] TIME: %s VALUE: %d ", sc_time_stamp().to_string().c_str(), read_en);
        if (read_en == 0) { // check if received all packets
            read(&read_en, 0);
            if (read_en == 1) { // check if can the module mcan work

                //sb_fprintf(stdout, "[COMUNICATION] ENTRY");
                for (unsigned int i = m_start_address_intern_memory + 8; i <= m_end_address_intern_memory; i += 4) {
                    read(&mydata, i);
                    packet.push_back(mydata);

                    sb_fprintf(stdout, "[COMUNICATION] -> I: %i, TIME: %s, VALUE: %u\n",
                               i, sc_time_stamp().to_string().c_str(), mydata);

                    wait(m_timeout, SC_NS);
                }
                // get crc from packets
                // check crc

                if (check_crc(decode(packet))) {
                    // save pixel in memory global (m_controller set position)
                    sb_fprintf(stdout, "[COMUNICATION] -> CRC @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@, TIME: %s\n"
                              , sc_time_stamp().to_string().c_str());


                    mydata = *(packet.end() - 1);
                    bus_port->direct_write(&mydata, memory_idx);
                    memory_idx += 4;
                }
                else{
                    sb_fprintf(stdout, "[COMUNICATION] -> CRC NNNNNNNNNNNNNNNNNNN, TIME: %s\n"
                              , sc_time_stamp().to_string().c_str());
                }

                // save pixel in memory global (m_controller set position)


                // Generator can work
                mydata = 0;
                write(&mydata, 0);

                packet.clear();

                wait(m_timeout, SC_NS);
            }
            else {
                sb_fprintf(stdout, "[COMUNICATION] TIME: %s, WAITING\n",
                           sc_time_stamp().to_string().c_str());
                wait(m_timeout, SC_NS);
            }
        }
        else {
            sb_fprintf(stdout, "[COMUNICATION] TIME: %s, FINISHED\n",
                       sc_time_stamp().to_string().c_str());

            // Stores the initial position of the image data in global memory
            mydata = m_start_address_intern_memory + 20;
            bus_port->direct_write(&mydata, 0);
            wait(m_timeout, SC_NS);

            sb_fprintf(stdout, "[COMUNICATION] SAVE BEGIN MEMORY: TIME: %s READ FROM: %d VALUE: %d\n",
                       sc_time_stamp().to_string().c_str(),
                       0,
                       mydata);

            // Stores the final position of the image data in global memory
            mydata = memory_idx;
            bus_port->direct_write(&mydata, 4);
            wait(m_timeout, SC_NS);

            sb_fprintf(stdout, "[COMUNICATION] SAVE END MEMORY: TIME: %s READ FROM: %d VALUE: %d\n",
                       sc_time_stamp().to_string().c_str(),
                       4,
                       mydata);


              mydata = 64;
              bus_port->direct_write(&mydata, m_address_width);
              sb_fprintf(stdout, "[COMUNICATION] ON CRIP:  TIME: %s READ FROM: %d VALUE: %d\n",
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
