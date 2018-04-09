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

#include "simple_bus_master_direct.h"
#include "simple_bus_types.h"
#include <stdlib.h>
#include <time.h>

void simple_bus_master_direct::main_action() {
    srand(time(NULL)); // initialize random seed
    while(true) {
        int* pixel = generate_rgba(); // generator
        check_crc(pixel[0], pixel[1], pixel[2], pixel[3], pixel[4]);
        set_rgba(pixel[0], pixel[1], pixel[2], pixel[3]);
        int red, green, blue, alpha, crc;

        red = get_red();
        green = get_green();
        blue = get_blue();
        alpha = get_alpha();
        crc = get_crc();

        sb_fprintf(stdout, "Red: %i, Green: %i, Blue: %i, Alpha: %i and CRC: %i\n",
                   red, green, blue, alpha, crc);

        /* writing to memory*/
        bus_port->direct_write(&red,   m_address);
        bus_port->direct_write(&green, m_address+4);
        bus_port->direct_write(&blue,  m_address+8);
        bus_port->direct_write(&alpha, m_address+12);

        wait(m_timeout, SC_NS);

        /* reading from memory*/
        int mydata[4];
        bus_port->direct_read(&mydata[0], m_address);
        bus_port->direct_read(&mydata[1], m_address+4);
        bus_port->direct_read(&mydata[2], m_address+8);
        bus_port->direct_read(&mydata[3], m_address+12);

        /* checking dada wrote*/
        if (m_verbose)
        	sb_fprintf(stdout, "%s %s : mem[%i:%i] = (%i, %i, %i, %i)\n",
            sc_time_stamp().to_string().c_str(), name(), m_address,
            m_address+15,
            mydata[0], mydata[1], mydata[2], mydata[3]);

        wait(m_timeout, SC_NS);
    }
}

int* simple_bus_master_direct::generate_rgba() {

    int red = rand() % 255; // 0 -> 255
    int green = rand() % 255;
    int blue = rand() % 255;
    int alpha = rand() % 255;
    int crc = red + green + blue + alpha;

    static int p[4];
    p[0] = red;
    p[1] = green;
    p[2] = blue;
    p[3] = alpha;
    p[4] = crc;

    return p;
}

bool simple_bus_master_direct::check_and_set_crc(int red, int green, int blue, int alpha, int crc_) {
  if (crc == (red + green + blue + alpha)) {
      this->crc = crc;
      return true;
  } else {
      return false;
  }
}
void simple_bus_master_direct::set_rgba(int red, int green, int blue, int alpha){
  this->r = red;
  this->g = green;
  this->b = blue;
  this->a = alpha;
}
int simple_bus_master_direct::get_red() {return r;}
int simple_bus_master_direct::get_green() {return g;}
int simple_bus_master_direct::get_blue() {return b;}
int simple_bus_master_direct::get_alpha() {return a;}
int simple_bus_master_direct::get_crc() {return crc;}


/*
void simple_bus_master_direct::main_action()
{
  int mydata[4];
  while (true)
    {
      bus_port->direct_read(&mydata[0], m_address);
      bus_port->direct_read(&mydata[1], m_address+4);
      bus_port->direct_read(&mydata[2], m_address+8);
      bus_port->direct_read(&mydata[3], m_address+12);

      if (m_verbose)
      	sb_fprintf(stdout, "%s %s : mem[%x:%x] = (%x, %x, %x, %x)\n",
		      sc_time_stamp().to_string().c_str(), name(), m_address,
		      m_address+15,
		      mydata[0], mydata[1], mydata[2], mydata[3]);

      wait(m_timeout, SC_NS);
    }
}
*/
/*
void simple_bus_master_direct::main_action()
{
  int mydata[4] = {1,1,1,1};

  while (true) {
      bus_port->direct_write(&mydata[0], m_address);
      bus_port->direct_write(&mydata[1], m_address+4);
      bus_port->direct_write(&mydata[2], m_address+8);
      bus_port->direct_write(&mydata[3], m_address+12);

      bus_port->direct_read(&mydata[0], m_address);
      bus_port->direct_read(&mydata[1], m_address+4);
      bus_port->direct_read(&mydata[2], m_address+8);
      bus_port->direct_read(&mydata[3], m_address+12);

      mydata[0] = mydata[0] + 1;

      if (m_verbose)
      	sb_fprintf(stdout, "%s %s : mem[%x:%x] = (%x, %x, %x, %x)\n",
		      sc_time_stamp().to_string().c_str(), name(), m_address,
		      m_address+15,
		      mydata[0], mydata[1], mydata[2], mydata[3]);

      wait(m_timeout, SC_NS);
  }

  // communication_module initial test
  set_rgba(30, 40, 50, 60);
  calculate_crc(30, 40, 50, 60);
  //crc = get_crc();
  //sb_fprintf(stdout, "CRC: %i\n", crc);
  int red, green, blue, alpha, crc;
  red = get_red();
  green = get_green();
  blue = get_blue();
  alpha = get_alpha();
  crc = get_crc();

  sb_fprintf(stdout, "Red: %i, Green: %i, Blue: %i, Alpha: %i and CRC: %i\n",
             red, green, blue, alpha, crc);

  wait(m_timeout, SC_NS);
}
*/
