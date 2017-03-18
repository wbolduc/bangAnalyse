# Makefile for Pilot tutorial C and Fortran labs

# Change to your local Pilot installation, or invoke as:
#	make PILOTHOME=... target
#

#PILOTHOME = /usr/local
PILOTHOME = /work/wgardner/pilot
MPEHOME = /work/wgardner/mpe

CC = mpicc
CPPFLAGS += -I$(PILOTHOME)/include -I$(MPEHOME)/include
CFLAGS = -g -O0
LDFLAGS += -L$(PILOTHOME)/lib -lpilot -L$(MPEHOME)/lib -lmpe

FC = mpif90
FFLAGS += -I$(PILOTHOME)/include
# LDFLAGS same as above


# "make labN" will compile labN.c using implicit make rules
bang: bang.o
	$(CC) $< $(LDFLAGS) -O0 -o $@

optimalBang: bang.o
	$(CC) $< $(LDFLAGS) -O3 -o $@


# "make flabN" will compile flabN.F90 and link flabN
flab%.o: flab%.F90
	$(FC) -c $< $(FFLAGS) -o $@

flab%: flab%.o
	$(FC) $< $(LDFLAGS) -o $@

test: bang
	rm log
	sqsub -q mpi -r 2 -n 4 -o log bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 6 -picheck=3 -pisvc=d
	watch sqjobs

bigTest: bang
	sqsub -q mpi -r 2 -n 1 -o Logs/log1 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 2 -o Logs/log2 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 3 -o Logs/log3 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 4 -o Logs/log4 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 5 -o Logs/log5 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 6 -o Logs/log6 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 7 -o Logs/log7 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 8 -o Logs/log8 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 9 -o Logs/log9 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 10 -o Logs/log10 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 11 -o Logs/log11 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 12 -o Logs/log12 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 13 -o Logs/log13 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 14 -o Logs/log14 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 15 -o Logs/log15 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 16 -o Logs/log16 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 17 -o Logs/log17 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 18 -o Logs/log18 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 19 -o Logs/log19 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 20 -o Logs/log20 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 21 -o Logs/log21 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 22 -o Logs/log22 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 23 -o Logs/log23 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 24 -o Logs/log24 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5

oppTest: bang
	sqsub -q mpi -r 2 -n 1 -o Logs/olog1 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 2 -o Logs/olog2 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 3 -o Logs/olog3 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 4 -o Logs/olog4 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 5 -o Logs/olog5 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 6 -o Logs/olog6 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 7 -o Logs/olog7 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 8 -o Logs/olog8 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 9 -o Logs/olog9 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 10 -o Logs/olog10 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 11 -o Logs/olog11 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 12 -o Logs/olog12 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 13 -o Logs/olog13 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 14 -o Logs/olog14 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 15 -o Logs/olog15 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 16 -o Logs/olog16 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 17 -o Logs/olog17 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 18 -o Logs/olog18 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 19 -o Logs/olog19 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 20 -o Logs/olog20 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 21 -o Logs/olog21 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 22 -o Logs/olog22 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 23 -o Logs/olog23 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5
	sqsub -q mpi -r 2 -n 24 -o Logs/olog24 bang /work/wgardner/a1/NCDB_1999_to_2012.csv 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5 1 2 3 4 5

format:
	grep "[$$\#].*" Logs/log1 > Logs/flog1
	grep "[$$\#].*" Logs/log2 > Logs/flog2
	grep "[$$\#].*" Logs/log3 > Logs/flog3
	grep "[$$\#].*" Logs/log4 > Logs/flog4
	grep "[$$\#].*" Logs/log5 > Logs/flog5
	grep "[$$\#].*" Logs/log6 > Logs/flog6
	grep "[$$\#].*" Logs/log7 > Logs/flog7
	grep "[$$\#].*" Logs/log8 > Logs/flog8
	grep "[$$\#].*" Logs/log9 > Logs/flog9
	grep "[$$\#].*" Logs/log10 > Logs/flog10
	grep "[$$\#].*" Logs/log11 > Logs/flog11
	grep "[$$\#].*" Logs/log12 > Logs/flog12
	grep "[$$\#].*" Logs/log13 > Logs/flog13
	grep "[$$\#].*" Logs/log14 > Logs/flog14
	grep "[$$\#].*" Logs/log15 > Logs/flog15
	grep "[$$\#].*" Logs/log16 > Logs/flog16
	grep "[$$\#].*" Logs/log17 > Logs/flog17
	grep "[$$\#].*" Logs/log18 > Logs/flog18
	grep "[$$\#].*" Logs/log19 > Logs/flog19
	grep "[$$\#].*" Logs/log20 > Logs/flog20
	grep "[$$\#].*" Logs/log21 > Logs/flog21
	grep "[$$\#].*" Logs/log22 > Logs/flog22
	grep "[$$\#].*" Logs/log23 > Logs/flog23
	grep "[$$\#].*" Logs/log24 > Logs/flog24


nukeLogs:
	rm Logs/*

cleanLogs:
	rm Logs/log*

clean:
	rm bang.o
	rm bang
