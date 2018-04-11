#ifndef __simple_bus_gpu_h
#define __simple_bus_gpu_h

#include <systemc.h>
#include "simple_bus_direct_if.h"
#include "imageWrite.h"

/**
 - Endereço padrão de leitura de imagens: 0x0080
 - Endereço padrão de escrita de imagens: 0x1090
 - Tamanho max de imagem: 64x64 = 4096 words(4 bytes)
**/

//typedef int pixel;

SC_MODULE(simple_bus_gpu) {
private:
    unsigned int m_rd_addr;
    unsigned int m_wr_addr;
    int m_max_size;
    int m_timeout;
    bool m_verbose;
    bool m_test;

    int pixelAddr(int x, int y, int w) { return x + y * w; };

public:
    sc_in_clk clock;
    sc_port<simple_bus_direct_if> bus_port;

    SC_HAS_PROCESS(simple_bus_gpu);

    explicit simple_bus_gpu(sc_module_name name_)
    {
        m_rd_addr = 0x80;
        m_wr_addr = 0x1090;
        m_max_size = 4096;
        m_timeout = 100;
        m_verbose = false;
        m_test = false;
        SC_THREAD(main_action);
    }

    simple_bus_gpu(sc_module_name name_,
                      unsigned int rd_addr,
                      unsigned int wr_addr,
                      int max_size = 4096,
                      int timeout = 100,
                      bool verbose = false,
                      bool test = false)
            :sc_module(name_),
             m_rd_addr(rd_addr),
             m_wr_addr(wr_addr),
             m_max_size(max_size),
             m_timeout(timeout),
             m_verbose(verbose),
             m_test(test)
    {
        SC_THREAD(main_action);
    }

    void main_action();
    void toggleVerbose() { m_verbose = !m_verbose; };
    void toggleTest() { m_test = !m_test; };
};

#endif
