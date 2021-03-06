    #!/bin/bash
    #PBS -q default
    #PBS -o $HOME/AACA/Lab/CP1.2_codes/1a/quadrature.out
    #PBS -e $HOME/AACA/Lab/CP1.2_codes/1a/quadrature.err
    #PBS -N Quadrature
    #PBS -l nodes=node1:ppn=10
    #PBS -l walltime=01:00:00
    #PBS -M ravikumar@iitgn.ac.in
    #PBS -m eab
    #PBS -V

    cd $HOME/AACA/Lab/CP1.2_codes/1a
    rm integral_db
    echo "10 10" > input
    mpiexec -np 2 ./paraboloid<input
    
    echo "100 10" > input
    mpiexec -np 5 ./paraboloid < input
    
    echo "100 100" > input
    mpiexec -np 10 ./paraboloid < input
    
    echo "100 1000" > input
    mpiexec -np 10 ./paraboloid < input
    
    echo "1000 1000" > input
    mpiexec -np 10 ./paraboloid < input

    echo "10000 1000" > input
    mpiexec -np 10 ./paraboloid < input

    echo "10000 10000" > input
    mpiexec -np 10 ./paraboloid < input

    echo "10000 100000" > input
    mpiexec -np 10 ./paraboloid < input

    echo "100000 100000" > input
    mpiexec -np 10 ./paraboloid < input

    echo "1000000 100000" > input
    mpiexec -np 10 ./paraboloid < input

    echo "1000000 1000000" > input
    mpiexec -np 10 ./paraboloid < input

    echo "1000000 10000000" > input
    mpiexec -np 10 ./paraboloid < input

    echo "10000000 10000000" > input
    mpiexec -np 10 ./paraboloid < input

    echo "10000000 100000000" > input
    mpiexec -np 10 ./paraboloid < input

    echo "100000000 100000000" > input
    mpiexec -np 10 ./paraboloid < input
