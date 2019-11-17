
SCRIPTS in receiver/transmitter directory
ASSUMPTIONS:
 -> bpsk is assumed (array m depends on that)

*experiments.sh: runs ieeeaccess-rx,tx programs from N=2..66 varying IxS algorithms (1=combinadic, 2=adapted). For each one of these setups, several runtime samples of the algorithms are output. THe number of outputs is determined by akaroa-2 (i manually verified that 20k samples suffice to reach default precision of Akaroa-2). The samples are in directory IxS-(1,2)/amostras and the result of akaroa is in IxS-(1,2)/resultados

*plot-runtime : gnuplot script for runtime-adapted-vs-combinadic-data.txt
*plot-sc : gnuplot script for scthroughput-adapted-vs-combinadic-data.txt
*providencia1par: (old fashion c++ coded) Akaroa-2 program to process data
*latex-table.txt: produces content of the paper's table (needs extra manual handling)
*se-gain: lists the SE gain over OFDM
