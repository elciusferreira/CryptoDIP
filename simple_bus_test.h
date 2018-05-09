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
 
  simple_bus_test.h : The test bench.
 
  Original Author: Ric Hilderink, Synopsys, Inc., 2001-10-11
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
 
 *****************************************************************************/

#ifndef __simple_bus_test_h
#define __simple_bus_test_h

#include <systemc.h>

#include "simple_bus.h"
#include "simple_bus_master_blocking.h"
#include "simple_bus_master_non_blocking.h"
#include "simple_bus_master_gerad.h"
#include "simple_bus_encryption.h"
#include "simple_bus_gpu.h"
#include "simple_bus_arbiter.h"
#include "simple_bus_master_com.h"
#include "simple_bus_slow_mem.h"
#include "simple_bus_fast_mem.h"

SC_MODULE(simple_bus_test)
{
  // channels
  sc_clock C1;

  // module instances
  simple_bus_encryption          *master_encryption;
  simple_bus_master_com          *master_communication;
  simple_bus_master_gerad        *master_generator;
  simple_bus_gpu                 *master_gpu;
  simple_bus                     *bus;
  simple_bus                     *bus_intern;
  simple_bus                     *bus_communication;
  simple_bus_fast_mem            *mem_fast;
  simple_bus_fast_mem            *mem_fast_intern;
  simple_bus_arbiter             *arbiter;
  simple_bus_arbiter             *arbiter_intern;
  simple_bus_arbiter             *arbiter_communication;

  // constructor
  SC_CTOR(simple_bus_test)
    : C1("C1")
  {
    // create instances
    master_generator = new simple_bus_master_gerad("master_generator", 0, 127, 10);
    master_communication = new simple_bus_master_com("master_communication", 0, 16383, 8, 12, 16, 0, 127, 50);
    master_encryption = new simple_bus_encryption("master_encryption", 0, 4, 8, 12, 50, 10);
    master_gpu = new simple_bus_gpu("master_gpu", 0, 4240, 12, 4096, 50);
    mem_fast = new simple_bus_fast_mem("mem_fast", 0, 16383);
    mem_fast_intern = new simple_bus_fast_mem("mem_fast_temp", 0, 1043);
    bus = new simple_bus("bus", true); // verbose output
    bus_intern = new simple_bus("bus_intern", true);
    bus_communication = new simple_bus("bus_communication", true);
    arbiter = new simple_bus_arbiter("arbiter", true); // verbose output
    arbiter_intern = new simple_bus_arbiter("arbiter_intern", true);
    arbiter_communication = new simple_bus_arbiter("arbiter_communication", true);

    // connect instances
    master_encryption->clock(C1);
    master_communication->clock(C1);
    master_generator->clock(C1);
    master_gpu->clock(C1);
    bus->clock(C1);
    bus_intern->clock(C1);
    bus_communication->clock(C1);
    master_encryption->bus_port(*bus);
    master_encryption->bus_port_intern(*bus_intern);
    master_communication->bus_port(*bus);
    master_generator->bus_port(*bus_communication);
    master_gpu->bus_port(*bus);
    bus->arbiter_port(*arbiter);
    bus->slave_port(*mem_fast);
    bus_intern->arbiter_port(*arbiter);
    bus_intern->slave_port(*mem_fast_intern);
    bus_communication->arbiter_port(*arbiter_communication);
    bus_communication->slave_port(*master_communication);
  }

  // destructor
  ~simple_bus_test()
  {
    if (master_encryption) {delete master_encryption; master_encryption = 0;}
    if (master_communication){delete master_communication; master_communication = 0;}
    if (master_generator){delete master_generator; master_generator = 0;}
    if (master_gpu){delete master_gpu; master_gpu = 0;}
    if (bus) {delete bus; bus = 0;}
    if (bus_intern) {delete bus_intern; bus_intern = 0;}
    if (mem_fast) {delete mem_fast; mem_fast = 0;}
    if (mem_fast_intern) {delete mem_fast_intern; mem_fast_intern = 0;}
    if (arbiter) {delete arbiter; arbiter = 0;}
    if (arbiter_intern) {delete arbiter_intern; arbiter_intern = 0;}
  }

}; // end class simple_bus_test

#endif
