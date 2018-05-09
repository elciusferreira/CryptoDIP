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
 
  simple_bus_master_direct.h : The monitor (master) using the direct BUS
                               interface.
 
  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/

#ifndef __simple_bus_encryption_h
#define __simple_bus_encryption_h

#include <systemc.h>

#include "simple_bus_direct_if.h"


SC_MODULE(simple_bus_encryption)
{
  // ports
  sc_in_clk clock;
  sc_port<simple_bus_direct_if> bus_port;
  sc_port<simple_bus_direct_if> bus_port_intern;

  SC_HAS_PROCESS(simple_bus_encryption);

  // constructor
  simple_bus_encryption(sc_module_name name_
                           , unsigned int address_start
                           , unsigned int address_end
                           , unsigned int address_reserved
                           , unsigned int address_graphs
                           , int timeout
                           , int timeout_internal
                           , bool verbose = true)
    : sc_module(name_)
    , m_address_start(address_start)
    , m_address_end(address_end)
    , m_address_reserved(address_reserved)
    , m_address_graphs(address_graphs)
    , m_timeout(timeout)
    , m_timeout_internal(timeout_internal)
    , m_verbose(verbose)
  {
    // process declaration
    size_key = 5;
    
    for(int i =0 ; i < 5; i++)
      key_c[i] = i*50;
    SC_THREAD(main_action);
  }

  // process
  void main_action();
  
private:
  unsigned int m_address_start, m_address_end, m_address_reserved, m_address_graphs;
  unsigned int address_read_start, address_read_end;
  int m_timeout, m_timeout_internal;
  bool m_verbose;
  unsigned int size_key;
  int key_c[5];
  
  // private process
  void KSA();
  void PRGA();
  void change(unsigned int i, unsigned int j);
  void getRange();
  void seeMemory();


}; // end class simple_bus_master_direct

#endif
