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
 * Gromacs Runs On Most of All Computer Systems
 */

#ifndef _sim_util_h
#define _sim_util_h

#include "typedefs.h"
#include "mdebin.h"
#include "update.h"
#include "vcm.h"
#include "../fileio/enxio.h"
#include "../fileio/mdoutf.h"
#include "../timing/walltime_accounting.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct gmx_global_stat *gmx_global_stat_t;

void do_pbc_first(FILE *log, matrix box, t_forcerec *fr,
                  t_graph *graph, rvec x[]);

void do_pbc_first_mtop(FILE *fplog, int ePBC, matrix box,
                       gmx_mtop_t *mtop, rvec x[]);

void do_pbc_mtop(FILE *fplog, int ePBC, matrix box,
                 gmx_mtop_t *mtop, rvec x[]);



/* ROUTINES from stat.c */
gmx_global_stat_t global_stat_init(t_inputrec *ir);

void global_stat_destroy(gmx_global_stat_t gs);

void global_stat(FILE *log, gmx_global_stat_t gs,
                 t_commrec *cr, gmx_enerdata_t *enerd,
                 tensor fvir, tensor svir, rvec mu_tot,
                 t_inputrec *inputrec,
                 gmx_ekindata_t *ekind,
                 gmx_constr_t constr, t_vcm *vcm,
                 int nsig, real *sig,
                 gmx_mtop_t *top_global, t_state *state_local,
                 gmx_bool bSumEkinhOld, int flags);
/* Communicate statistics over cr->mpi_comm_mysim */

int do_per_step(gmx_large_int_t step, gmx_large_int_t nstep);
/* Return TRUE if io should be done */

/* ROUTINES from sim_util.c */

void print_time(FILE *out, gmx_walltime_accounting_t walltime_accounting,
                gmx_large_int_t step, t_inputrec *ir, t_commrec *cr);

void print_date_and_time(FILE *log, int pid, const char *title,
                         const gmx_walltime_accounting_t walltime_accounting);

void finish_run(FILE *log, t_commrec *cr,
                t_inputrec *inputrec,
                t_nrnb nrnb[], gmx_wallcycle_t wcycle,
                gmx_walltime_accounting_t walltime_accounting,
                wallclock_gpu_t *gputimes,
                gmx_bool bWriteStat);

void calc_enervirdiff(FILE *fplog, int eDispCorr, t_forcerec *fr);

void calc_dispcorr(FILE *fplog, t_inputrec *ir, t_forcerec *fr,
                   gmx_large_int_t step, int natoms,
                   matrix box, real lambda, tensor pres, tensor virial,
                   real *prescorr, real *enercorr, real *dvdlcorr);

void initialize_lambdas(FILE *fplog, t_inputrec *ir, int *fep_state, real *lambda, double *lam0);

void do_constrain_first(FILE *log, gmx_constr_t constr,
                        t_inputrec *inputrec, t_mdatoms *md,
                        t_state *state, t_commrec *cr, t_nrnb *nrnb,
                        t_forcerec *fr, gmx_localtop_t *top);

void init_md(FILE *fplog,
             t_commrec *cr, t_inputrec *ir, const output_env_t oenv,
             double *t, double *t0,
             real *lambda, int *fep_state, double *lam0,
             t_nrnb *nrnb, gmx_mtop_t *mtop,
             gmx_update_t *upd,
             int nfile, const t_filenm fnm[],
             gmx_mdoutf_t **outf, t_mdebin **mdebin,
             tensor force_vir, tensor shake_vir,
             rvec mu_tot,
             gmx_bool *bSimAnn, t_vcm **vcm, unsigned long Flags);
/* Routine in sim_util.c */

#ifdef __cplusplus
}
#endif

#endif  /* _sim_util_h */
