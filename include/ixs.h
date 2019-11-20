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

#ifndef __IXS
#define __IXS

#include <iostream>
#include <assert.h> 
#include <stdlib.h> 
#include <stdio.h> 
using namespace std;

    #ifndef __TYPES
    #define __TYPES
        typedef unsigned long int TypeData;
        typedef unsigned int TypeIndex;
    #endif

#define LARGEST_N 64

/*
 * Class that implements different combinatorial unranking/ranking
 * algorithms. Users of method IMMapper::map/IMMapper:demap must 
 * provide it with one of these algorithms.
 */

class UnRankingAlgorithmsCallBack
{
  public:
    //the following are intended to general use
    void combinadicUnranking(TypeData X, int N, int k, TypeIndex* answer); 
    void optimalUnranking(TypeData X, int N, int k, TypeIndex* answer);//optimal = optimal spectro-computational OFDM-IM
    void pascalUnranking(TypeData X, int N, int k, TypeIndex* answer);//not implemented yet
    void thirdPartUnranking(TypeData X, int N, int k, TypeIndex* answer);
    TypeData combinadicRanking(int N, int k, TypeIndex* indexesArray); 
    TypeData optimalRanking(int N, int k, TypeIndex* indexesArray); 
    TypeData thirdPartRanking(int N, int k, TypeIndex* indexesArray); 
    TypeData pascalRanking(int N, int k, TypeIndex* indexesArray);  //not implemented yet
    TypeData binomialCoefficient(TypeData N, TypeData K);
};
#endif
