/**
 ** Copyright (c) 2019 Saulo Jorge Beltrao de Queiroz
 ** Contact: Saulo Queiroz <ssaulojorge@gmail.com> <sauloqueiroz@utfpr.edu.br> <saulo@dei.uc.pt>
 ** This program is free software; you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation; either version 2 of the License, or
 ** (at your option) any later version.
 ** 
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 ** 
 ** You should have received a copy of the GNU General Public License
 ** along with this program; if not, write to the Free Software 
 ** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 **/

#ifndef __MLUT
#define __MLUT

#include <complex>
#include <iostream>
#include <stdlib.h>
#include <assert.h> 

using namespace std;

#ifndef __TYPES
#define __TYPES
    typedef unsigned long int TypeData;
    typedef unsigned int TypeIndex;
#endif

//name = constellation size
enum CONSTELLATION
{
  BPSK = 2,
  QPSK = 4
};

// Standard M-ary mapper
class MLUT
{
  private:
    int M; //constellation size
    complex<double> *table; //Look-up table for active subcarriers
    string name;
  public:
    MLUT(); //default constructor: BPSK ie M=2
    MLUT(int _M); //constructor with constellation size
    ~MLUT(); //default: BPSK ie M=2
    int getM();
    void setM(int M);
    complex<double> map(TypeData data);//from integer data to complex point
    TypeData demap(complex<double> constellationPoint);//from integer data to complex point
    void showConstellation();
};
#endif
