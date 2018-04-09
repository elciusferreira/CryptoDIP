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

#ifndef __simple_bus_master_gerad_h
#define __simple_bus_master_gerad_h

#include <systemc.h>

#include "simple_bus_direct_if.h"
#include <iostream>
#include <fstream>



SC_MODULE(simple_bus_master_gerad)
{
  // ports
  sc_in_clk clock;
  sc_port<simple_bus_direct_if> bus_port;

  SC_HAS_PROCESS(simple_bus_master_gerad);

  // constructor
  simple_bus_master_gerad(sc_module_name name_
                           , unsigned int address_start
                           , unsigned int address_end
                           , std::string _path
                           , int timeout
                           , bool verbose = true)
    : sc_module(name_)
    , m_address_start(address_start)
    , m_address_end(address_end)
    , m_timeout(timeout)
    , m_verbose(verbose)
  {
    path = _path;
    // process declaration
    SC_THREAD(main_action);
  }

  // process
  void main_action();

private:
  unsigned int m_address_start, m_address_end;
  int m_timeout;
  bool m_verbose;
  std::string path;

}; // end class simple_bus_master_gerad

#endif
