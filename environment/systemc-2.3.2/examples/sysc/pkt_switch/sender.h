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

  sender.h - This is the interface file for the synchronous process "sender".

  Original Author: Rashmi Goswami, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

#ifndef SENDER_H_INCLUDED
#define SENDER_H_INCLUDED

#include "systemc.h"
#include "pkt.h"

struct sender: sc_module {
  sc_out<pkt> pkt_out; 
  sc_in<sc_int<4> > source_id;       
  sc_in_clk CLK;

  SC_CTOR(sender)
    {
      SC_CTHREAD(entry, CLK.pos()); 
     }  
  void entry();
};
#endif // SENDER_H_INCLUDED
