/*
 *  Convert a HEALpix variance map into a 
 *  n-sigma depth map, in magnitudes
 *  
 *  AUTHOR(S): Daniel Farrow (MPE)
 *  
 *  USAGE
 *  -----
 *  
 *  convert_to_sigma in_file out_file aper_rad nsigma [psf_map]
 *  
 *  in_file: Input HEALpix file
 *  
 *  out_file: Output HEALpix file
 *  
 *  aper_rad: Radius of detection aperture in arcseconds (note no 
 *            correction for flux outside aperture included!)
 *  
 *  nsigma: Number of sigmas that the magnitude in the map should
 *          correspond to
 *  
 *  [psf_map] : Optionally pass a map of the PSF (note this
 *              is not yet implemented)
 * 
 */

#define PI 3.14159265359; 
#define COVAR_FACTOR 1.379

#include<stdio.h>
#include<string.h>
#include<math.h>
#include "fitsio.h"
#include "chealpix.h"

int main(int argc, char* argv[])
{
   /*Inputs*/
   char infile[200], outfile[200], fnpsfmap[200];	
   float aperrad, nsigma, aperrad2;

   /* HEALpix-required parameters */
   long nsides, nsides_psf;
   long npix;
   float* input_hp = NULL;
   float* psf_map = NULL;
   char coord[10], ordering[10];    
   char coord_psf[10], ordering_psf[10];    
   float *vals = NULL;

   long i;

   /*Read in input*/
   if(argc < 5){
      printf("USAGE: convert_to_sigma in_file out_file aper_rad nsigma [psf_map]\n");
      return 1;
   }
   else
   {
      sscanf(argv[1], "%s", infile);
      sscanf(argv[2], "%s", outfile);
      sscanf(argv[3], "%f", &aperrad);
      sscanf(argv[4], "%f", &nsigma);
   }
 
   /* Optionally read in PSF size */
   if(argc == 6){
       sscanf(argv[5], "%s", fnpsfmap);
       psf_map = read_healpix_map(fnpsfmap, &nsides_psf, coord_psf, ordering_psf);
       printf("Also read PSF map from %s\n", fnpsfmap);
   }


  /* Read in Healpix mask */
  input_hp = read_healpix_map(infile, &nsides, coord, ordering);
  printf("Read nsides = %ld for input: %s and output: %s \n", nsides, infile, outfile);
  npix = nside2npix(nsides);


  /* Allocate and fill output array */
  vals = calloc(npix, sizeof(float)); 

  aperrad2 = aperrad*aperrad;
  for(i = 0; i < npix; i++){

     float area = 0.0;
     float total_variance; 

     if(psf_map != NULL)
     {
         /* Set area based off of the PSF */
         printf("Error: Not implemented!\n");
         return 1;     
     }
     else
     {
         /* Area is just area of fixed aperture */
         area = aperrad2*PI;
     }

     /* Total variance as number of pixels*variance in map */
     total_variance = area*input_hp[i]*COVAR_FACTOR;

     /* Convert target SNR to a magnitude */
     vals[i] = -2.5*log10(nsigma*sqrt(total_variance)) + 25.0;
  }

/* Output HEALpix map */
write_healpix_map(vals, nsides, outfile, 1, coord);

/* Free the allocated memory */
if(psf_map) free(psf_map);
free(input_hp);
free(vals);

return 0;

}


























