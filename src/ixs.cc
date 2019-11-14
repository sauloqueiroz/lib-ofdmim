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


#include "ixs.h"

void UnRankingAlgorithmsCallBack::combinadicUnranking(TypeData X, int N, int k, TypeIndex* answer) //unranking: from current p1 data to k-index
{
  assert(answer);
  TypeData bc;
  TypeIndex cc = N; // cc is the next candidate for arrayI[i]
  int i;
  for (i=k-1; i>-1; i--)
  {
    do
    {
      cc--; //the first candidate for  arrayI[k] is N-1
      bc = binomialCoefficient(cc, i+1);
    }
    while (bc > X);
    X = X - bc;
    answer[i] = cc;    
  }
}

//unranking: from current p1 data to k-index
void UnRankingAlgorithmsCallBack::optimalUnranking(TypeData X, int N, int k, TypeIndex* indexesArray) 
{
  assert(indexesArray);
  TypeIndex cc = N - 1;
  TypeData bc = (double) binomialCoefficient(cc, k);
  double  bc2; 
  int i;
  for (i=k-1; i>-1; i--)
  {
   while (bc > X)
    {
      bc2 = (double)(cc - (i + 1))*((double)bc/cc); //1st: from C(N, k) to C(N-1, k)
      bc = (TypeData) bc2;
      cc--; //update cc to correspond to current bc
    }
    indexesArray[i] = cc;
    X = X - bc;
    bc2 = (double)(i + 1)*((double)bc/cc); //1st C(cc, k) to C(cc - 1, k - 1)
    bc = (TypeData)bc2;
    cc--;    
  }
}


TypeData UnRankingAlgorithmsCallBack::combinadicRanking(int N, int k, TypeIndex* indexesArray)
{
  assert(indexesArray);
  TypeData X = 0;
  TypeIndex cc = N; // cc is the next candidate for arrayI[i]
  int i;
  for (i=0; i<k; i++)
      X = X + binomialCoefficient(indexesArray[i], i+1); 
  return X;
}

TypeData UnRankingAlgorithmsCallBack::optimalRanking(int N, int k, TypeIndex* indexesArray)
{
  assert(indexesArray);
  TypeIndex cc, i = 1;
  double X = 0.0, ccbincoef = 0.0;

  while (i<=k && indexesArray[i-1]<i)
    i++;
  if (i>k) return 0;

  //indexesArray is indexed from 0 to k-1.
  ccbincoef = (double) binomialCoefficient(indexesArray[i-1], i);
  cc = indexesArray[i-1];
  for (cc = indexesArray[i-1]; cc < N; cc++)
  {
    if ( i > k) return X;
    if (indexesArray[i-1] == cc)
    {
      X = X + (unsigned long int)ccbincoef;
      ccbincoef = ccbincoef * ((double)cc+1) / ((double)i+1);
      i++;   
    }
   else
     ccbincoef = ccbincoef * ((double)cc+1) / ((double)cc+1-i);
  }
  return (TypeData) X;
}


TypeData UnRankingAlgorithmsCallBack::binomialCoefficient(unsigned int n, unsigned int k)
{  
  int i;
  if (k > n) return 0;
  if (k == n) return 1;
  double prod = 1.0;
  for(i=1; i<=k; i++)
    prod= prod * ((double)n-i+1)/i;
  return (unsigned long int)prod;
}

//replace the content of this method to your own implementation of unranking (index selector at transmitter)
TypeData UnRankingAlgorithmsCallBack::thirdPartRanking(int N, int k, TypeIndex* indexesArray)
{
   UnRankingAlgorithmsCallBack obj;
   return obj.optimalRanking(N, k, indexesArray);
}

//replace the content of this method to your own implementation of ranking (index selector at receiver)
void UnRankingAlgorithmsCallBack::thirdPartUnranking(TypeData X, int N, int k, TypeIndex* answer)
{
   UnRankingAlgorithmsCallBack obj;
   obj.optimalUnranking(X, N, k, answer);
}
