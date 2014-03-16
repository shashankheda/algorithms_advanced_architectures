#!/bin/bash
#PBS -q default
#PBS -N output
#PBS -l nodes=1:ppn=4
#PBS -l walltime=00:01:10
#PBS -o  /home/shashankheda/Previous_Progs/output
#PBS -e  /home/shashankheda/Previous_Progs/error
//#PBS -m shashank.heda8@gmail.com
//#PBS -M eab
#PBS -V
cd /home/shashankheda/Previous_Progs
mpiexec -np 4 ./abc < input
