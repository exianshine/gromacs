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
 * S  C  A  M  O  R  G
 */
static char *SRCID_orise_h = "$Id$";

#include "typedefs.h"
#include "pinput.h"
	
extern void optim_rise(int nx,rvec *xxav,rvec **EEV,
		       real **eevprj,int nnatoms,
		       int nnca,atom_id *cca_index,
		       t_pinp *p);
		    
extern void optim_radius(int nx,rvec *xxav,rvec **EEV,
			 real **eevprj,int nnatoms,
			 int nnca,atom_id *cca_index,
			 t_pinp *p);
     
