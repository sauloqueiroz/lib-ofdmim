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
#include <iomanip>      // std::setprecision
#include "immapper.h"
#include <stdio.h>
#include <random>
#include <iostream>
#include <fstream>
#include <sys/time.h> 
using namespace std;

/*
 * THIS PROGRAM REPORTS THE OFDMIM MAPPER RUNTIME IN MICROSECONDS
*/

//to set highest priority
#include <sys/time.h>
#include <sys/resource.h>

/* libboost for high resolution clock.
 * compilation demands directives -lboost_system -lboost_timer
 * We use it along with real-time priority and CPU core dedication (isolcpu and taskset)
 */
#include <boost/timer/timer.hpp>
using namespace boost::timer;

/*  Explanation from https://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html
     int clock_gettime( clockid_t clock_id, struct  
     timespec *tp ); The clock_gettime() function gets 
     the current time of the clock specified by clock_id,  
     and puts it into the buffer  pointed to by tp.tp  
     parameter points to a structure containing  
     atleast the following members:     
     struct timespec { 
                // This represents the number of whole seconds of elapsed time.
               time_t   tv_sec;        
               // This is the rest of the elapsed time (a fraction of a second), 
               // represented as the number of nanoseconds. It is always less than one billion.
               long     tv_nsec;       
           }; 
    clock id = CLOCK_REALTIME, CLOCK_PROCESS_CPUTIME_ID,  
               CLOCK_MONOTONIC ...etc 
    CLOCK_REALTIME : clock  that  measures real (i.e., wall-clock) time. 
    CLOCK_PROCESS_CPUTIME_ID : High-resolution per-process timer  
                               from the CPU. 
    CLOCK_MONOTONIC : High resolution timer that is unaffected 
                      by system date changes (e.g. NTP daemons). 
*/


int main(int argc, char *argv[])
{
  //assigns highest priority to process. Requires root privileges.
  setpriority(PRIO_PROCESS, 0, -20);
 
  int i, j, ik; 
  int N, k, M, ixsAlgorithm;
  if (argc < 2)
  { 
     cout <<"Format: ";
     cout <<"./ofdmim N IxSAlgoritm"<<endl;
     cout <<"\t N = number of subcarriers (k=N/2)"<<endl;
     cout <<"\t M = constellation size for modulation of active subcarriers: 2 = BPSK, 4 = QPSK"<<endl;
     cout <<"\t IxSAlgorithm = Index selector algorithm: 1 = combinadic, 2 = adapted combinadic "<<endl;
     return -1;
  }
  N = stoi(argv[1]);
  k = N/2;
  M = 2;
  ixsAlgorithm = stoi(argv[2]);
  IMMapper myMappers[3];// 3 random streams -> 3 mappers
  for (i=0; i<3; i++)
  {     
      myMappers[i].setN(N);
      myMappers[i].setk(k);
      myMappers[i].setM(M);
      void (UnRankingAlgorithmsCallBack::*unrank)(TypeData, int, int, TypeIndex*) = 0;
      TypeData (UnRankingAlgorithmsCallBack::*rank) (int N, int k, TypeIndex* indexesArray);
      if (ixsAlgorithm == 1)
      {
        unrank = &UnRankingAlgorithmsCallBack::combinadicUnranking;
        rank = &UnRankingAlgorithmsCallBack::combinadicRanking;
      }
      else
      {
        unrank = &UnRankingAlgorithmsCallBack::optimalUnranking;
        rank = &UnRankingAlgorithmsCallBack::optimalRanking;
      }

      myMappers[i].setIxSAlgorithm(unrank, rank);
  }

  mt19937_64 randomStream[3];
  // good seeds
  randomStream[0].seed(1973272912);
  randomStream[1].seed(1822174485);
  randomStream[2].seed(1998078925);

  TypeData dataInterval = myMappers[0].getNumberOfIMWaveforms(); //same for the 3
  std::uniform_int_distribution<TypeData> distribution(0, dataInterval - 1);
 
  cpu_timer temporizador;//contador. Tempo ja está contando desde construtor. Usar start.
  TypeData _dataForActiveSubcarriers[k];


  struct timespec start, end; 
  // unsync the I/O of C and C++. 
  ios_base::sync_with_stdio(false); 

  //20k suffices for our tests to reach default confidence of Akaroa-2 
  for (i=0; i<20000; i++)  
  {
    //we feed akaroa-2 with samples simulated from 3 independent PRNGs
    for (j=0; j<3; j++)
    {
       //generates 64-bit random number from stream j (out of 3), from 0 to dataInterval - 1
       TypeData indexData = distribution(randomStream[j]); 
       myMappers[j].loadP1(indexData); //just set p1 to i
       for (ik=0; ik<k; ik++)
         _dataForActiveSubcarriers[ik] = distribution(randomStream[j]) % M;
       myMappers[j].loadP2(_dataForActiveSubcarriers); //load p2 data array with our random values

       //temporizador.start(); 
       clock_gettime(CLOCK_MONOTONIC, &start); 

       //myMappers[j].demapP1(); //ixs + mlut + symbol creation      
       myMappers[j].demap(); //ixs + mlut + symbol creation      
       clock_gettime(CLOCK_MONOTONIC, &end); 
       //clock_gettime(CLOCK_REALTIME, &end); 
       //clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end); 
  
       // Calculating total time taken by the program. 
       double time_taken = 0.0; 
       time_taken = (end.tv_sec - start.tv_sec) * 1e6;//elapsed time from secs to microsecs
       time_taken = time_taken  + (end.tv_nsec - start.tv_nsec) * 1e-3; //residual time from nano to microsec
       cout << fixed  << time_taken << setprecision(9) << endl; 
    }
   }
 return 0;
}
