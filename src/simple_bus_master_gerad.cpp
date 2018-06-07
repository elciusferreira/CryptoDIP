#include "simple_bus_master_gerad.h"
#include "simple_bus_types.h"
#include <fstream>
#include <sstream>
#include <vector>

void printFile(std::string filename, int num){
    std::cout << "Save File\n";
    std::ofstream outputFile;
    outputFile.open (filename.c_str(), std::ios_base::app);
    outputFile << "numero de linhas: ";
    outputFile << num << "\n";
    outputFile.close();
}

std::vector<std::string> Process_file(std::string file_name)
{
    std::vector<std::string> lines;
    std::ifstream input(file_name.c_str());
    std::string line;

    while (getline(input, line))
        lines.push_back(line);

    printFile("debug.txt", lines.size());
    return lines;
}


int inline stoi(const std::string str){
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

    return Data;
}


void simple_bus_master_gerad::main_action() {
    sb_fprintf(stdout, "GENERATOR START!\n");
    int flag;
    int flags;
    int change = 0;

    flag = 0;
    bus_port->direct_write(&flag, 0);

    //std::vector<std::string> file = Process_file("../generator/teste.txt");
    std::vector<std::string> file = Process_file("../resources/output.txt");

//    newValue = 0;
//    bus_port->direct_write(&newValue, 0);
    int line_count = 0;
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
            bus_port->direct_read(&flags, 4);
            if(flags == 0 && change == 0){
              flags = 1;
              change = 1;
              bus_port->direct_write(&flags, 4);
            }
            wait(m_timeout*200, SC_NS);
            sb_fprintf(stdout, "[GENERATOR] WAITING!!!\n");
            continue;
        }
        sb_fprintf(stdout, "[GENERATOR] WORKING!!!\n");
        std::string line = file.at(0);
        file.erase(file.begin());
        int size = line.length();
        sb_fprintf(stdout, "[GENERATOR] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ LINE: %i!\n", line_count++);
        printFile("contador_de_linhas.txt", line_count);
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
