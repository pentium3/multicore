#

## Files:

pstm.cpp : parallel version of traveling salesman problem

tsm.cpp  :  serial version of traveling salesman problem

## Compile: 

g++-7 -fopenmp ptsm.cpp -o ptsm.o

## Run: 

/ptsm x t filename.txt

Where :

x is the number of cities

t is the number of threads

filename.txt is the file that contains the distance matrix 
