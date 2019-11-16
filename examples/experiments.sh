#!/bin/bash
# ./experiments

# this script assumes cpu logic cores 2 and 3 (i.e., physical core 2) are 
# isolated with the linux kernel directive isolcpu
# also, the source code ieeeaccess.cc contains a flag to run with highest priority

echo "ATTENTION: This scripts works only for BPSK and even numbers N<=68"

#=====================================> 1.  Measuring runtime
#----->  run ieeeaccess varying N and IxS algorithm (M=BPSK, k=N/2, g=1)
#-- algorithm 1 = combinadic
#-- algorithm 2 = optimal (linear-time)
#-----> This loop requires providencia1par, an Akaroa-2 based program
#-- akaroa-2 is an old project. I had to compile it with g++-4.6
#-- ieeeaccess produces 20000 runtime samples. I verified this is enough
#   to enable the convergence of akaroa-2 with 95% conf. and rel. error < 5%

LARGESTN=68
for ALGORITHM in {1..2..1} 
do
#    for i in {2..$LARGESTN..2} 
    for ((i=2; i<=$LARGESTN; i=i+2 ))
     do
       while :
       do 
        sudo taskset -c 2 sudo ./ieeeaccess $i 2 $ALGORITHM > "amostras-$ALGORITHM-$i"
        wait 
        ./providencia1par "amostras-$ALGORITHM-$i" > "resultado-$ALGORITHM-$i"
            #checa se arquivo previamente criado ta vazio
            #poe 1 em $? ta vazio, 0 otherwise
            [ -s "resultado-$ALGORITHM-$i" ] 
            if [ $? -eq  0 ];
            then            
                sudo cat "resultado-$ALGORITHM-$i"; #joga akaroa's output na tela
                sudo killall ieeeaccess;
                #sudo rm timesamples.txt
                break;
                #exit 0;
            fi
       done
     done
done
sudo killall ieeeaccess

#=====================================> 2.  
#----->  run ieeeaccess varying N and IxS algorithm (M=BPSK, k=N/2, g=1)
#-- 
#-- 

# computing m requires calculation of binomial coefficients, which is hard shell.
# Thus I compute them offline for different Ns and load them in an array here.
# WE ASSUME LARGESTN = 68 AND N is even
m=([1]=2 [2]=4 [3]=7 [4]=10 [5]=12 [6]=15 [7]=18 [8]=21 [9]=24 [10]=27 [11]=30 [12]=33 [13]=36 [14]=39 [15]=42 [16]=45 [17]=48 [18]=51 [19]=54 [20]=57 [21]=59 [22]=62 [23]=65 [24]=68 [25]=71 [26]=74 [27]=77 [28]=80 [29]=83 [30]=86 [31]=89 [32]=92 [33]=95[34]=98)


rm runtime-adapted-vs-combinadic-data.txt
rm scthroughput-adapted-vs-combinadic-data.txt
rm latex-table.txt
rm se-gain.txt

# generate latex table and file for gnuplot
for ((i=2; i<=$LARGESTN; i=i+2 ))
do
    #awk 'NR==2 {print $3}' FILE.txt means: output the content of column 3 of line 2 of file FILE.txt
    tempocomb=`awk 'NR==2 {print $2}' "resultado-1-$i"`
    deltacomb=`awk 'NR==2 {print $3}' "resultado-1-$i"`
    varianciacomb=`awk 'NR==2 {print $5}' "resultado-1-$i"`
    ncomb=`awk 'NR==2 {print $6}' "resultado-1-$i"`
    transientecomb=`awk 'NR==2 {print $7}' "resultado-1-$i"`

    tempoadapted=`awk 'NR==2 {print $2}' "resultado-2-$i"`
    deltaadapted=`awk 'NR==2 {print $3}' "resultado-2-$i"`
    varianciaadapted=`awk 'NR==2 {print $5}' "resultado-2-$i"`
    nadapted=`awk 'NR==2 {print $6}' "resultado-2-$i"`
    transienteadapted=`awk 'NR==2 {print $7}' "resultado-2-$i"`
    # file to plot runtimes with gnuplot i.e. NON rounded values
    echo "$i $tempoadapted $deltaadapted $tempocomb $deltacomb" >> runtime-adapted-vs-combinadic-data.txt

   # so reportamos ate n=LARGESTN 
   if [ $i -le $LARGESTN ]
   then   
    j=$((i / 2)); #indice do array de bits
    bits=`echo ${m[$j]}` #bits conseguidos para essa configuracao IM
    ## calculating SC throughput to gnuplot i.e. NON rounded values
    #tempocombsegs=`bc <<< "scale = 6; $tempocomb/1000000"` 
    #tempoadaptedsegs=`bc <<< "scale = 6; $tempoadapted/1000000"` 

    #scadapted=`bc <<< "scale = 3; $bits / $tempoadaptedsegs"`; # Spectro COmputational throughput
    #sccomb=`bc <<< "scale = 3; $bits / $tempocombsegs"`; # Spectro COmputational throughput
    ## file to plot spectro computational throughput with gnuplot.
    #echo "$i $scadapted $deltaadapted $sccomb $deltacomb" >> scthroughput-adapted-vs-combinadic-data.txt

    # rounding digits of time and sc values for paper
    tempoadapted=`echo $tempoadapted | xargs printf "%.*f\n" 2` # rounding to 2 digits
    deltaadapted=`echo $deltaadapted | xargs printf "%.*f\n" 3` # rounding to 3 digits
    tempocomb=`echo $tempocomb | xargs printf "%.*f\n" 2` # rounding to 2 digits
    deltacomb=`echo $deltacomb | xargs printf "%.*f\n" 3` # rounding to 3 digits
    #scadapted=`echo $scadapted | xargs printf "%.*f\n" 2` # rounding to 2 digits
    #sccomb=`echo $sccomb | xargs printf "%.*f\n" 2` # rounding to 2 digits


    ganho=`bc <<< "scale = 3; $bits / $i"`; # ganho da Efic. Espec. sobre OFDM
    ganho=`echo $ganho | xargs printf "%.*f\n" 2` # rounding to 2 digits
    echo $i #$ganho
    echo $i $ganho >> se-gain.txt
    echo "\multirow{2}{*}{$i} & \multirow{2}{*}{$bits} & \multirow{2}{*}{$ganho} &LIxS &\textbf{$tempoadapted} & $deltaadapted & $nadapted & $transienteadapted \\\ \cline{4-8}" >> latex-table.txt
    echo "& & & IxS & \textbf{$tempocomb} & $deltacomb & $ncomb & $transientecomb \\\ \hline\hline" >> latex-table.txt
   fi
done
sed -i 's/,/./g' latex-table.txt #replace , by . in file latex-table.txt
sed -i 's/,/./g' se-gain.txt

for j in {1..2..1} 
do
    rm -rf "IxS-$j"
    mkdir "IxS-$j"
    mkdir "IxS-$j/amostras"
    mkdir "IxS-$j/resultados"
    sudo mv amostras-* "IxS-$j/amostras"
    sudo mv resultado-* "IxS-$j/resultados"
done
gnuplot plot-runtime &> /dev/null # suppresses impression
#gnuplot plot-scthroughput &> /dev/null # suppresses impression
evince runtime.eps &
#evince throughput.eps &
