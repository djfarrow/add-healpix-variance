/*
 *
 *  Add the variance measured from a HEALpix map
 *  to a FITs catalogue. Borrows from fitscopy.c
 *
 *
 *  USAGE: add_snr_to_catalogue in_cat out_cat in_hp racname deccname outcname
 *  
 *  incat -> Input fits catalogue. Must be a fits-basic catalogue, not the 
 *           default fits-plus that libraries such as Starlink outputs
 *  
 *  outcat -> Output fits file
 *
 *  in_hp -> Input Healpix fits file
 *
 *  racname, deccname -> Name of RA/Dec columns in input file
 *
 *  outcname -> Name for output column
 *
 *
 *  AUTHOR: Daniel Farrow
 *
 *
 */

#include<stdio.h>
#include<string.h>
#include "fitsio.h"
#include "chealpix.h"

const double PI = 3.14159265359; 
const double DEG2RAD = 3.14159265359/180.0; 

int main(int argc, char* argv[])
{
   /*Inputs*/
   char infile[200], outfile[200], inhp[200];	
   char racname[60], deccname[60], outcname[60];

   /* HEALpix-required parameters */
   long nsides;
   float npix;
   float* input_hp;
   char coord[10], ordering[10];    

   /* Variables for reading/writing the catalogues*/
   int status=0, ncols, colnum, i;
   fitsfile *fin, *fout;
   long nrows=1;

   /* Arrays for actually adding the HEALpix map thingy*/
   float *ra, *dec;
   float *cvals;

   /*Read in input*/
   if(argc < 7){
      printf("USAGE: add_snr_to_catalogue in_cat out_cat in_hp racname deccname outcname\n");
      return 1;
   }
   else
   {
      sscanf(argv[1], "%s", infile);
      sscanf(argv[2], "%s", outfile);
      sscanf(argv[3], "%s", inhp);
      sscanf(argv[4], "%s", racname);
      sscanf(argv[5], "%s", deccname);
      sscanf(argv[6], "%s", outcname);
   }

  /* Read in Healpix mask */
  input_hp = read_healpix_map(inhp, &nsides, coord, ordering);
  printf("Read nsides = %ld for input: %s and output: %s \n", nsides, inhp, outfile);

  /* Open the catalogue and read number of rows */
  fits_open_file(&fin, infile, READONLY, &status);
  fits_movabs_hdu(fin, 2, NULL, &status);
  fits_get_num_rows(fin, &nrows, &status);

  if(status) {
    fits_report_error(stderr, status);
    return status;
  }
 
  /* Get the output file ready */ 
  if(!fits_create_file(&fout, outfile, &status))
  {
     /* copy the previous, current, and following HDUs */
     fits_copy_file(fin, fout, 1, 1, 1, &status);
  } 
  else
  {
     fits_report_error(stderr, status);
     return status;
  }
   
  printf("Input file has %ld rows\n", nrows);
 
  /* Array to store the variances/limiting mags*/
  cvals = calloc(nrows, sizeof(float)); 
  ra = calloc(nrows, sizeof(float));
  dec = calloc(nrows, sizeof(float));  

  /* Read in ra/dec */
  fits_get_colnum(fin, 0, racname, &colnum, &status);
  fits_read_col(fin, TFLOAT, colnum, 1, 1, nrows, NULL, ra, 
                NULL, &status);

  fits_get_colnum(fin, 0, deccname, &colnum, &status);
  fits_read_col(fin, TFLOAT, colnum, 1, 1, nrows, NULL, dec, 
                NULL, &status);

  fits_get_num_cols(fout, &ncols, &status);
  printf("Adding new column at position %d\n", ncols + 1);
  fits_insert_col(fout, ncols + 1, outcname, "E", &status);

  /* Look up HEALpix values */
  for(int i = 0; i<nrows; i++){
      long index = -99;
      float theta = PI/2.0 - dec[i]*DEG2RAD; //between 0 and pi
      float phi = ra[i]*DEG2RAD;

      if(phi < 0.0) phi = 2*PI + phi;

      if( strcmp(ordering, "RING") == 0){
          ang2pix_ring(nsides, theta, phi, &index); 
      } 
      else if( strcmp(ordering, "NESTED") == 0){   	 
          ang2pix_nest(nsides, theta, phi, &index); 
      }
      else 
      {
          printf("ERROR: Cannot recognize HEALPIX ordering scheme %s\n", ordering);
	  return 1;
      }
  
      cvals[i] = input_hp[index];
  }

  // XXX todo replace NULL with nulval
  fits_write_colnull(fout, TFLOAT, ncols + 1, 1, 1, nrows, 
		     cvals, NULL, &status);  

  if(status) {
    fits_report_error(stderr, status);
    return status;
  }
 
  fits_close_file(fin, &status);
  fits_close_file(fout, &status);
  free(ra);
  free(dec);
  free(cvals);

  printf("All done!\n");

  return status;

}
