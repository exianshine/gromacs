/*
 * This file is part of the GROMACS molecular simulation package.
 *
 * Copyright (c) 2012,2013, by the GROMACS development team, led by
 * Mark Abraham, David van der Spoel, Berk Hess, and Erik Lindahl,
 * and including many others, as listed in the AUTHORS file in the
 * top-level source directory and at http://www.gromacs.org.
 *
 * GROMACS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * GROMACS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GROMACS; if not, see
 * http://www.gnu.org/licenses, or write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
 *
 * If you want to redistribute modifications to GROMACS, please
 * consider that scientific software is very special. Version
 * control is crucial - bugs must be traceable. We will be happy to
 * consider code for inclusion in the official distribution, but
 * derived work must not be called official GROMACS. Details are found
 * in the README & COPYING files - if they are missing, get the
 * official version at http://www.gromacs.org.
 *
 * To help us fund GROMACS development, we humbly ask that you cite
 * the research papers on the package. Check out http://www.gromacs.org.
 */
/*! \libinternal \file
 * \brief
 * Declares OpenMP wrappers to avoid conditional compilation.
 *
 * This module defines wrappers for OpenMP API functions and enables compiling
 * code without conditional compilation even when OpenMP is turned off in the
 * build system.
 * Therefore, OpenMP API functions should always be used through these wrappers
 * and omp.h should never be directly included.  Instead, this header should be
 * used whenever OpenMP API functions are needed.
 *
 * \ingroup module_utility
 */
#ifndef GMX_UTILITY_OMP_H
#define GMX_UTILITY_OMP_H

#include "types/commrec.h"
#include "mdrun.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*! \addtogroup module_utility
 * \{
 */

/*! \brief
 * Returns an integer equal to or greater than the number of threads
 * that would be available if a parallel region without num_threads were
 * defined at that point in the code.
 *
 * Acts as a wrapper for omp_get_max_threads().
 */
int gmx_omp_get_max_threads(void);

/*! \brief
 * Returns the number of processors available when the function is called.
 *
 * Acts as a wrapper around omp_get_num_procs().
 */
int gmx_omp_get_num_procs(void);

/*! \brief
 * Returns the thread number of the thread executing within its thread team.
 *
 * Acts as a wrapper for omp_get_thread_num().
 */
int gmx_omp_get_thread_num(void);

/*! \brief
 * Sets the number of threads in subsequent parallel regions, unless overridden
 * by a num_threads clause.
 *
 * Acts as a wrapper for omp_set_num_threads().
 */
void gmx_omp_set_num_threads(int num_threads);

/*! \brief
 * Check for externally set thread affinity to avoid conflicts with \Gromacs
 * internal setting.
 */
void gmx_omp_check_thread_affinity(FILE *fplog, const t_commrec *cr,
                                   gmx_hw_opt_t *hw_opt);

/*! \} */

#ifdef __cplusplus
}
#endif

#endif
