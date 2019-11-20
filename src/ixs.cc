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


#include <ixs.h>

void UnRankingAlgorithmsCallBack::combinadicUnranking(TypeData X, int N, int k, TypeIndex* answer) //unranking: from current p1 data to k-index
{
  assert(answer);
  TypeData bc;
  TypeIndex cc = N; // cc is the next candidate for arrayI[i]
  long int i;
  for (i=k-1; i>=0; i--)
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
  TypeData cc = N - 1;
  TypeData bc = binomialCoefficient(cc, k);
  long int i;
  for (i=k-1; i>=0; i--)
  {
   while (bc > X)
    {
      if (cc == 0) return;
      bc =  ((cc - (i + 1))*bc)/cc; //1st: from C(N, k) to C(N-1, k)
      cc--; //update cc to correspond to current bc
    }
    indexesArray[i] = cc;
    X = X - bc;
    if (cc == 0) return;
    bc = ((i+1)*bc)/cc; //1st C(cc, k) to C(cc - 1, k - 1)
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
  TypeData X = 0, ccbincoef = 0;

  while (i<=k && indexesArray[i-1]<i)
    i++;
  if (i>k) return 0;
  ccbincoef = (TypeData) binomialCoefficient(indexesArray[i-1], i);
  cc = indexesArray[i-1];
  for (cc = indexesArray[i-1]; cc < N; cc++)
  {
    if ( i > k) return X;
    if (indexesArray[i-1] == cc)
    {
      X = X + ccbincoef;
      ccbincoef = (ccbincoef *(cc+1)) / (i+1);
      i++;   
    }
   else
     ccbincoef = (ccbincoef * (cc+1)) / (cc+1-i); 
  }
  return (TypeData) X;
}

/*
 * This is the best algorithm I know to compute binomial coefficients
 * It is from the ancient book Lilavati, see below link for details
 * REF: https://blog.plover.com/math/choose.html
 * TODO: employ BigInt class to support arbitrarily large symbols
 */
TypeData UnRankingAlgorithmsCallBack::binomialCoefficient(TypeData N, TypeData K)
{  
  //the product in this multiplicative formula of C(N,k) produces very large numbers
  //the largest one that fits a 64-bit variable is C(66,33)
  assert(N <= LARGEST_N && N >= 0 && K >= 0);

  if (N < K)
    return 0;  // special case
  if (N == K || K == 0)
    return 1;

  
  long double r = 1.0, d, n = (long double) N, k = (long double) K;

   for (d=1; d <= k; d++) 
   {
     r *= n--;
     r /= d;
   }
    return (TypeData) r;
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
