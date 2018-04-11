#ifndef __main2_h
#define __main2_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <opencv2/highgui.hpp>


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

    int getR(){
        return r & 0xff;
    }

    int getG(){
        return g & 0xff;
    }

    int getB(){
        return b & 0xff;
    }

    int getA(){
        return a & 0xff;
    }

    void print(){
        printf("R: %c\nG: %c\nB: %c\nA: %c\n\n", r, g, b, a);
    }
};


class Encryption{
private:
    int s[256];
    int key[5];
    int sizeKey;
    std::vector<Pixel> input;

    void change(int i, int j){
        int aux = s[i];
        s[i] = s[j];
        s[j] = aux;
    }

public:
    Encryption(){
        for(int i=0; i<5 ; i++)
            key[i] = (i*12+70)%256;
        sizeKey = 5;
    }

    Encryption(std::vector<Pixel> input){
        this->input = input;
        for(int i=0; i<5 ; i++)
            key[i] = (i*12+70)%256;
        sizeKey = 5;
    }

    std::vector<Pixel> run(){
        // KSA
        for(int i =0; i< 256; i++){
            s[i]=i;
        }

        int j = 0;
        for(int i = 0 ; i< 256; i++){
            j = (j + s[i] + key[i % sizeKey]) % 256;
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
            g = (s[(s[i] + s[j]) % 256])^input.at(w).getG();
            b = (s[(s[i] + s[j]) % 256])^input.at(w).getB();
            a = (s[(s[i] + s[j]) % 256])^input.at(w).getA();

            output.push_back(Pixel(r,g,b,a));
        }

        return output;
    }

    void setInput(std::vector<Pixel> newInput){
        input= newInput;
    }

};


class ModuleTest{
private:
    Encryption encryptionF;
    std::string path, outputText;
    std::vector<Pixel> elements;
    bool _read;


public:
    ModuleTest(std::string path){
        this->path = path;
        cv::Mat image;
        image = cv::imread(path, CV_LOAD_IMAGE_COLOR);

        if (!image.data){
            std::cerr << "Could not open or find the image." << std::endl;
            _read = false;
        }
        else{
            _read = true;

            printf("The image dimensions are %i x %i pixels.\n\n", image.cols, image.rows);

//          int size = image.rows*image.cols*4;
            unsigned int it = 0;
            char buffer[3];

            for (int y = 0; y < image.rows; ++y) {
                for (int x = 0; x < image.cols; ++x) {
                    buffer[it++] = *(image.ptr(y,x)+2); // R component
                    buffer[it++] = *(image.ptr(y,x)+1); // G component
                    buffer[it] = *image.ptr(y,x);     // B component

                    it = 0;
                    Pixel aux = Pixel(buffer[0], buffer[1], buffer[2], char(0));
//                    aux.print();
                    elements.push_back(aux);
                }
            }
        }
    }

    /* Open image and save in elements */

    void encryption(){
        encryptionF.setInput(elements);
        elements = encryptionF.run();
    }

    /* CRC */
    void crc(){
        for(unsigned int i = 0; i < elements.size(); i++){
            outputText+= "" + std::to_string(elements.at(i).getR());
            outputText+= " " + std::to_string(elements.at(i).getG());
            outputText+= " " + std::to_string(elements.at(i).getB());
            outputText+= " " + std::to_string(elements.at(i).getA());
            outputText+= ", CRC\n";
        }
    }

    // Save file
    void saveFile(){
        std::ofstream output;
        output.open ("ouput.txt");
        output << outputText;
        output.close();
    }
};

#endif

