/* detector_geometry_ppc.c -- for "ppc" geometry
 * Karin Lagergren
 *
 * This module keeps track of the detector geometry
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "detector_geometry.h"
#include "point.h"
#include "cyl_point.h"


#define SQ(x) ((x)*(x))
/* outside_detector
   returns 1 if pt is outside the detector, 0 if inside detector
*/
int outside_detector(point pt, MJD_Siggen_Setup *setup){
  float r, z, br;

  z = pt.z;
  if (z >= setup->zmax || z < 0) return 1;

  r = sqrt(SQ(pt.x)+SQ(pt.y));
  if (r > setup->rmax) return 1;
  br = setup->top_bullet_radius;
  if (z > setup->zmax - br &&
      r > (setup->rmax - br) + sqrt(SQ(br)- SQ(z-(setup->zmax - br)))) return 1;
  if (setup->pc_radius > 0 &&
      z <= setup->pc_length && r <= setup->pc_radius) return 1;
  if (setup->taper_length > 0 && z < setup->taper_length &&
      r > setup->zmax - setup->taper_length + z) return 1;

  return 0;
}

int outside_detector_cyl(cyl_pt pt, MJD_Siggen_Setup *setup){
  float r, z, br;

  z = pt.z;
  if (z >= setup->zmax || z < 0) return 1;

  r = pt.r;
  if (r > setup->rmax) return 1;
  br = setup->top_bullet_radius;
  if (z > setup->zmax - br &&
      r > (setup->rmax - br) + sqrt(SQ(br)- SQ(z-(setup->zmax - br)))) return 1;
  if (setup->pc_radius > 0 &&
      z <= setup->pc_length && r <= setup->pc_radius) return 1;
  if (setup->taper_length > 0 && z < setup->taper_length &&
      r > setup->zmax - setup->taper_length + z) return 1;

  return 0;
}
#undef SQ
