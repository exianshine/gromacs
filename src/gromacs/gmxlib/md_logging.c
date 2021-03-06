/* -*- mode: c; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; c-file-style: "stroustrup"; -*-
 *
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
 * Gallium Rubidium Oxygen Manganese Argon Carbon Silicon
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdarg.h>
#include "types/commrec.h"
#include "md_logging.h"


void md_print_info(const t_commrec *cr, FILE *fplog,
                   const char *fmt, ...)
{
    va_list ap;

    if (cr == NULL || SIMMASTER(cr))
    {
        va_start(ap, fmt);

        vfprintf(stderr, fmt, ap);

        va_end(ap);
    }
    if (fplog != NULL)
    {
        va_start(ap, fmt);

        vfprintf(fplog, fmt, ap);

        va_end(ap);
    }
}

void md_print_warn(const t_commrec *cr, FILE *fplog,
                   const char *fmt, ...)
{
    va_list ap;

    if (cr == NULL || SIMMASTER(cr))
    {
        va_start(ap, fmt);

        fprintf(stderr, "\n");
        vfprintf(stderr, fmt, ap);
        fprintf(stderr, "\n");

        va_end(ap);
    }
    if (fplog != NULL)
    {
        va_start(ap, fmt);

        fprintf(fplog, "\n");
        vfprintf(fplog, fmt, ap);
        fprintf(fplog, "\n");

        va_end(ap);
    }
}
