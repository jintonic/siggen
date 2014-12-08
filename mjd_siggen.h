/*
 *  mjd_siggen.h -- define data structures used by rewritten fieldgen and siggen for MJD detectors
 *                    (BEGes and PPC detectors)
 *  David Radford   Oct 2014
 */

#ifndef _MJD_SIGGEN_H
#define _MJD_SIGGEN_H

#include "cyl_point.h"

/* verbosity levels for std output */
#define TERSE  0
#define NORMAL 1
#define CHATTY 2

// #define VERBOSE 2  // Set to 0 for quiet, 1 or 2 for less or more info
#define TELL_NORMAL if (setup->verbosity >= NORMAL) tell
#define TELL_CHATTY if (setup->verbosity >= CHATTY) tell

/* Reference temperature for drift vel. corrections is 77K */
#define REF_TEMP 77
/* max, min temperatures for allowed range */
#define MIN_TEMP 77
#define MAX_TEMP 110
/* enum to identify cylindrical or cartesian coords */
#define CYL 0
#define CART 1

float sqrtf(float x);
float fminf(float x, float y);

// from fields.c
struct velocity_lookup{
  float e;
  float e100;
  float e110;
  float e111;
  float h100;
  float h110;
  float h111;
  float ea; //coefficients for anisotropic drift 
  float eb;
  float ec;
  float ebp;
  float ecp;
  float ha;
  float hb;
  float hc;
  float hbp;
  float hcp;
  float hcorr;
  float ecorr;
};

/* setup parameters data structure */
typedef struct {
  // general
  int verbosity;              // 0 = terse, 1 = normal, 2 = chatty/verbose

  // geometry
  float xtal_length;          // z length
  float xtal_radius;          // radius
  float top_bullet_radius;    // bulletization radius at top of crystal
  float bottom_bullet_radius; // bulletization radius at bottom of BEGe crystal
  float pc_length;            // point contact length
  float pc_radius;            // point contact radius
  float taper_length;         // size of 45-degree taper at bottom of ORTEC-type crystal
  float wrap_around_radius;   // wrap-around radius for BEGes. Set to zero for ORTEC
  float ditch_depth;          // depth of ditch next to wrap-around for BEGes. Set to zero for ORTEC
  float ditch_thickness;      // width of ditch next to wrap-around for BEGes. Set to zero for ORTEC
  float Li_thickness;         // depth of full-charge-collection boundary for Li contact

  // electric fields & weighing potentials
  float xtal_grid;            // grid size in mm for field files (either 0.5 or 0.1 mm)
  float impurity_z0;          // net impurity concentration at Z=0, in 1e10 e/cm3
  float impurity_gradient;    // net impurity gardient, in 1e10 e/cm4
  float xtal_HV;              // detector bias for fieldgen, in Volts
  int   max_iterations;       // maximum number of iterations to use in mjd_fieldgen
  int   write_field;          // set to 1 to write V and E to output file, 0 otherwise
  int   write_WP;             // set to 1 to calculate WP and write it to output file, 0 otherwise

  // file names
  char drift_name[256];       // drift velocity lookup table
  char field_name[256];       // potential/efield file name
  char wp_name[256];          // weighting potential file name

  // signal calculation 
  float xtal_temp;            // crystal temperature in Kelvin
  float preamp_tau;           // integration time constant for preamplifier, in ns
  int   time_steps_calc;      // number of time steps used in calculations
  float step_time_calc;       // length of time step used for calculation, in ns
  float step_time_out;        // length of time step for output signal, in ns
  //    nonzero values in the next few lines significantly slow down the code
  float charge_cloud_size;    // initial FWHM of charge cloud, in mm; set to zero for point charges
  float cloud_size_slope;     // additional size of charge cloud per 1MeV energy deposited
  int   use_diffusion;        // set to 0/1 for ignore/add diffusion as the charges drift

  int   coord_type;           // set to CART or CYL for input point coordinate system
  int   ntsteps_out;          //number of time steps in output signal

  // data for fields.c
  float rmin, rmax, rstep;
  float zmin, zmax, zstep;
  int   v_lookup_len;
  struct velocity_lookup *v_lookup;
  cyl_pt **efld;
  float  **wpot;
  
  // data for calc_signal.c
  point *dpath_e, *dpath_h;
  float initial_vel, final_vel;
  float final_charge_size_sq;

} MJD_Siggen_Setup;


int read_config(char *config_file_name, MJD_Siggen_Setup *setup);

#endif /*#ifndef _MJD_SIGGEN_H */
