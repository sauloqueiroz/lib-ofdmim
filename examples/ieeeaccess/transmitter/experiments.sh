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

LARGESTN=62
for ALGORITHM in {1..2..1} 
do
#    for i in {2..$LARGESTN..2} 
    for ((i=2; i<=$LARGESTN; i=i+2 ))
     do
       while :
       do 
        sudo taskset -c 2 sudo ./ieeeaccess-tx $i $ALGORITHM > "amostras-$ALGORITHM-$i"
        wait 
        ./providencia1par "amostras-$ALGORITHM-$i" > "resultado-$ALGORITHM-$i"
            #checa se arquivo previamente criado ta vazio
            #poe 1 em $? ta vazio, 0 otherwise
            [ -s "resultado-$ALGORITHM-$i" ] 
            if [ $? -eq  0 ];
            then            
                sudo cat "resultado-$ALGORITHM-$i"; #joga akaroa's output na tela
                #sudo killall ieeeaccess;
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
m=([1]=2 [2]=4 [3]=7 [4]=10 [5]=12 [6]=15 [7]=18 [8]=21 [9]=24 [10]=27 [11]=30 [12]=33 [13]=36 [14]=39 [15]=42 [16]=45 [17]=48 [18]=51 [19]=54 [20]=57 [21]=59 [22]=62 [23]=65 [24]=68 [25]=71 [26]=74 [27]=77 [28]=80 [29]=83 [30]=86 [31]=89 [32]=92 [33]=95 [34]=98)


sudo rm runtime-adapted-vs-combinadic-data.txt
sudo rm scthroughput-adapted-vs-combinadic-data.txt
sudo rm latex-table.txt
sudo rm se-gain.txt
sudo rm sc.eps
sudo rm runtime.eps
prec=0.0000000001


# generate latex table and file for gnuplot
echo "0 0 0 0 0" >> scthroughput-adapted-vs-combinadic-data.txt
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
    j=$((i / 2)); #indice do array de bits
    bits=`echo ${m[$j]}` #bits conseguidos para essa configuracao IM
    echo "$i $tempoadapted $deltaadapted $tempocomb $deltacomb $bits" >> runtime-adapted-vs-combinadic-data.txt

   # so reportamos ate n=LARGESTN 
   if [ $i -le $LARGESTN ];
   then   
    #j=$((i / 2)); #indice do array de bits
    #bits=`echo ${m[$j]}` #bits conseguidos para essa configuracao IM
    ## calculating SC throughput to gnuplot i.e. NON rounded values
    
    tempocombsegs=`bc <<< "scale = 10; $tempocomb/1000000"` 
    tempoadaptedsegs=`bc <<< "scale = 10; $tempoadapted/1000000"` 
    if (( $(echo "$tempocombsegs < $prec" |bc -l) ))
    then
       tempocombsegs=$prec
    fi
    if (( $(echo "$tempoadaptedsegs< $prec" |bc -l) ))
    then
       tempoadaptedsegs=$prec
    fi
    echo "Tempo em segundos $tempocombsegs $tempoadaptedsegs";
    echo "Bits=$bits"
    sccomb=`bc <<< "scale = 10; $bits/$tempocombsegs"`; # Spectro COmputational throughput in bits per seconds
    sccomb=`bc <<< "scale = 10; $sccomb/1000000"`; # bps to mbps
    scadapted=`bc <<< "scale = 10; $bits/$tempoadaptedsegs"`; # Spectro COmputational throughput
    scadapted=`bc <<< "scale = 10; $scadapted/1000000"`; # bps to mbps
    echo "SC Mbps = $scomb $scadapted"
    # file to plot spectro computational throughput with gnuplot.
    echo "$i $scadapted $deltaadapted $sccomb $deltacomb" >> scthroughput-adapted-vs-combinadic-data.txt

    # rounding digits of time and sc values for paper
    tempoadapted=`echo $tempoadapted | xargs printf "%.*f\n" 2` # rounding to 2 digits
    deltaadapted=`echo $deltaadapted | xargs printf "%.*f\n" 3` # rounding to 3 digits
    tempocomb=`echo $tempocomb | xargs printf "%.*f\n" 2` # rounding to 2 digits
    deltacomb=`echo $deltacomb | xargs printf "%.*f\n" 3` # rounding to 3 digits
    scadapted=`echo $scadapted | xargs printf "%.*f\n" 2` # rounding to 2 digits
    sccomb=`echo $sccomb | xargs printf "%.*f\n" 2` # rounding to 2 digits


    ganho=`bc <<< "scale = 3; $bits / $i"`; # ganho da Efic. Espec. sobre OFDM BPSK
    ganho=`echo $ganho | xargs printf "%.*f\n" 2` # rounding to 2 digits
    echo $i #$ganho
    echo $i $ganho >> se-gain.txt
    echo "\multirow{2}{*}{$i} & \multirow{2}{*}{$bits} & \multirow{2}{*}{$ganho} &Adapted &\textbf{$tempoadapted} & $deltaadapted & \textbf{$scadapted}  & $nadapted \\\ \cline{4-8}" >> latex-table.txt
    echo "& & & Original & \textbf{$tempocomb} & $deltacomb & \textbf{$sccomb} & $ncomb \\\ \hline\hline" >> latex-table.txt
   # below lines prints transient information in latex table instead of sc throughput
   # echo "\multirow{2}{*}{$i} & \multirow{2}{*}{$bits} & \multirow{2}{*}{$ganho} &Adapted &\textbf{$tempoadapted} & $deltaadapted & $nadapted & $transienteadapted \\\ \cline{4-8}" >> latex-table.txt
    #echo "& & & Original & \textbf{$tempocomb} & $deltacomb & $ncomb & $transientecomb \\\ \hline\hline" >> latex-table.txt
   fi
done
sed -i 's/,/./g' latex-table.txt #replace , by . in file latex-table.txt
sed -i 's/,/./g' se-gain.txt


    sudo rm -rf "IxS-1"
    mkdir "IxS-1"
    mkdir "IxS-1/amostras"
    mkdir "IxS-1/resultados"
    sudo mv amostras-1* "IxS-1/amostras"
    sudo mv resultado-1* "IxS-1/resultados"

    sudo rm -rf "IxS-2"
    mkdir "IxS-2"
    mkdir "IxS-2/amostras"
    mkdir "IxS-2/resultados"
    sudo mv amostras-2* "IxS-2/amostras"
    sudo mv resultado-2* "IxS-2/resultados"

gnuplot plot-runtime &> /dev/null # suppresses impression
gnuplot plot-sc &> /dev/null # suppresses impression
evince runtime.eps &
evince sc.eps &

# finishing latex table
