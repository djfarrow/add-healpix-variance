# add-healpix-variance

## Introduction 
Add the variance in the [PS1 survey](https://panstarrs.stsci.edu/) to a FITS catalogue, by reading it from a 
[HEALpix](https://healpix.jpl.nasa.gov/) file. The input must be a 
basic [FITs catalogue](https://heasarc.gsfc.nasa.gov/fitsio/), you may encounter issues if using other formats like fits-plus. Although this is designed for PS1, it can be adapted to work with other HEALpix maps and catalogues.

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
