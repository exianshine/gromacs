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
 * Good gRace! Old Maple Actually Chews Slate
 */

#ifndef _nener_h
#define _nener_h

static char *SRCID_nener_h = "$Id$";

#ifdef HAVE_IDENT
#ident	"@(#) nener.h 1.19 9/30/97"
#endif /* HAVE_IDENT */
#include "x11.h"
#include "xutil.h"
#include "popup.h"

typedef struct {
  t_windata   wd;		/* Window struct 		*/
  t_menu      *selener;		/* The Select energy menu	*/
  int         nre,nwidth;	/* The number of terms 		*/
  int	      nlast;		/* The last frame added		*/	
  int         etype;		/* The term selected		*/
  real        **e;		/* The energy array		*/
} t_enerwin;

extern t_enerwin *init_ew(t_x11 *x11,Window Parent,
			  int x,int y,int width,int height,
			  ulong fg,ulong bg);

extern void map_ewin(t_x11 *x11,t_enerwin *ew);

extern void add_ener(t_x11 *x11,t_enerwin *ew,t_energy e[]);

extern void rewind_ener(t_x11 *x11,t_enerwin *ew);

extern void done_ew(t_x11 *x11,t_enerwin *ew);

#endif	/* _nener_h */
