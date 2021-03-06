#ifndef __generator_h
#define __generator_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <openssl/sha.h>
#include <string>


class Pixel{
private:
    char r, g, b, a;

public:
    Pixel(char r, char g, char b, char a){
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    explicit Pixel(unsigned int fullNumber){
        r = (fullNumber >> 24) & 0xff;
        g = (fullNumber >> 16) & 0xff;
        b = (fullNumber >> 8) & 0xff;
        a = (fullNumber) & 0xff;
    }

    unsigned int convertToFullNumber(){
        return ((r << 24) & 0xFF000000) + ((g << 16) & 0x00FF0000) + ((b << 8) & 0x0000FF00) + (a & 0x000000FF);
    }

    unsigned int getR(){
        return r & 0xff;
    }

    unsigned int getG(){
        return g & 0xff;
    }

    unsigned int getB(){
        return b & 0xff;
    }

    unsigned int getA(){
        return a & 0xff;
    }

    void print(){
//        printf("R: %c\nG: %c\nB: %c\nA: %c\n\n", r, g, b, a);
        //std::cout << (r & 0xff) << " " << (g & 0xff) << " " << (b & 0xff) << " " << (a & 0xff) << "\n";
    }
};


class Encryption{
private:
    int s[256];
    int key_c[9];
    int size_key;
    std::vector<Pixel> input;

    void change(int i, int j){
        int aux = s[i];
        s[i] = s[j];
        s[j] = aux;
    }

public:
    Encryption(){
        key_c[0] = 99;
        key_c[1] = 111;
        key_c[2] = 112;
        key_c[3] = 114;
        key_c[4] = 111;
        key_c[5] = 106;
        key_c[6] = 101;
        key_c[7] = 116;
        key_c[8] = 111;
        size_key = 9;
    }

    Encryption(std::vector<Pixel> input){
        this->input = input;
        key_c[0] = 99;
        key_c[1] = 111;
        key_c[2] = 112;
        key_c[3] = 114;
        key_c[4] = 111;
        key_c[5] = 106;
        key_c[6] = 101;
        key_c[7] = 116;
        key_c[8] = 111;
        size_key = 9;
    }

    std::vector<Pixel> run(){
        // KSA
        for(int i =0; i< 256; i++){
            s[i]=i;
        }

        int j = 0;
        for(int i = 0 ; i< 256; i++){
            j = (j + s[i] + key_c[i % size_key]) % 256;
            change(i, j);
        }

        // PRGA
        std::vector<Pixel> output;
        j=0;
        int i=0;
        char r,g,b,a;
        for (unsigned int w = 0; w < input.size() ; w++){
            i = (i+1)%256;
            j = (j+s[i])%256;
            change(i,j);
            r = (s[(s[i] + s[j]) % 256])^(input.at(w).getR());
            g = (s[(s[i] + s[j]) % 256])^(input.at(w).getG());
            b = (s[(s[i] + s[j]) % 256])^(input.at(w).getB());
            a = (s[(s[i] + s[j]) % 256])^(input.at(w).getA());

            output.push_back(Pixel(r,g,b,a));
        }

        return output;
    }


    std::vector<int> run(std::vector<int> array_test){
        for(int k = 0; k < size_key; k++){
            std::cout<< " "<< key_c[k];
        }
        // KSA
        for(int i =0; i< 256; i++){
            s[i]=i;
        }

        int j = 0;
        for(int i = 0 ; i< 256; i++){
            j = (j + s[i] + key_c[i % size_key]) % 256;
            if(i<11)
                std::cout<< "J: "<< j<< " S: "<< s[i]
                         << " K: " << key_c[i % size_key]<<
                         " ALL: "<< (j + s[i] + key_c[i % size_key]) <<"\n";
            change(i, j);
        }

        // PRGA
        std::vector<int> output;
        j=0;
        int i=0;
        int r;
        int position;
        for (unsigned int w = 0; w < 11; w++){
            i = (i+1)%256;
            j = (j+s[i])%256;
            std::cout << "I: "<< i << " J: "<<j;
            change(i,j);
            std::cout<< " Value: "<< array_test.at(w);
            position = (s[i] + s[j]) % 256;
            std::cout<< " Position Change: "<< position;
            r = (s[position])^(array_test.at(w));
            std::cout<< " Descryption: " << r<<"\n";
            output.push_back(r);
        }

        return output;
    }

    void setInput(std::vector<Pixel> newInput){
        input= newInput;
    }

};


class TestCrypt{
private:
    Encryption module_crip;
    std::vector<int> array_file;
    std::vector<int> array_test;
    std::string outputFile;

public:
    TestCrypt(std::string file_name){
        std::cout << "Get File enpytion\n";
        std::ifstream input(file_name.c_str());
        std::string line;

        while (getline(input, line)){
            array_file.push_back(std::stoi(line));
        }

    }

    void compareFiles(std::string file_dec){
        std::cout<<"Compare Files\n";
        std::ifstream input(file_dec.c_str());
        std::string line;

        std::vector<int> file_d;
        while (getline(input, line)){
            file_d.push_back(std::stoi(line));
            //std::cout<< std::stoi(line)<<"\n";
        }

        for(unsigned int i=0; i< file_d.size(); i++){
            if(array_test.at(i) != file_d.at(i)){
                std::cout<< " FAil";
                break;
            }
        }
    }

