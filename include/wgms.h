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
 * Gnomes, ROck Monsters And Chili Sauce
 */

#ifndef _wgms_h
#define _wgms_h

static char *SRCID_wgms_h = "$Id$";

#include <stdio.h>
#include "typedefs.h"

extern void write_gms(FILE *fp,int natoms,rvec x[],matrix box);
/* Write a gromos-87 trajectory frame (10f8.3) + box size 
 * If box == NULL it is not written
 */

extern void write_gms_ndx(FILE *fp,int isize,atom_id index[],
			  rvec x[],matrix box);
/* Write a gromos-87 trajectory frame (10f8.3) + box size for
 * a subset of the atoms.
 * If box == NULL it is not written
 */

#endif
