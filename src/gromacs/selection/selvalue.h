/*
 * This file is part of the GROMACS molecular simulation package.
 *
 * Copyright (c) 2009,2010,2011, by the GROMACS development team, led by
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
/*! \file
 * \brief
 * Declares ::gmx_ana_selvalue_t.
 *
 * There should be no need to use the data structures in this file directly
 * unless implementing a custom selection routine.
 *
 * \author Teemu Murtola <teemu.murtola@gmail.com>
 * \ingroup module_selection
 */
#ifndef GMX_SELECTION_SELVALUE_H
#define GMX_SELECTION_SELVALUE_H

#include "../legacyheaders/types/simple.h"

/** Defines the value type of a different selection objects. */
typedef enum
{
    NO_VALUE,           /**< No value; either an error condition or an boolean
                             parameter. */
    INT_VALUE,          /**< One or more integer values. */
    REAL_VALUE,         /**< One or more real values. */
    STR_VALUE,          /**< One or more string values. */
    POS_VALUE,          /**< One or more position values. */
    GROUP_VALUE         /**< One group of atoms. */
} e_selvalue_t;

/*! \brief
 * Describes a value of a selection expression or of a selection method
 * parameter.
 *
 * Which field in the union is used depends on the \p type.
 */
typedef struct gmx_ana_selvalue_t
{
    /** Type of the value. */
    e_selvalue_t                type;
    /*! \brief
     * Number of values in the array pointed by the union.
     *
     * Note that for position and group values, it is the number of
     * data structures in the array, not the number of positions or
     * the number of atoms in the group.
     */
    int                         nr;
    /** Pointer to the value. */
    union {
        /*! \brief
         * Generic pointer for operations that do not need type information.
         *
         * Needs to be the first member to be able to use initialized arrays.
         */
        void                   *ptr;
        /** Integer value(s) (type \ref INT_VALUE). */
        int                    *i;
        /** Real value(s) (type \ref REAL_VALUE). */
        real                   *r;
        /** String value(s) (type \ref STR_VALUE). */
        char                  **s;
        /** Structure for the position value(s) (type \ref POS_VALUE). */
        struct gmx_ana_pos_t   *p;
        /** Group value (type \ref GROUP_VALUE). */
        struct gmx_ana_index_t *g;
        /** Boolean value (only parameters of type \ref NO_VALUE); */
        bool                   *b;
    }                           u;
    /*! \brief
     * Number of elements allocated for the value array.
     */
    int                         nalloc;
} gmx_ana_selvalue_t;

/** Initializes an empty selection value structure. */
void
_gmx_selvalue_clear(gmx_ana_selvalue_t *val);
/** Reserve memory for storing selection values. */
int
_gmx_selvalue_reserve(gmx_ana_selvalue_t *val, int n);
/** Sets the memory for storing selection values. */
int
_gmx_selvalue_setstore(gmx_ana_selvalue_t *val, void *ptr);
/** Sets the memory for storing selection values and marks it for automatic freeing. */
int
_gmx_selvalue_setstore_alloc(gmx_ana_selvalue_t *val, void *ptr, int nalloc);

#endif
