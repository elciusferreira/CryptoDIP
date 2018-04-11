#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/highgui.hpp>

class Pixel{
private:
    char r,g,b,a;

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
        for (int w = 0; w < input.size() ; w++){
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
    std::string path;
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

//            int size = image.rows*image.cols*4;
            unsigned int it = 0;
            char buffer[3];

            for (int y = 0; y < image.rows; ++y) {
                for (int x = 0; x < image.cols; ++x) {
                    buffer[it++] = *(image.ptr(y,x)+2); // R component
                    buffer[it++] = *(image.ptr(y,x)+1); // G component
                    buffer[it] = *image.ptr(y,x);     // B component

                    it = 0;
                    elements.push_back(Pixel(buffer[0], buffer[1], buffer[2], '0'));
                }
            }



        }
    }

    /* Open image and save in elements */

    void encryption(){


    }


};


//int main(){
//    // TEST PIXEL
//    Pixel p(50,100,150,200);
//    //std::cout<< p.convertToFullNumber()<<"\n";
//    Pixel p2(3358745750);
//    std::cout<< p2.getR()<< " "<<p2.getG()<< " "<<p2.getB()<< " "<<p2.getA()<<"\n";
//
//    // TESTE ENCRYPTION
//    std::vector<Pixel> arrayPixels;
//    arrayPixels.push_back(p);
//    arrayPixels.push_back(p2);
//    //std::cout<< arrayPixels.at(1).getR()<< "\n";
//
//    // IDA
//    Encryption encryption(arrayPixels);
//    arrayPixels = encryption.run();
//    //std::cout<< arrayPixels.at(0).getR()<< "\n";
//
//    // VOLTA
//    encryption.setInput(arrayPixels);
//    arrayPixels = encryption.run();
//    //std::cout<< arrayPixels.at(0).getR()<<"\n";
//
//
//    return 0;
//}
