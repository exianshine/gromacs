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
 * Good ROcking Metal Altar for Chronical Sinners
 */

#ifndef _init_sh_h
#define _init_sh_h

static char *SRCID_init_sh_h = "$Id$";

#include <stdio.h>
#include "typedefs.h"
	
typedef struct {
  atom_id shell;	/* The shell id				*/
  atom_id nucl1;	/* The nucleus connected to the shell	*/
  real    k_1;		/* 1 over force constant		*/
} t_atomshell;

typedef struct {
  atom_id shell;	/* The shell id				*/
  atom_id nucl1,nucl2;	/* The nuclei connected to the shell	*/
  real    k_1;		/* 1 over force constant		*/
} t_bondshell;

extern void init_shells(FILE *log,int start,int homenr,
			t_idef *idef,t_mdatoms *md,
			int *nashell,t_atomshell **atom_shell,
			int *nbshell,t_bondshell **bond_shell);

extern void pr_ashell(FILE *log,int nas,t_atomshell as[]);

extern void pr_bshell(FILE *log,int nbs,t_bondshell bs[]);

#endif
