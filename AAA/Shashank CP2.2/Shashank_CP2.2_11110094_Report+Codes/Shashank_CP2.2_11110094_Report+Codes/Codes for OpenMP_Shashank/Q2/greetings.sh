#!/bin/bash
#PBS -q default

#PBS -N output
//#PBS -l nodes=1:ppn=16
#PBS -l walltime=00:00:10
#PBS -o  /home/shashankheda/openmp/q2/output
#PBS -e  /home/shashankheda/openmp/q2/error
#PBS -V
cd /home/shashankheda/openmp/q2
./abc