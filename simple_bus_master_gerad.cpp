#include "simple_bus_master_gerad.h"
#include "simple_bus_types.h"
#include <fstream>
#include <sstream>
#include <vector>

std::vector<std::string> Process_file(std::string file_name)
{
    std::vector<std::string> lines;
    std::ifstream input(file_name.c_str());
    std::string line;

    while (getline(input, line)){
        //sb_fprintf(stdout, "%s\n", line.c_str());
        lines.push_back(line);
    }

    return lines;
}


void simple_bus_master_gerad::main_action() {
    sb_fprintf(stdout, "GENERATOR START!\n");
    int newValue, flag;
    unsigned int newAddress;

    std::vector<std::string> file = Process_file("../generator/ouput.txt");

    newValue = 0;
    bus_port->direct_write(&newValue, 0);

    // READ TXT!!!!
    while (true) {
        bus_port->direct_read(&flag, 0);
        sb_fprintf(stdout, "[GENERATIONS] TIME: %s VALUE: %d\n", sc_time_stamp().to_string().c_str(), flag);
        if (flag == 1) {  // SE FOR 1 FICA OCIOSO
            wait(m_timeout, SC_NS);
            continue;
        }

        // e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855,0,0,0,0
        //std::string line = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855,0,0,0,0";
        std::string line = file.at(0);
        file.erase(file.begin());
        int size = line.length();

        std::string crc;
        std::string r;
        std::string g;
        std::string b;
        std::string a;
        int op = 0;

        for (int i = 0; i < size; ++i) {
            if (line.at(i) == ',') {
                op++;
                continue;
            }

            switch (op) {
                case 0:
                    crc += line.at(i);
                    break;
                case 1:
                    r += line.at(i);
                    break;
                case 2:
                    g += line.at(i);
                    break;
                case 3:
                    b += line.at(i);
                    break;
                case 4:
                    a += line.at(i);
                    break;
            }
        }
        //    sb_fprintf(stdout, "[GENERATOR] CRC -> %s\n", crc.c_str());
        //    sb_fprintf(stdout, "[GENERATOR] R -> %s\n", r.c_str());
        //    sb_fprintf(stdout, "[GENERATOR] G -> %s\n", g.c_str());
        //    sb_fprintf(stdout, "[GENERATOR] B -> %s\n", b.c_str());
        //    sb_fprintf(stdout, "[GENERATOR] A -> %s\n", a.c_str());

        std::string str = "123";
        int numb;
        std::istringstream(str) >> numb;

        for (unsigned int i = 0; i < 64; i += 4) {
            int _char = 0;
            for (unsigned int j = i; j < i+4; ++j) {
                int aux = crc.at(j);
                _char <<= 8;
                aux &= 0x0ff;
                _char += aux;
            }

//            char val = (_char >> 8*3) & 0xff;
//            sb_fprintf(stdout, "[VRAU](%i) - %c\n", i, val);
//
//            val = (_char >> 8*2) & 0xff;
//            sb_fprintf(stdout, "[VRAU](%i) - %c\n", i, val);
//
//            val = (_char >> 8*1) & 0xff;
//            sb_fprintf(stdout, "[VRAU](%i) - %c\n", i, val);
//
//            val = _char & 0xff;
//            sb_fprintf(stdout, "[VRAU](%i) - %c\n", i, val);

            sb_fprintf(stdout, "[GREAD] - %i\n", _char);
            bus_port->direct_write(&_char, i*4 + 4);
        }
        break;
        // SAVE MEMORY
        for (unsigned int i = m_address_start + 4; i < m_address_end; i += 4) {
            newValue = 1;
            newAddress = i;

            // for
            bus_port->direct_write(&newValue, newAddress);

            sb_fprintf(stdout, "[GENERATOR] WRITE-> TIME: %s READ FROM: %d VALUE: %d\n",
                       sc_time_stamp().to_string().c_str(),
                       newAddress,
                       newValue);


            // write

            wait(m_timeout, SC_NS);
        }
        newValue = 1;
        bus_port->direct_write(&newValue, 0);
    }
}
