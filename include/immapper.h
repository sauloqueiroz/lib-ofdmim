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

#ifndef __IMMAPPER
#define __IMMAPPER

#include "mlut.h" 
#include "ixs.h" 
#include <complex>
#include <iostream>
#include <assert.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 
using namespace std;

    #ifndef __TYPES
    #define __TYPES
        typedef unsigned long int TypeData;
        typedef unsigned int TypeIndex;
    #endif
/* OFDMIM mapper
 * The constructor creates array I of indexes and s of
 */
class IMMapper
{
  private:
    int N; //symbol size
    int g; //number of subblocks
    int k; //number of active subcarriers
    TypeData numberOfIMWaveforms;
    //callbacks: pointer to any method of class UnRankingAlgorithmsCallBack
    UnRankingAlgorithmsCallBack *indexSelector;
    void (UnRankingAlgorithmsCallBack::*unrank)(TypeData X, int N, int k, TypeIndex* answer);
    TypeData (UnRankingAlgorithmsCallBack::*rank) (int N, int k, TypeIndex* indexesArray);

    TypeData p1; //data to modulate indexes 
    TypeIndex *arrayI; //k-size array of indexes
    TypeData *p2; //k-size array of data to modulate the k active subcarriers
    complex<double> *arrayS; //k-size array of M-ary complex baseband samples from p2.
    MLUT *mlut; //M-ary LUT constellation for active subcarriers
    complex<double> * ofdmIMSymbol;
  public:   
     IMMapper(int _N);//default: g=1, k=N/2, M=2, unranking=linear-time unranking
     //default: g=1, k=N/2, M=2, unranking=linear-time unranking
     IMMapper(int _N,  
              void (UnRankingAlgorithmsCallBack::*_unrank)(TypeData, int, int, TypeIndex*), 
              TypeData (UnRankingAlgorithmsCallBack::*_rank)(int, int, TypeIndex*));
     IMMapper(int _N, int _M);//default: g=1, k=N/2
     IMMapper(int _g, int _k, int _N, int _M,
              void (UnRankingAlgorithmsCallBack::*_unrank)(TypeData, int, int, TypeIndex*), 
              TypeData (UnRankingAlgorithmsCallBack::*_rank)(int, int, TypeIndex*)); //Alternative constructor
     ~IMMapper();
     //TypeData binomialCoefficient(unsigned int n, unsigned int k);
     void loadP1(TypeData data); //load p1 with the given data. Do not trigger unranking (IxS)
     TypeData getP1();
     void loadP2(); //load p2 with k randomly generated data, each as wide as log2M bits. Do not tigger ranking
     void loadP2(TypeData *p2DataArray); //load p2 the given input. Do not tigger ranking
     void printP2();
     void map();//triggers mapP1, mapP2 and createSymbol. Needs unranking algorithm
     void mapP1(); //perform IxS (unranking) on current P1. Output goes to arrayI.
     void mapP2(); //from data array p2 to arrayS of complex baseband samples
     void createOFDMIMSymbol();//from p1 and p2 to the N-size array of complex baseband symbol


     void demap(); //from arrayS to p2
     void oracleOFDMIMDetector();
     void demapP1(); //from arrayS to p2
     void demapP2(); //from arrayS to p2

     int getN();
     void setN(int _N);
     void setM(int _M);
     void setIxSAlgorithm(void (UnRankingAlgorithmsCallBack::*_unrank)(TypeData, int, int, TypeIndex*), 
                          TypeData (UnRankingAlgorithmsCallBack::*_rank)(int, int, TypeIndex*));
     TypeData getNumberOfIMWaveforms();
     int getk();
     void setk(int _k);
     void printSymbol();
     void printArrayS();
     void printArrayI();
     TypeData binomialCoefficient(unsigned int n, unsigned int k);
};
#endif
