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

#include "simple_bus_master_com.h"
#include "simple_bus_types.h"

void simple_bus_master_com::main_action()
{

  int *mydata = new int;
  for(unsigned int i = m_start_address_intern_memory; i< m_end_address_intern_memory; i= i+4){
      read(mydata, i);
      bus_port->direct_write(mydata, i);


      sb_fprintf(stdout, "READ COMUNICATION-> TIME: %s READ FROM: %d VALUE: %d\n", 
        sc_time_stamp().to_string().c_str(),
        i, 
        *mydata);


      wait(m_timeout, SC_NS);
  }
  // Can Work
  *mydata = 1;
  bus_port->direct_write(mydata, m_address_reserved);
  sb_fprintf(stdout, "WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n", 
      sc_time_stamp().to_string().c_str(),
      m_address_reserved, 
      *mydata);

  *mydata = m_address_start;
  bus_port->direct_write(mydata, 0);

  sb_fprintf(stdout, "WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n", 
      sc_time_stamp().to_string().c_str(),
      m_address_reserved, 
      *mydata);

  *mydata = m_address_end;
  bus_port->direct_write(mydata, 4);

  sb_fprintf(stdout, "WRITE MEM FULL-> TIME: %s READ FROM: %d VALUE: %d\n", 
      sc_time_stamp().to_string().c_str(),
      m_address_reserved, 
      *mydata);

}

inline bool simple_bus_master_com::direct_read(int *data, unsigned int address)
{
  return (read(data, address) == SIMPLE_BUS_OK);
}

inline bool simple_bus_master_com::direct_write(int *data, unsigned int address)
{
  return (write(data, address) == SIMPLE_BUS_OK);
}

inline simple_bus_status simple_bus_master_com::read(int *data
               , unsigned int address)
{
  *data = MEM[(address - m_start_address_intern_memory)/4];
  return SIMPLE_BUS_OK;
}

inline simple_bus_status simple_bus_master_com::write(int *data
                , unsigned int address)
{
  MEM[(address - m_start_address_intern_memory)/4] = *data;
  return SIMPLE_BUS_OK;
}

inline  simple_bus_master_com::~simple_bus_master_com()
{
  if (MEM) delete [] MEM;
  MEM = (int *)0;
}

inline unsigned int simple_bus_master_com::start_address() const
{
  return m_start_address_intern_memory;
}

inline unsigned int simple_bus_master_com::end_address() const
{
  return m_end_address_intern_memory;
}



