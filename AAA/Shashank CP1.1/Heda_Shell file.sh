#!/bin/bash
#PBS -q default
#PBS -N output
#PBS -l nodes=1:ppn=4
#PBS -l walltime=00:00:10
#PBS -o  /home/shashankheda/Gn_Programs/output
#PBS -e  /home/shashankheda/Gn_Programs/error
#PBS -V
cd /home/shashankheda/HDa
mpiexec -np 4 ./abc

