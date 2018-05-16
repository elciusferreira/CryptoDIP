#include "generator.h"

int main()
{
    ModuleTest test("sample.png");
    test.encryption();
    test.crcGenerator();
    test.saveFile();
    return 0;
}
