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


#include "immapper.h"
#include <stdio.h>
#include <random>
#include <iostream>
#include <string>

using namespace std;
//#include <complex.h>

int main(int argc, char *argv[])
{
  cout << "This program generates all OFDM-IM waveforms for given N and k (g=1)" << endl;
  int N, k, M, ixsAlgorithm;
  if (argc < 4)
  { 
     cout <<"Format: ";
     cout <<"./ofdmim N k M IxSAlgoritm"<<endl;
     cout <<"\t N = number of subcarriers "<<endl;
     cout <<"\t k = number of active subcarriers "<<endl;
     cout <<"\t M = constellation size for modulation of active subcarriers: 2 = BPSK, 4 = QPSK"<<endl;
     cout <<"\t IxSAlgorithm = Index selector algorithm: 1 = combinadic, 2 = adapted combinadic "<<endl;
     return -1;
  }
  N = stoi(argv[1]);
  k = stoi(argv[2]);
  M = stoi(argv[3]);
  ixsAlgorithm = stoi(argv[4]);
  IMMapper myMapper(N, M);
  myMapper.setk(k);
  void (UnRankingAlgorithmsCallBack::*unrank)(TypeData, int, int, TypeIndex*);
  TypeData (UnRankingAlgorithmsCallBack::*rank) (int N, int k, TypeIndex* indexesArray);
  if (ixsAlgorithm == 1)
  {
    unrank = &UnRankingAlgorithmsCallBack::combinadicUnranking;
    rank = &UnRankingAlgorithmsCallBack::combinadicRanking;
  }
  {
    unrank = &UnRankingAlgorithmsCallBack::optimalUnranking;
    rank = &UnRankingAlgorithmsCallBack::optimalRanking;
  }

  myMapper.setIxSAlgorithm(unrank, rank);
  int i;
  TypeData dataInterval = myMapper.getNumberOfIMWaveforms();
  for (i=0; i <  dataInterval; i++)   
  {
      TypeData indexData = rand() % dataInterval;
      myMapper.loadP1(indexData); //just set p1 to i
      myMapper.loadP2(); //load data array p2 with random values     
      myMapper.map(); //ixs + mlut + symbol creation

      cout << endl<< "Chosen Indexes for P1 data="<< i << endl;
      myMapper.printArrayI(); //print indexes modulated by p1
      cout << endl<< "k-data of p2 array" << endl;
      myMapper.printP2();
      cout << endl<< "Resulting symbol" << endl;
      myMapper.printSymbol(); //create OFDM IM symbol
      cout << endl;

      cout << "Demapping" << endl;
      myMapper.map();
      cout << "Value of P1="<< myMapper.getP1() << endl;
      cout << "Values of P2 are:\n";
      myMapper.printP2();
      cout << endl;
   }
  cout << "Total generated symbols " << dataInterval << endl;
 return 0;
}
