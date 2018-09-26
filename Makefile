C = gcc
CCFLAGS = -I${HEALPIX}/include -I/home/dfarrow/star-2014A/include/
LDFLAGS = -L${HEALPIX}/lib -L/home/dfarrow/star-2014A/lib
LIBS =   -lchealpix -lcfitsio -lm -lgsl -lgslcblas


add_healpix_variance: add_healpix_variance.o
		$(CC) $(LDFLAGS) -o add_healpix_variance $< $(LIBS) 

clean:
		rm *.o add_healpix_variance

%.o: %.c
		$(CC) $(CCFLAGS) -g -c $< 


