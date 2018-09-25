C = gcc
CCFLAGS = -I/home/dfarrow/Healpix_3.40/include -I/home/dfarrow/cfitsio/include
LDFLAGS = -L/home/dfarrow/Healpix_3.40/lib -L/home/dfarrow/cfitsio/lib
LIBS =   -lchealpix -lcfitsio -lm -lgsl -lgslcblas


add_healpix_variance: add_healpix_variance.o
		$(CC) $(LDFLAGS) -o add_healpix_variance $< $(LIBS) 

clean:
		rm *.o add_healpix_variance

%.o: %.c
		$(CC) $(CCFLAGS) -g -c $< 


