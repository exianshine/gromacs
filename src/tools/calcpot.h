/*
 * $Id$
 * 
 *                This source code is part of
 * 
 *                 G   R   O   M   A   C   S
 * 
 *          GROningen MAchine for Chemical Simulations
 * 
 *                        VERSION 3.0
 * 
 * Copyright (c) 1991-2001
 * BIOSON Research Institute, Dept. of Biophysical Chemistry
 * University of Groningen, The Netherlands
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * If you want to redistribute modifications, please consider that
 * scientific software is very special. Version control is crucial -
 * bugs must be traceable. We will be happy to consider code for
 * inclusion in the official distribution, but derived work must not
 * be called official GROMACS. Details are found in the README & COPYING
 * files - if they are missing, get the official version at www.gromacs.org.
 * 
 * To help us fund GROMACS development, we humbly ask that you cite
 * the papers on the package - you can find them in the top README file.
 * 
 * Do check out http://www.gromacs.org , or mail us at gromacs@gromacs.org .
 * 
 * And Hey:
 * S  C  A  M  O  R  G
 */
static char *SRCID_calcpot_h = "$Id$";
	
extern void init_calcpot(int nfile,t_filenm fnm[],t_topology *top,
			 rvec **x,t_parm *parm,t_commrec *cr,
			 t_graph **graph,t_mdatoms **mdatoms,
			 t_nsborder *nsb,t_groups *grps,
			 t_forcerec **fr,real **coulomb,
			 matrix box);

extern void calc_pot(FILE *logf,t_nsborder *nsb,t_commrec *cr,t_groups *grps,
		     t_parm *parm,t_topology *top,rvec x[],t_forcerec *fr,
		     t_mdatoms *mdatoms,real coulomb[]);

extern void write_pdb_coul();

extern void delete_atom(t_topology *top,int inr);
/* Delete an atom from a topology */

extern void replace_atom(t_topology *top,int inr,char *anm,char *resnm,
			 real q,real m,int type);
/* Replace an atom in a topology by someting else */

