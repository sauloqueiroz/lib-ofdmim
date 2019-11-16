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

#include "mlut.h"
//=============================================================> MLUT CLASS
/*
 * Default MLUT constructor: BPSK
 */
MLUT::MLUT(): M(2), table(NULL)
{
  name  = "BPSK";
  setM(2);
}

/*
 * Only BPSK and QPSK are supported.
 */
MLUT::MLUT(int _M): table(NULL)
{
  M = _M;
  //checking whether M is not power of two
  if ((M <= 0) || !((M & (M - 1)) == 0)) 
  {
    std::cout <<"M>0 must be power of two. Setting to default M=2"<< endl;
    M = 2;
    name = "BPSK";
  }
  int i;
  setM(_M);
}

int MLUT::getM()
{
  return M;
}

void MLUT::setM(int _M)
{

  //if table is NULL, allocate for the first time
  //otherwise, reallocate resize it because user wanna changes modulation
  table = (complex<double> *) realloc(table, M * sizeof(complex<double>));
  assert(table);
  M = _M;
  //IEEE 802.11 Constellation points
  switch ( M )
  {
    case BPSK: //BPSK
        bpsk: //label for goto. Remove this when all M-QAM are supported.
        table[0] = complex<double>(-1.0, 0.0);
        table[1] = complex<double>(+1.0, 0.0);
    break;
    case QPSK: //QPSK
        table[0] =  complex<double>(-1.0, -1.0);
        table[1] = complex<double>(-1.0, 1.0); 
        table[2] = complex<double>(1.0, -1.0);
        table[3] = complex<double>(1.0, 1.0); 
        name = "QPSK";
    break;
    default:
        std::cout <<"Only BPSK or QPSK are supported! Setting BPSK."<< endl;
         M = 2;
         goto bpsk; //Remove this when all M-QAM are supported.
    break;
  }
}

MLUT::~MLUT()
{
}

/*
 * data is an integer from 0 to M-1.
 * returns the complex constellation point.
 */
complex<double> MLUT::map(TypeData data)
{
  assert((data >= 0) && (data < M));
  return (complex<double>) table[data];
}

/*
 * Returns (decimal) data corresponding to constellationPoint
 */
TypeData MLUT::demap(complex<double> constellationPoint)
{
  assert(table);
  TypeData i;
  for (i=0; i<M; i++)
    if (table[i] == constellationPoint)
      return i;
  cout <<"Constellation point"<< constellationPoint <<" not found." << endl;
  assert(i < M);
}

void MLUT::showConstellation()
{
  int i;
  assert(table);
  cout << name << endl;
  for (i=0; i<M; i++)
    cout << real(table[i]) <<"+"<< imag(table[i]) <<"i" << endl;
}

