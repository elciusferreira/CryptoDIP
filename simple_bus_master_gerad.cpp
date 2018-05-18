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

    /*std::string test1 = "42a5fac04ad74c7274d1994cb3a576d0975dbab36e1c9841ac0be9c99702f5fd,75,75,75,75";
    for (int i = 0; i < 1 ;  i++){
        lines.push_back(test1);
    }*/

    return lines;
}


int inline stoi(const std::string str){
    //std::string str = "123";
    int num;
    std::istringstream(str) >> num;
    return num;
}


std::vector<int> Gen_stream(const std::string crc,
                            const std::string r,
                            const std::string g,
                            const std::string b,
                            const std::string a)
{
    std::vector<int> Data;

    // Concatenating and appending crc code to data stream
    for (unsigned int i = 0; i < 64; i += 4) {
        int _char = 0;
        for (unsigned int j = i; j < i+4; ++j) {
            int aux = crc.at(j);
            _char <<= 8;
            aux &= 0x0ff;
            _char += aux;
        }

        Data.push_back(_char);
    }

    //sb_fprintf(stdout, "colors <rgba>: [%i, %i, %i, %i]\n", stoi(r), stoi(b), stoi(g), stoi(a));
    //Appending the pixel
    int _color = 0;
    _color += stoi(r) << 24;
    _color += stoi(g) << 16;
    _color += stoi(b) << 8;
    _color += stoi(a);

    //sb_fprintf(stdout, "colors <rgba>: [%u]\n", _color);

    Data.push_back(_color);

//    for (std::vector<int>::iterator it = Data.begin(); it != Data.end(); it++) {
//        sb_fprintf(stdout, "[GREAD] - %i\n", *it);
//
//        char val = (*it >> 8 * 3) & 0xff;
//        sb_fprintf(stdout, "[VRAU] %c\n", val);
//
//        val = (*it >> 8 * 2) & 0xff;
//        sb_fprintf(stdout, "[VRAU] %c\n", val);
//
//        val = (*it >> 8 * 1) & 0xff;
//        sb_fprintf(stdout, "[VRAU] %c\n", val);
//
//        val = *it & 0xff;
//        sb_fprintf(stdout, "[VRAU] %c\n", val);
//    }

    return Data;
}


void simple_bus_master_gerad::main_action() {
    sb_fprintf(stdout, "GENERATOR START!\n");
    int flag;
    int flags;

    flag = 0;
    bus_port->direct_write(&flag, 0);

    //std::vector<std::string> file = Process_file("../generator/teste.txt");
    std::vector<std::string> file = Process_file("../generator/output.txt");


//    newValue = 0;
//    bus_port->direct_write(&newValue, 0);
    int line_size = 1;
    // READ TXT!!!!
    while (true) {
        bus_port->direct_read(&flag, 0);
        //sb_fprintf(stdout, "[GENERATIONS] TIME: %s VALUE: %d\n", sc_time_stamp().to_string().c_str(), flag);
        if (flag == 1) {  // SE FOR 1 FICA OCIOSO
            sb_fprintf(stdout, "[GENERATOR] CANT WORK!!!\n");
            wait(m_timeout, SC_NS);
            continue;
        }

        // e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855,0,0,0,0
        //std::string line = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855,0,0,0,0";
        if (file.size() == 0) {
            flags = 1;
            bus_port->direct_write(&flags, 4);
            wait(m_timeout*200, SC_NS);
        }
        std::string line = file.at(0);
        file.erase(file.begin());
        int size = line.length();
        sb_fprintf(stdout, "[GENERATOR] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ LINE: %i!\n", line_size++);
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
                default:
                    break;
            }
        }
        //    sb_fprintf(stdout, "[GENERATOR] CRC -> %s\n", crc.c_str());
        //    sb_fprintf(stdout, "[GENERATOR] R -> %s\n", r.c_str());
        //    sb_fprintf(stdout, "[GENERATOR] G -> %s\n", g.c_str());
        //    sb_fprintf(stdout, "[GENERATOR] B -> %s\n", b.c_str());
        //    sb_fprintf(stdout, "[GENERATOR] A -> %s\n", a.c_str());

        std::vector<int> data = Gen_stream(crc, r, g, b, a);

        // SAVE IN MEMORY
        int value;
        unsigned int address = m_address_start + 8;
        for (std::vector<int>::iterator it = data.begin(); it != data.end(); it++){
            value = *it;
            bus_port->direct_write(&value, address);

            sb_fprintf(stdout, "[GENERATOR] WRITE-> TIME: %s READ FROM: %d VALUE: %d\n",
                       sc_time_stamp().to_string().c_str(),
                       address,
                       value);

            address += 4;
            wait(m_timeout, SC_NS);
        }
        flag = 1;
        bus_port->direct_write(&flag, 0);
        wait(m_timeout, SC_NS);
    }
}
