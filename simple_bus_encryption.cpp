/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  simple_bus_master_direct.cpp : The monitor (master) using the direct BUS
                                 interface.

  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#include "simple_bus_encryption.h"
#include "simple_bus_types.h"
#include <vector>


std::vector<int> unpack(int pixel){
    std::vector<int> ret;
    for (int i = 0; i < 4; ++i)
        ret.push_back(pixel >> ((3-i) * 8) & 0xff);

    return ret;
}

unsigned int pack(std::vector<int> pix){
    unsigned int ret= 0;
    for (int i = 0; i < 4 ; ++i)
        ret += (pix[i] << (8 * (3-i))) & 0xff;

    return ret;
}

void simple_bus_encryption::main_action() {
    int *control = new int;

    while (true) {
        bus_port->direct_read(control, m_address_reserved);
        sb_fprintf(stdout, "[CRIPT] TIME: %s VALUE: %d ", sc_time_stamp().to_string().c_str(), *control);

        if (*control == 1) {
            sb_fprintf(stdout, " CAN WORK!!!\n");
            *control = 0;
            bus_port->direct_write(control, m_address_reserved);


            getRange();

            seeMemory();
            KSA();
            PRGA();

            //seeMemory();
            //KSA();
            //PRGA();

            seeMemory();

            *control = 1;
            bus_port->direct_write(control, m_address_graphs);
            sb_fprintf(stdout, "[CRIPT] WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n",
                       sc_time_stamp().to_string().c_str(),
                       m_address_graphs,
                       *control);
            wait(m_timeout, SC_NS);

        } else {
            sb_fprintf(stdout, " CANT WORK!!!\n");
        }
        wait(m_timeout, SC_NS);
    }
}

void simple_bus_encryption::getRange() {
    int *value = new int;
    bus_port->direct_read(value, m_address_start);
    address_read_start = *value;

    bus_port->direct_read(value, m_address_end);
    address_read_end = *value;

    sb_fprintf(stdout, "BEGIN: %d END: %d RESERVED: %d!!!\n", address_read_start, address_read_end, m_address_reserved);
    delete value;
}

void simple_bus_encryption::KSA() {
    sb_fprintf(stdout, "[CRIPT] KSA BEGIN\n");
    for(int i =0; i< 256; i++){
        s[i]=i;
    }

    int j = 0;
    for(int i = 0 ; i< 256; i++){
        j = (j + s[i] + key_c[i % size_key]) % 256;
        changee(i, j);
    }

    sb_fprintf(stdout, "[CRIPT] KSA END\n");
}

void simple_bus_encryption::changee(int i, int j){
    int aux = s[i];
    s[i] = s[j];
    s[j] = aux;
}

void simple_bus_encryption::PRGA() {
    sb_fprintf(stdout, "[CRIPT] PRGA BEGIN\n");
    int *valuesI = new int();
    //int *valuesJ = new int();
    int *result = new int();
    //unsigned int posicao;
    //unsigned int aux;
    int i = address_read_start, j = address_read_start;

    j=0;
    i=0;
    //char r,g,b,a;
    std::vector<int> pixelAtual;
    for (unsigned int aux = address_read_start; aux < address_read_end; aux += 4){
        i = (i+1)%256;
        j = (j+s[i])%256;
        changee(i,j);

        bus_port->direct_read(valuesI, aux);
        pixelAtual = unpack(*valuesI);
        //sb_fprintf(stdout, "[CRIPT] V: %d \n", *valuesI);


        for(unsigned int b = 0 ; b < pixelAtual.size(); b++){
                //sb_fprintf(stdout, "[CRIPT] V: %d \n", pixelAtual.at(b));

                *result = (s[(s[i] + s[j]) % 256])^(pixelAtual.at(b));
                pixelAtual.at(b) = *result;
                //sb_fprintf(stdout, "[CRIPT] RESULT: %d \n", *result);
        }

        *result = pack(pixelAtual);
        //sb_fprintf(stdout, "[CRIPT] PACK: %d \n", *result);

        bus_port->direct_write(result, aux);
    }





    /*std::vector<int> pixelAtual;
    for (aux = address_read_start; aux < address_read_end; aux = aux + 4) {
        i = ((i + 1) % 256) * 4;
        bus_port_intern->direct_read(valuesI, i);
        j = ((j + *valuesI) % 256) * 4;
        change(i, j);
        bus_port_intern->direct_read(valuesI, i);
        bus_port_intern->direct_read(valuesJ, j);
        posicao = ((*valuesI + *valuesJ) % 256) * 4;
        bus_port->direct_read(valuesI, aux);
        pixelAtual = unpack(*valuesI);
        bus_port_intern->direct_read(valuesJ, posicao);

        sb_fprintf(stdout, "[CRIPT] UNPACK: %d \n", posicao);
        for(unsigned int i = 0 ; i < pixelAtual.size(); i++){
                sb_fprintf(stdout, "[CRIPT] V: %d \n", pixelAtual.at(i));
                *result = pixelAtual.at(i) ^ *valuesJ;
                pixelAtual.at(i) = *result;
                sb_fprintf(stdout, "[CRIPT] RESULT: %d \n", *result);
        }

        *result = pack(pixelAtual);
        sb_fprintf(stdout, "[CRIPT] PACK: %d \n", *result);
        *result = *valuesI ^ *valuesJ;
        bus_port->direct_write(result, aux);
    }
    delete valuesI;
    delete valuesJ;
    delete result; */
    sb_fprintf(stdout, "[CRIPT] PRGA END\n");
}

void simple_bus_encryption::change(unsigned int i, unsigned int j) {
    int *aux = new int;
    int si;
    bus_port_intern->direct_read(aux, i);
    si = *aux;
    bus_port_intern->direct_read(aux, j);
    bus_port_intern->direct_write(aux, i);
    *aux = si;
    bus_port_intern->direct_write(aux, j);
    delete aux;
}

void simple_bus_encryption::seeMemory() {
    int *values = new int;
    sb_fprintf(stdout, "[CRIPT] START SEE MEMORY\n");
    for (unsigned int i = address_read_start; i < address_read_end; i = i + 4) {
        bus_port->direct_read(values, i);
        sb_fprintf(stdout, "M-> %s MEMORY: %d  VALUE:%d\n", sc_time_stamp().to_string().c_str(), i, *values);
        wait(0, SC_NS);
    }
    delete values;
    sb_fprintf(stdout, "[CRIPT] FINISH SEE MEMORY\n");
}
