#include "simple_bus_gpu.h"
#include "simple_bus_types.h"

void simple_bus_gpu::main_action() {
    int width, height, size, start, rd_addr;
    int *bufferA = new int[m_max_size];
    int *bufferB = new int[m_max_size];

    while (true) {
        bus_port->direct_read(&start, flag_addr);
        sb_fprintf(stdout, "[GPU] TIME: %s VALUE: %d ", sc_time_stamp().to_string().c_str(), start);


        if (start == 1) {
            sb_fprintf(stdout, "[GPU] TIME: %s VALUE: %d\n", sc_time_stamp().to_string().c_str(), start);

            bus_port->direct_read(&rd_addr, im_addr);
            bus_port->direct_read(&width, rd_addr);
            bus_port->direct_read(&height, rd_addr + 4);
            wait(m_timeout/2, SC_NS);
            size = width * height;
            rd_addr += 8;

            if (size <= m_max_size) {
                // Fetch image pixels into Buffer A
                for (int i = 0; i < size; i += 4) {
                    bus_port->direct_read(&bufferA[i], rd_addr + i*4);
                    bus_port->direct_read(&bufferA[i+4], rd_addr + i*4 + 4);
                    bus_port->direct_read(&bufferA[i+8], rd_addr + i*4 + 8);
                    bus_port->direct_read(&bufferA[i+12], rd_addr + i*4 + 12);
                    wait(m_timeout, SC_NS);
                }

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
                sb_fprintf(stdout, "[GPU] ROTATION GO!\n");
                // Write to file
                if (m_test) {
                    imageWrite(bufferA, height, width, "original.png");
                    imageWrite(bufferB, height, width, "rotated.png");
                }

                // Write back the rotation result
                /***
                  The original height is the new width
                  The original width is the new height
                ***/
                bus_port->direct_write(&height, wr_addr);
                bus_port->direct_write(&width, wr_addr + 4);
                wait(m_timeout/2, SC_NS);
                wr_addr += 8;

                for (int k = 0; k < size; k += 4) {
                    bus_port->direct_write(&bufferB[k], wr_addr + i*4);
                    bus_port->direct_write(&bufferB[k], wr_addr + i*4 + 4);
                    bus_port->direct_write(&bufferB[k], wr_addr + i*4 + 8);
                    bus_port->direct_write(&bufferB[k], wr_addr + i*4 + 12);
                    wait(m_timeout, SC_NS);
                }

                if (m_verbose) {
                    sb_fprintf(stdout, "[GPU] %s %s : mem[%x:%x]=(%x, %x, %x, %x, %x, %x, %x, %x)\n",
                               sc_time_stamp().to_string().c_str(), name(), rd_addr, rd_addr + 31,
                               width, height, bufferA[0], bufferA[1], bufferA[2], bufferA[3],
                               bufferA[4], bufferA[5], bufferA[6], bufferA[7]);
                    sb_fprintf(stdout, "[GPU] %s %s : mem[%x:%x]=(%x, %x, %x, %x, %x, %x, %x, %x)\n",
                               sc_time_stamp().to_string().c_str(), name(), wr_addr, wr_addr + 31,
                               height, width, bufferB[0], bufferB[1], bufferB[2], bufferB[3],
                               bufferB[4], bufferB[5], bufferB[6], bufferB[7]);
                }
            } else if (m_verbose) {
                sb_fprintf(stderr, "[GPU] ERROR: image too big.\n");
            }
        }
        else
            sb_fprintf(stdout, "[GPU] CANT WORK!!!\n");

        wait(m_timeout, SC_NS);
    }
}
