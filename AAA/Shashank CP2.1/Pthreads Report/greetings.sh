#!/bin/bash
#PBS -q default

#PBS -N output
//#PBS -l nodes=1:ppn=4
#PBS -l walltime=00:00:10
#PBS -o  /home/shashankheda/pthreads/output
#PBS -e  /home/shashankheda/pthreads/error
#PBS -V
cd /home/shashankheda/pthreads
./abc 1
./abc 2
./abc 3
./abc 4
./abc 5
./abc 8
