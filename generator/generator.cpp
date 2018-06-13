#include "generator.h"

int main(int argc, char *argv[])
{
    if (argc != 2){
        std::cout << "\n\tUso: " << argv[0] << " <imagem>\n\n";
        return 0;
    }

    std::cout << "Lendo imagem: " << argv[1] << std::endl;
    ModuleTest test(argv[1]);
    test.encryption();
    // test.decryption();
    // test.crcGenerator();
    test.packetGenerator();
    test.saveFile();

    //test.CreateImageFromFile("output_test.txt");

    //TestCrypt t("output_e.txt");
    //t.decrypt();
    //t.compareFiles("output_d.txt");
    //t.saveFile();
    return 0;
}
