#include "simple_bus_encryption.h"
#include "simple_bus_types.h"

std::vector<int> unpack(int pixel){
    std::vector<int> ret;
    for (int i = 0; i < 4; ++i)
        ret.push_back(pixel >> ((3-i) * 8) & 0xff);

    return ret;
}

int pack(std::vector<int> pix){
    int ret= 0;
    for (int i = 0; i < 4 ; ++i){
        ret += pix[i] << (8 * (3-i));
    }
    return ret;
}

int stringToint(const std::string str){
    //std::string str = "123";
    int num;
    std::istringstream(str) >> num;
    return num;
}

std::string intTostring(int number){
    std::stringstream ss;
	  ss << number;
	  return ss.str();
}

void simple_bus_encryption::main_action() {
    int control;

    while (true) {
        bus_port->direct_read(&control, m_address_reserved);
        wait(m_timeout, SC_NS);
        if(m_verbose)
            sb_fprintf(stdout, "[CRIPT] TIME: %s VALUE: %d ", sc_time_stamp().to_string().c_str(), control);

        if (control == 1) {
            if(m_verbose)
                sb_fprintf(stdout, " CAN WORK!!!\n");
            control = 0;
            bus_port->direct_write(&control, m_address_reserved);
            wait(m_timeout, SC_NS);

            getRange();
            if(m_verbose)
                sb_fprintf(stdout, "[MEMORY] BEGIN: %d END: %d FLAG: %d!!!\n", address_read_start, address_read_end, m_address_reserved);

            //openFileAndSaveMemory();

            //seeMemory();
            KSA();
            PRGA();

            //compareResult();

            control = 1;
            bus_port->direct_write(&control, m_address_graphs);
            wait(m_timeout, SC_NS);
            if(m_verbose)
                sb_fprintf(stdout, "[CRIPT] WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n",
                       sc_time_stamp().to_string().c_str(),
                       m_address_graphs,
                       control);

            control = 24;
            bus_port->direct_write(&control, m_address_start);
            wait(m_timeout, SC_NS);


        } else {
            if(m_verbose)
                sb_fprintf(stdout, " CANT WORK!!!\n");
        }
    }
}

void simple_bus_encryption::getRange() {
    int value;
    bus_port->direct_read(&value, m_address_start);
    wait(m_timeout, SC_NS);
    address_read_start = value;

    bus_port->direct_read(&value, m_address_end);
    wait(m_timeout, SC_NS);
    address_read_end = value;

    if(m_verbose)
        sb_fprintf(stdout, "BEGIN: %d END: %d RESERVED: %d!!!\n", address_read_start, address_read_end, m_address_reserved);
}

void simple_bus_encryption::KSA() {
    if(m_verbose)
        sb_fprintf(stdout, "[CRIPT] KSA BEGIN\n");
    for(int i =0; i< 256; i++){
        s[i]=i;
    }

    int j = 0;
    for(int i = 0 ; i< 256; i++){
        j = (j + s[i] + key_c[i % size_key]) % 256;
        change(i, j);
    }
    if(m_verbose)
        sb_fprintf(stdout, "[CRIPT] KSA END\n");
}

void simple_bus_encryption::PRGA() {
    if(m_verbose)
        sb_fprintf(stdout, "[CRIPT] PRGA BEGIN\n");

    int mydata;
    int j=0;
    int i=0;
    int temp;
    int descryption;
    std::vector<int> pixels, input, output, line;
    for (unsigned int w = address_read_start; w < address_read_end ; w+= 4){
        i = (i+1)%256;
        j = (j+s[i])%256;
        change(i,j);
        bus_port->direct_read(&mydata, w);
        wait(m_timeout, SC_NS);
        input.push_back(mydata);

        pixels = unpack(mydata);

        for(unsigned int k = 0; k < pixels.size(); k++){
          temp = (s[(s[i] + s[j]) % 256])^(pixels.at(k));
          input.push_back(pixels.at(k));
          line.push_back((s[(s[i] + s[j]) % 256]));
          output.push_back(temp);
          pixels.at(k) = temp;
        }


        descryption = pack(pixels);
        output.push_back(descryption);

        // Only 1 channel
        //descryption = (s[(s[i] + s[j]) % 256])^(mydata);

        // Test Error
        //if(w==16380)
        //  descryption = -8;
        bus_port->direct_write(&descryption, w);
        wait(m_timeout, SC_NS);
        if(m_verbose)
            sb_fprintf(stdout, "[CRIPT] DECRYPTION !!!!!!\n");
    }
    if(m_verbose) {
        saveFile("./teste/line.txt", line, 4);
        saveFile("./teste/entrada.txt", input, 5);
        saveFile("./teste/saida.txt", output, 5);

        sb_fprintf(stdout, "[CRIPT] PRGA END\n");
    }
}

void simple_bus_encryption::change(int i,int j) {
    int aux = s[i];
    s[i] = s[j];
    s[j] = aux;
}

void simple_bus_encryption::saveFile(std::string filename, std::vector<int> input, int split){
    std::cout<<"Save File\n";
    std::ofstream outputFile;
    outputFile.open (filename.c_str());
    std::string outputText= "";
    for(unsigned int i=0; i < input.size(); i++){
      if(i%split == 0){
        outputText+= "\n";
      }
      outputText+= " " + intTostring(input.at(i));
    }
    outputFile << outputText;

    outputFile.close();
}

void simple_bus_encryption::compareResult(){
    std::vector<int> lines;
    std::string path = "../generator/output_d.txt";
    std::ifstream input(path.c_str());
    std::string line;

    while (getline(input, line)){
        lines.push_back(stringToint(line));
    }

    int data;
    int k = 0;
    for(unsigned i = address_read_start; i <= address_read_end; i+= 4){
        bus_port->direct_read(&data, i);
        if(data != lines.at(k))
            while(true)
              std::cout<<"Err\n";
        k++;
    }
}

void simple_bus_encryption::openFileAndSaveMemory(){
        std::vector<int> lines;
        std::string path = "../generator/output_e.txt";
        std::ifstream input(path.c_str());
        std::string line;

        while (getline(input, line)){
            lines.push_back(stringToint(line));
        }
        address_read_start = 20;
        address_read_end = (lines.size()*4);
        std::cout<<"MEMORY MAX: "<< address_read_end <<"\n";
        int data;

        int k = 0;
        for(unsigned i = address_read_start; i <= address_read_end; i+= 4){
            data = lines.at(k);
            k++;
            bus_port->direct_write(&data, i);
        }
}



void simple_bus_encryption::seeMemory() {
    int values;
    sb_fprintf(stdout, "[CRIPT] START SEE MEMORY\n");
    for (unsigned int i = address_read_start; i <= address_read_end; i = i + 4) {
        bus_port->direct_read(&values, i);
        sb_fprintf(stdout, "M-> %s MEMORY: %d  VALUE:%d\n", sc_time_stamp().to_string().c_str(), i, values);
        wait(0, SC_NS);
    }
    sb_fprintf(stdout, "[CRIPT] FINISH SEE MEMORY\n");
}
