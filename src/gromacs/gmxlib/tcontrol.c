/*
 *
 *                This source code is part of
 *
 *                 G   R   O   M   A   C   S
 *
 *          GROningen MAchine for Chemical Simulations
 *
 *                        VERSION 3.2.0
 * Written by David van der Spoel, Erik Lindahl, Berk Hess, and others.
 * Copyright (c) 1991-2000, University of Groningen, The Netherlands.
 * Copyright (c) 2001-2004, The GROMACS development team,
 * check out http://www.gromacs.org for more information.

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
 * For more info, check our website at http://www.gromacs.org
 *
 * And Hey:
 * GROningen Mixture of Alchemy and Childrens' Stories
 */
#include "gromacs/legacyheaders/statutil.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "typedefs.h"
#include "gmx_fatal.h"

#include "gromacs/legacyheaders/thread_mpi/threads.h"

/* The source code in this file should be thread-safe.
         Please keep it that way. */

/* Globals for trajectory input */
typedef struct {
    real     t;
    gmx_bool bSet;
} t_timecontrol;

static t_timecontrol timecontrol[TNR] = {
    { 0, FALSE },
    { 0, FALSE },
    { 0, FALSE }
};

static tMPI_Thread_mutex_t tc_mutex = TMPI_THREAD_MUTEX_INITIALIZER;

gmx_bool bTimeSet(int tcontrol)
{
    gmx_bool ret;

    tMPI_Thread_mutex_lock(&tc_mutex);
    range_check(tcontrol, 0, TNR);
    ret = timecontrol[tcontrol].bSet;
    tMPI_Thread_mutex_unlock(&tc_mutex);

    return ret;
}

real rTimeValue(int tcontrol)
{
    real ret;

    tMPI_Thread_mutex_lock(&tc_mutex);
    range_check(tcontrol, 0, TNR);
    ret = timecontrol[tcontrol].t;
    tMPI_Thread_mutex_unlock(&tc_mutex);
    return ret;
}

void setTimeValue(int tcontrol, real value)
{
    tMPI_Thread_mutex_lock(&tc_mutex);
    range_check(tcontrol, 0, TNR);
    timecontrol[tcontrol].t    = value;
    timecontrol[tcontrol].bSet = TRUE;
    tMPI_Thread_mutex_unlock(&tc_mutex);
}
