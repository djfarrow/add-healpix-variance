# add_healpix_variance

## Introduction 
Add the variance in the [PS1 survey](https://panstarrs.stsci.edu/) to a FITS catalogue, by reading it from a 
[HEALpix](https://healpix.jpl.nasa.gov/) file. The input must be a 
basic [FITs catalogue](https://heasarc.gsfc.nasa.gov/fitsio/), you may encounter issues if using other formats like fits-plus. Although this is designed for PS1, it can be adapted to work with other HEALpix maps and catalogues. It also can
be used to add other HEALpix-stored values, such as limiting magnitudes to a catalogue.

## Building the code 
Before use you will need to change the `CCFLAGS` and `LDFLAGS` in the Makefile to point to your version of 
HEALpix and cfitsio.

## Usage
`USAGE: add_snr_to_catalogue in_cat out_cat in_hp racname deccname outcname`


| Input        | Description                 |
| -------------|:---------------------------:|
| in_cat       | Input catalogue             |
| out_cat      | Output catalogue            |
| in_hp        | Input HEALpix file          |
| racname      | name of RA column in input  |
| deccname     | name of DEC column in input |
| outcname     | name to give appended column|

A input catalogue for testing, as well as the expected output catalogue, is included in the `/tests/data` directory.

# convert_to_nsigma

## Introduction

Convert a PS1 HEALpix variance map into a map of the magnitude in a given aperture that corresponds to an Nsigma detection.

`USAGE: convert_to_sigma in_file out_file aper_rad nsigma [psf_map]`


  | Input    | Description                                          |
  | ---------|:----------------------------------------------------:|
  | in_file  | Input HEALpix file                                   | 
  | out_file | Output HEALpix file                                  |
  | aper_rad | Radius of detection aperture in arcseconds (note no correction for flux outside aperture included!)      |
  |  nsigma  | Number of sigmas that the magnitude in the map should correspond to             |
  | psf_map |  Optionally pass a map of the PSF (note this is not yet implemented)              |








