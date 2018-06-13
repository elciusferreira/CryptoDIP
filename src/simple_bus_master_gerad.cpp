#include "simple_bus_master_gerad.h"
#include "simple_bus_types.h"
#include <fstream>
#include <sstream>
#include <vector>

void printFile(std::string filename, int num) {
    std::cout << "Save File\n";
    std::ofstream outputFile;
    outputFile.open(filename.c_str(), std::ios_base::app);
    outputFile << "numero de linhas: ";
    outputFile << num << "\n";
    outputFile.close();
}

std::vector<std::string> Process_file(std::string file_name) {
    std::vector<std::string> lines;
    std::ifstream input(file_name.c_str());
    std::string line;

    while (getline(input, line))
        lines.push_back(line);

    //printFile("debug.txt", lines.size());
    return lines;
}


int inline stoi(const std::string str) {
    int num;
    std::istringstream(str) >> num;
    return num;
}

int pixelStream(const std::string r,
                const std::string g,
                const std::string b,
                const std::string a)
{
    int _color = 0;
    _color += stoi(r) << 24;
    _color += stoi(g) << 16;
    _color += stoi(b) << 8;
    _color += stoi(a);

    return _color;
}


int crcStream(const std::string crc)
{
    // Concatenating and appending crc code to data stream
    int _char = 0;
    for (unsigned int j = 0; j < 8; ++j) {
        int aux = crc.at(j);
        _char <<= 8;
        aux &= 0x0ff;
        _char += aux;
    }

    return _char;
}


void simple_bus_master_gerad::main_action() {
    if (m_verbose)
        sb_fprintf(stdout, "GENERATOR START!\n");
    int flag;
    int flags;
    int change = 0;


    flag = 0;
    bus_port->direct_write(&flag, m_opflag);

    //std::vector<std::string> file = Process_file("../generator/teste.txt");
    std::vector<std::string> file = Process_file("../resources/output.txt");

    int line_count = 0;

    // READ TXT!!!!
    while (true) {
        bus_port->direct_read(&flag, m_opflag);
        wait(m_timeout, SC_NS);

        if (flag == 1) {  // SE FOR 1 FICA OCIOSO
            if (m_verbose)
                sb_fprintf(stdout, "[GENERATOR] CANT WORK!!!\n");
            continue;
        }

        if (file.size() == 0) {
            bus_port->direct_read(&flags, m_end_flag);
            wait(m_timeout, SC_NS);
            if (flags == 0 && change == 0) {
                flags = 1;
                change = 1;
                bus_port->direct_write(&flags, m_end_flag);
            }
            wait(m_timeout, SC_NS);
            if (m_verbose)
                sb_fprintf(stdout, "[GENERATOR] WAITING!!!\n");
            continue;
        }
        if (m_verbose)
            sb_fprintf(stdout, "[GENERATOR] WORKING!!!\n");

        std::string line = file.at(0);
        file.erase(file.begin());
        unsigned int size = line.length();

        if (m_verbose) {
            sb_fprintf(stdout, "[GENERATOR] @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ LINE: %i!\n", line_count);
            printFile("contador_de_linhas.txt", line_count);
        }

        std::vector<int> packet; // = Gen_stream(crc, r, g, b, a)
        std::string crc;
        int op = 0;

        for (unsigned int j = 0; j < 8; ++j)
            crc += line.at(j);

        packet.push_back(crcStream(crc));

        if (line_count == 0) {  // Caso seja o primeiro pacote, salvamos apenas a largura e altura da imagem.
            std::string w;  // width
            std::string h;  // heigth
            bool isHeight = false;

            for (unsigned int i = 9; i < size; ++i) {
                if (line.at(i) == ',') {
                    if (isHeight)
                        packet.push_back(stoi(h));
                    else
                        packet.push_back(stoi(w));

                    isHeight = true;
                    continue;
                }

                if (isHeight)
                    h += line.at(i);
                else
                    w += line.at(i);

            }
        }
        else {
            std::string r;
            std::string g;
            std::string b;
            std::string a;

            for (unsigned int i = 9; i < size; ++i) {
                if (line.at(i) == ',') {
                    op++;
                    if (op == 4) {
                        op = 0;
                        r = "";
                        g = "";
                        b = "";
                        a = "";
                        packet.push_back(pixelStream(r, g, b, a));
                    }
                    continue;
                }

                switch (op) {
                    case 0:
                        r += line.at(i);
                        break;
                    case 1:
                        g += line.at(i);
                        break;
                    case 2:
                        b += line.at(i);
                        break;
                    case 3:
                        a += line.at(i);
                        break;
                    default:
                        break;
                }
            }
        }

        // SAVE IN MEMORY
        int value;
        unsigned int address = m_address_start + 8;
        for (std::vector<int>::iterator it = packet.begin(); it != packet.end(); it++) {
            value = *it;
            bus_port->direct_write(&value, address);
            wait(m_timeout, SC_NS);

            if (m_verbose)
                sb_fprintf(stdout, "[GENERATOR] WRITE-> TIME: %s READ FROM: %d VALUE: %d\n",
                           sc_time_stamp().to_string().c_str(),
                           address,
                           value);

            address += 4;
        }

        flag = 1;
        bus_port->direct_write(&flag, m_opflag);
        wait(m_timeout, SC_NS);
        line_count++;
    }
}
