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
 * Giant Rising Ordinary Mutants for A Clerical Setup
 */

#ifndef _angstat_h
#define _angstat_h

static char *SRCID_angstat_h = "$Id$";

#include <stdio.h>
#include <typedefs.h>

extern void do_angav(FILE *status, char *title,
		     atom_id index[], int nind, 
		     char *outfile, bool bAT);

extern void do_dihav(FILE *status, char *title,
		     atom_id index[], int nind, 
		     char *outfile, bool bAT);

extern void ramachandran(FILE *status,atom_id index[],int nind,char *outfile);

extern void dis_mon(FILE *status,atom_id index[],int nind,char *outfile,
		    t_first_x *fx, t_next_x *nx);

#endif