    void decrypt(){
        std::cout<<"Decryption\n";
        array_test = module_crip.run(array_file);
    }
    void saveFile(){
        std::cout<<"Save decryption\n";
        outputFile.clear();
        for(unsigned int i=0; i< array_test.size(); i++){
            outputFile += std::to_string(array_test.at(i)) + "\n";

        }
        std::ofstream output;
        output.open ("output_testCrp.txt");
        output << outputFile;
        output.close();
    }
};



class ModuleTest{
private:
    Encryption encryptionF;
    std::string path, outputText;
    std::vector<Pixel> elements;
    bool _read;
    int image_width;
    int image_height;

public:
    ModuleTest(std::string path){
        std::cout << "Open file\nGetImageToPixels\n";
        this->path = path;
        cv::Mat image;
        image = cv::imread(path, CV_LOAD_IMAGE_COLOR);

        if (!image.data){
            std::cerr << "Could not open or find the image." << std::endl;
            _read = false;
            exit(-1);
        }
        else{
            _read = true;   // ?????????????????????????????????????????????????
            image_width = image.cols;
            image_height = image.rows;
            elements.push_back(Pixel(image_width));
            elements.push_back(Pixel(image_height));

            //printf("The image dimensions are %i x %i pixels.\n\n", image.cols, image.rows);

            unsigned int it = 0;
            char buffer[3];

            for (int y = 0; y < image.rows; ++y) {
                for (int x = 0; x < image.cols; ++x) {
                    buffer[it++] = *(image.ptr(y,x)+2); // R component
                    buffer[it++] = *(image.ptr(y,x)+1); // G component
                    buffer[it] = *image.ptr(y,x);     // B component

                    it = 0;
                    Pixel aux = Pixel(buffer[0], buffer[1], buffer[2], char(0));
                    aux.print();
                    elements.push_back(aux);
                }
            }
        }
    }

    void encryption(){
        std::cout<<"Encryption\n";
        encryptionF.setInput(elements);
        elements = encryptionF.run();
    }

    void decryption(){
        encryptionF.setInput(elements);
        elements = encryptionF.run();
    }

    /* CRC */
    void crcGenerator(){
        std::cout<<"CRC\n";
        unsigned int size_pixels = elements.size();

        for(unsigned int i = 0; i < size_pixels; i++){
            //printf("volta %u\n", i);
            //elements.at(i).print();

            std::string pixels;
            pixels += char(elements[i].getR());
            pixels += char(elements[i].getG());
            pixels += char(elements[i].getB());
            pixels += char(elements[i].getA());

            const char *components = pixels.c_str();

            //printf("Components: [%u %u %u %u]\n", components[0] & 0xff, components[1] & 0xff, components[2] & 0xff, components[3] & 0xff);

            unsigned char digest[SHA256_DIGEST_LENGTH];

            SHA256_CTX ctx;
            SHA256_Init(&ctx);
            SHA256_Update(&ctx, components, strlen(components));
            SHA256_Final(digest, &ctx);

            char* SHAString = new char[SHA256_DIGEST_LENGTH*2+1];
            for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++)
                sprintf(&SHAString[i*2], "%02x", (unsigned int)digest[i]);

            outputText += std::string(SHAString) +  ","
                       + std::to_string(elements[i].getR()) + ","
                       + std::to_string(elements[i].getG()) + ","
                       + std::to_string(elements[i].getB()) + ","
                       + std::to_string(elements[i].getA()) + "\n";


            //outputText += std::to_string(elements[i].getR()) + "\n";
        }
    }

    // Save file
    void saveFile(){
        std::cout<<"Save File\n";
        std::ofstream output;
        output.open ("output_test.txt");
        output << outputText;
        output.close();
    }

    void CreateImageFromFile(std::string file_name) {
        std::vector<std::string> lines;
        std::ifstream input(file_name.c_str());
        std::string l;
        std::string crc, r, g, b, a;

        while (getline(input, l))
            lines.push_back(l);

        int height = 64;
        int width = 64;
        int size = height * width;
        char *data = new char[size*3];
        int op = 0;

        for (unsigned int line = 0; line < lines.size(); ++line){
            for (unsigned int i = 0; i < lines.at(line).length(); ++i) {
                if (lines.at(line).at(i) == ',') {
                    op++;
                    continue;
                }
                switch (op) {
                    case 0:
                        // std::cout << "CRC\n";
                        crc += lines.at(line).at(i);
                        break;
                    case 1:
                        // std::cout << "R\n";
                        r += lines.at(line).at(i);
                        break;
                    case 2:
                        // std::cout << "G\n";
                        g += lines.at(line).at(i);
                        break;
                    case 3:
                        // std::cout << "B\n";
                        b += lines.at(line).at(i);
                        break;
                    case 4:
                        // std::cout << "A\n";
                        a += lines.at(line).at(i);
                        break;
                    default:
                        break;
                }
            }
            // aqui

            std::cout << "Li a linha " << lines.at(line) << std::endl;
            std::cout << "R: " << r << std::endl;
            std::cout << "G: " << g << std::endl;
            std::cout << "B: " << b << std::endl << std::endl;

            data[line*3] = char(std::atoi(b.c_str()) & 0xff);  // Blue component
            data[line*3 +1] = char(std::atoi(g.c_str()) & 0xff); // Green component
            data[line*3 +2] = char(std::atoi(r.c_str()) & 0xff); // Red component
            op = 0;
            r = "";
            g = "";
            b = "";
            a = "";
        }

        std::string file = "imagemm.jpg";
        cv::Mat src = cv::Mat(width, height, CV_8UC3, data);
        cv::imwrite(file, src);
    }
};

#endif
