/*
 *       $Id$
 *
 *       This source code is part of
 *
 *        G   R   O   M   A   C   S
 *
 * GROningen MAchine for Chemical Simulations
 *
 *            VERSION 1.6
 * 
 * Copyright (c) 1991-1997
 * BIOSON Research Institute, Dept. of Biophysical Chemistry
 * University of Groningen, The Netherlands
 * 
 * Please refer to:
 * GROMACS: A message-passing parallel molecular dynamics implementation
 * H.J.C. Berendsen, D. van der Spoel and R. van Drunen
 * Comp. Phys. Comm. 91, 43-56 (1995)
 *
 * Also check out our WWW page:
 * http://rugmd0.chem.rug.nl/~gmx
 * or e-mail to:
 * gromacs@chem.rug.nl
 *
 * And Hey:
 * Great Red Oystrich Makes All Chemists Sane
 */
static char *SRCID_rando_c = "$Id$";

#include "typedefs.h"
	
real rando(int *ig)
     /* generate a random number. */
{
  int  irand;

  int  m    = 100000000;
  real rm   = 100000000.0;  /* same number as m, but real format */
  int  m1   = 10000;
  int  mult = 31415821;
  
  real r;
  int  irandh,irandl,multh,multl;

  irand = abs(*ig) % m;
  
  /* multiply irand by mult, but take into account that overflow
   * must be discarded, and do not generate an error.
   */
  irandh = irand / m1;
  irandl = irand % m1;
  multh  = mult / m1;
  multl  = mult % m1;
  irand  = ((irandh*multl+irandl*multh) % m1) * m1 + irandl*multl;
  irand  = (irand + 1) % m;

  /* convert irand to a real random number between 0 and 1. */
  r = (irand / 10);
  r = r * 10 / rm;
  if ((r <= 0) || (r > 1))
    r = 0.0;
  *ig = irand;
  
  return r;
}        



