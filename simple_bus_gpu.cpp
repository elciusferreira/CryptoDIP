#include "simple_bus_gpu.h"
#include "simple_bus_types.h"

void simple_bus_gpu::main_action() {
    int width, height, size;
    int *bufferA = new int[m_max_size];
    int *bufferB = new int[m_max_size];

    if (m_test) {
        for (int i = 1; i <= 6; ++i)
            bufferA[i - 1] = i;
        width = 2;
        height = 3;
    }

    while (true) {
        bus_port->direct_read(&width, m_rd_addr);
        bus_port->direct_read(&height, m_rd_addr + 4);
        size = width * height;

        if (size <= m_max_size) {
            // Fetch image pixels into Buffer A
            for (int i = 0; i < size; ++i)
                bus_port->direct_read(&bufferA[i], m_rd_addr + 8 + i * 4);

            // Write pixels in order to rotate image in Buffer B
            int j = height - 1;
            int i = 0;

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    i = x;
                    // First get a pixel in A
                    int pix = bufferA[pixelAddr(x, y, width)];

                    // Then write it in the new calculated position in B
                    bufferB[pixelAddr(j, i, height)] = pix;
                }
                --j;
            }

            // Write to file
            imageWrite(bufferA, height, width, "original.png");
            imageWrite(bufferB, height, width, "rotated.png");


            // Write back the rotation result
            /***
              The original height is the new width
              The original width is the new height
            ***/
            bus_port->direct_write(&height, m_wr_addr);
            bus_port->direct_write(&width, m_wr_addr + 4);

            for (int k = 0; k < size; k++) {
                bus_port->direct_write(&bufferB[k], m_wr_addr + 8 + i * 4);
            }

            if (m_verbose) {
                sb_fprintf(stdout, "%s %s : mem[%x:%x]=(%x, %x, %x, %x, %x, %x, %x, %x)\n",
                           sc_time_stamp().to_string().c_str(), name(), m_rd_addr, m_rd_addr + 31,
                           width, height, bufferA[0], bufferA[1], bufferA[2], bufferA[3],
                           bufferA[4], bufferA[5], bufferA[6], bufferA[7]);
                sb_fprintf(stdout, "%s %s : mem[%x:%x]=(%x, %x, %x, %x, %x, %x, %x, %x)\n",
                           sc_time_stamp().to_string().c_str(), name(), m_wr_addr, m_wr_addr + 31,
                           height, width, bufferB[0], bufferB[1], bufferB[2], bufferB[3],
                           bufferB[4], bufferB[5], bufferB[6], bufferB[7]);
            }
        } else if (m_verbose) {
            sb_fprintf(stderr, "ERROR: image too big.");
        }

        wait(m_timeout, SC_NS);
    }
}
