#include "generator.h"

int main()
{
    ModuleTest test("sample.png");
    test.encryption();
    //test.decryption();
    test.crcGenerator();
    test.saveFile();

    //TestCrypt t("output_e.txt");
    //t.decrypt();
    //t.compareFiles("output_d.txt");
    //t.saveFile();
    return 0;
}
