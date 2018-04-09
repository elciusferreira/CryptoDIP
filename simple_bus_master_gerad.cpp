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

#include "simple_bus_master_gerad.h"
#include "simple_bus_types.h"

void simple_bus_master_gerad::main_action()
{
  sb_fprintf(stdout, "GENERATOR START!\n");
  int *newValue= new int;
  unsigned int newAddress;
  /*unsigned int i = m_address_start;


  std::string line;
  std::ifstream fs;
  fs.open("ouput.txt");


  std::string array[5];
  std::string aux = "";
  int counter = 0;
  if (fs.is_open()){ 
    while (getline (fs,line)){
        for(unsigned int i = 0; i< line.size(); i++){
            if(line.at(i) == ","){
                array[counter++] = aux;
                aux = "";
            }
            aux += line.at(i);
        }

        std::cout<< array[0];
        


        //std::cout << line << '\n';
        bus_port->direct_write(newValue,  newAddress);
    }
    fs.close();
  }
  else
    std::cout<< "error open file!";
    */
  
  // SAVE MEMORY
  for(unsigned int i = m_address_start; i < m_address_end; i = i+4){
    *newValue = i+1;
    newAddress = i;

    bus_port->direct_write(newValue,  newAddress);

    sb_fprintf(stdout, "WRITE GENERATOR-> TIME: %s READ FROM: %d VALUE: %d\n", 
      sc_time_stamp().to_string().c_str(),
      newAddress, 
      *newValue);

    wait(m_timeout, SC_NS);
  }
  
}


