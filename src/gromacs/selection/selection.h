/*
 * This file is part of the GROMACS molecular simulation package.
 *
 * Copyright (c) 2009,2010,2011,2012,2013, by the GROMACS development team, led by
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
 * Declares gmx::Selection and supporting classes.
 *
 * \author Teemu Murtola <teemu.murtola@gmail.com>
 * \inpublicapi
 * \ingroup module_selection
 */
#ifndef GMX_SELECTION_SELECTION_H
#define GMX_SELECTION_SELECTION_H

#include <string>
#include <vector>

#include "../legacyheaders/typedefs.h"

#include "../utility/arrayref.h"
#include "../utility/common.h"
#include "../utility/gmxassert.h"

#include "position.h"
#include "indexutil.h"
#include "selectionenums.h"

namespace gmx
{

class SelectionOptionStorage;
class SelectionTreeElement;

class AnalysisNeighborhoodPositions;
class Selection;
class SelectionPosition;

//! Container of selections used in public selection interfaces.
typedef std::vector<Selection> SelectionList;

namespace internal
{

/*! \internal
 * \brief
 * Internal data for a single selection.
 *
 * This class is internal to the selection module, but resides in a public
 * header because of efficiency reasons: it allows frequently used access
 * methods in \ref Selection to be inlined.
 *
 * Methods in this class do not throw unless otherwise specified.
 *
 * \ingroup module_selection
 */
class SelectionData
{
    public:
        /*! \brief
         * Creates a new selection object.
         *
         * \param[in] elem   Root of the evaluation tree for this selection.
         * \param[in] selstr String that was parsed to produce this selection.
         * \throws    std::bad_alloc if out of memory.
         */
        SelectionData(SelectionTreeElement *elem, const char *selstr);
        ~SelectionData();

        //! Returns the name for this selection.
        const char *name() const { return name_.c_str(); }
        //! Returns the string that was parsed to produce this selection.
        const char *selectionText() const { return selectionText_.c_str(); }
        //! Returns true if the size of the selection (posCount()) is dynamic.
        bool isDynamic() const { return bDynamic_; }
        //! Returns the type of positions in the selection.
        e_index_t type() const { return rawPositions_.m.type; }

        //! Number of positions in the selection.
        int posCount() const { return rawPositions_.count(); }
        //! Returns the root of the evaluation tree for this selection.
        SelectionTreeElement &rootElement() { return rootElement_; }

        //! Returns whether the covered fraction can change between frames.
        bool isCoveredFractionDynamic() const { return bDynamicCoveredFraction_; }

        //! Returns true if the given flag is set.
        bool hasFlag(SelectionFlag flag) const { return flags_.test(flag); }
        //! Sets the flags for this selection.
        void setFlags(SelectionFlags flags) { flags_ = flags; }

        //! \copydoc Selection::initCoveredFraction()
        bool initCoveredFraction(e_coverfrac_t type);

        /*! \brief
         * Updates the name of the selection if missing.
         *
         * \throws    std::bad_alloc if out of memory.
         *
         * If selections get their value from a group reference that cannot be
         * resolved during parsing, the name is final only after group
         * references have been resolved.
         *
         * This function is called by SelectionCollection::setIndexGroups().
         */
        void refreshName();
        /*! \brief
         * Computes total masses and charges for all selection positions.
         *
         * \param[in] top   Topology information.
         * \throws    std::bad_alloc if out of memory.
         *
         * For dynamic selections, the values need to be updated after each
         * evaluation with refreshMassesAndCharges().
         * This is done by SelectionEvaluator.
         *
         * This function is called by SelectionCompiler.
         *
         * Strong exception safety guarantee.
         */
        void initializeMassesAndCharges(const t_topology *top);
        /*! \brief
         * Updates masses and charges after dynamic selection has been
         * evaluated.
         *
         * \param[in] top   Topology information.
         *
         * Called by SelectionEvaluator.
         */
        void refreshMassesAndCharges(const t_topology *top);
        /*! \brief
         * Updates the covered fraction after a selection has been evaluated.
         *
         * Called by SelectionEvaluator.
         */
        void updateCoveredFractionForFrame();
        /*! \brief
         * Computes average covered fraction after all frames have been evaluated.
         *
         * \param[in] nframes  Number of frames that have been evaluated.
         *
         * \p nframes should be equal to the number of calls to
         * updateCoveredFractionForFrame().
         * Called by SelectionEvaluator::evaluateFinal().
         */
        void computeAverageCoveredFraction(int nframes);
        /*! \brief
         * Restores position information to state it was in after compilation.
         *
         * \param[in] top   Topology information.
         *
         * Depends on SelectionCompiler storing the original atoms in the
         * \a rootElement_ object.
         * Called by SelectionEvaluator::evaluateFinal().
         */
        void restoreOriginalPositions(const t_topology *top);

    private:
        //! Name of the selection.
        std::string               name_;
        //! The actual selection string.
        std::string               selectionText_;
        //! Low-level representation of selected positions.
        gmx_ana_pos_t             rawPositions_;
        //! Total masses for the current positions.
        std::vector<real>         posMass_;
        //! Total charges for the current positions.
        std::vector<real>         posCharge_;
        SelectionFlags            flags_;
        //! Root of the selection evaluation tree.
        SelectionTreeElement     &rootElement_;
        //! Type of the covered fraction.
        e_coverfrac_t             coveredFractionType_;
        //! Covered fraction of the selection for the current frame.
        real                      coveredFraction_;
        //! The average covered fraction (over the trajectory).
        real                      averageCoveredFraction_;
        //! true if the value can change as a function of time.
        bool                      bDynamic_;
        //! true if the covered fraction depends on the frame.
        bool                      bDynamicCoveredFraction_;

        /*! \brief
         * Needed to wrap access to information.
         */
        friend class gmx::Selection;
        /*! \brief
         * Needed for proper access to position information.
         */
        friend class gmx::SelectionPosition;

        GMX_DISALLOW_COPY_AND_ASSIGN(SelectionData);
};

}   // namespace internal

/*! \brief
 * Provides access to a single selection.
 *
 * This class provides a public interface for accessing selection information.
 * General information about the selection can be accessed with methods name(),
 * selectionText(), isDynamic(), and type().  The first three can be accessed
 * any time after the selection has been parsed, and type() can be accessed
 * after the selection has been compiled.
 *
 * There are a few methods that can be used to change the behavior of the
 * selection.  setEvaluateVelocities() and setEvaluateForces() can be called
 * before the selection is compiled to request evaluation of velocities and/or
 * forces in addition to coordinates.
 *
 * Each selection is made of a set of positions.  Each position has associated
 * coordinates, and possibly velocities and forces if they have been requested
 * and are available.  It also has a set of atoms associated with it; typically
 * the coordinates are the center-of-mass or center-of-geometry coordinates for
 * that set of atoms.  To access the number of positions in the selection, use
 * posCount().  To access individual positions, use position().
 * See SelectionPosition for details of how to use individual positions.
 * setOriginalId() can be used to adjust the return value of
 * SelectionPosition::mappedId(); see that method for details.
 *
 * It is also possible to access the list of atoms that make up all the
 * positions directly: atomCount() returns the total number of atoms in the
 * selection and atomIndices() an array of their indices.
 * Similarly, it is possible to access the coordinates and other properties
 * of the positions as continuous arrays through coordinates(), velocities(),
 * forces(), masses(), charges(), refIds(), and mappedIds().
 *
 * Both positions and atoms can be accessed after the selection has been
 * compiled.  For dynamic selections, the return values of these methods change
 * after each evaluation to reflect the situation for the current frame.
 * Before any frame has been evaluated, these methods return the maximal set
 * to which the selection can evaluate.
 *
 * There are two possible modes for how positions for dynamic selections are
 * handled.  In the default mode, posCount() can change, and for each frame,
 * only the positions that are selected in that frame can be accessed.  In a
 * masked mode, posCount() remains constant, i.e., the positions are always
 * evaluated for the maximal set, and SelectionPosition::selected() is used to
 * determine whether a position is selected for a frame.  The masked mode can
 * be requested with SelectionOption::dynamicMask().
 *
 * The class also provides methods for printing out information: printInfo()
 * and printDebugInfo().  These are mainly for internal use by Gromacs.
 *
 * This class works like a pointer type: copying and assignment is lightweight,
 * and all copies work interchangeably, accessing the same internal data.
 *
 * Methods in this class do not throw.
 *
 * \see SelectionPosition
 *
 * \inpublicapi
 * \ingroup module_selection
 */
class Selection
{
    public:
        /*! \brief
         * Creates a selection wrapper that has no associated selection.
         *
         * Any attempt to call methods in the object before a selection is
         * assigned results in undefined behavior.
         */
        Selection() : sel_(NULL) {}
        /*! \brief
         * Creates a new selection object.
         *
         * \param  sel  Selection data to wrap.
         *
         * Only for internal use by the selection module.
         */
        explicit Selection(internal::SelectionData *sel) : sel_(sel) {}

        //! Returns the name of the selection.
        const char *name() const  { return data().name(); }
        //! Returns the string that was parsed to produce this selection.
        const char *selectionText() const { return data().selectionText(); }
        //! Returns true if the size of the selection (posCount()) is dynamic.
        bool isDynamic() const { return data().isDynamic(); }
        //! Returns the type of positions in the selection.
        e_index_t type() const { return data().type(); }

        //! Total number of atoms in the selection.
        int atomCount() const
        {
            return data().rawPositions_.m.mapb.nra;
        }
        //! Returns atom indices of all atoms in the selection.
        ConstArrayRef<int> atomIndices() const
        {
            return ConstArrayRef<int>(sel_->rawPositions_.m.mapb.a,
                                      sel_->rawPositions_.m.mapb.nra);
        }
        //! Number of positions in the selection.
        int posCount() const { return data().posCount(); }
        //! Access a single position.
        SelectionPosition position(int i) const;
        //! Returns coordinates for this selection as a continuous array.
        ConstArrayRef<rvec> coordinates() const
        {
            return ConstArrayRef<rvec>(data().rawPositions_.x, posCount());
        }
        //! Returns whether velocities are available for this selection.
        bool hasVelocities() const { return data().rawPositions_.v != NULL; }
        /*! \brief
         * Returns velocities for this selection as a continuous array.
         *
         * Must not be called if hasVelocities() returns false.
         */
        ConstArrayRef<rvec> velocities() const
        {
            GMX_ASSERT(hasVelocities(), "Velocities accessed, but unavailable");
            return ConstArrayRef<rvec>(data().rawPositions_.v, posCount());
        }
        //! Returns whether forces are available for this selection.
        bool hasForces() const { return sel_->rawPositions_.f != NULL; }
        /*! \brief
         * Returns forces for this selection as a continuous array.
         *
         * Must not be called if hasForces() returns false.
         */
        ConstArrayRef<rvec> forces() const
        {
            GMX_ASSERT(hasForces(), "Forces accessed, but unavailable");
            return ConstArrayRef<rvec>(data().rawPositions_.f, posCount());
        }
        //! Returns masses for this selection as a continuous array.
        ConstArrayRef<real> masses() const
        {
            // posMass_ may have more entries than posCount() in the case of
            // dynamic selections that don't have a topology
            // (and thus the masses and charges are fixed).
            GMX_ASSERT(data().posMass_.size() >= static_cast<size_t>(posCount()),
                       "Internal inconsistency");
            return ConstArrayRef<real>(data().posMass_.begin(),
                                       data().posMass_.begin() + posCount());
        }
        //! Returns charges for this selection as a continuous array.
        ConstArrayRef<real> charges() const
        {
            // posCharge_ may have more entries than posCount() in the case of
            // dynamic selections that don't have a topology
            // (and thus the masses and charges are fixed).
            GMX_ASSERT(data().posCharge_.size() >= static_cast<size_t>(posCount()),
                       "Internal inconsistency");
            return ConstArrayRef<real>(data().posCharge_.begin(),
                                       data().posCharge_.begin() + posCount());
        }
        /*! \brief
         * Returns reference IDs for this selection as a continuous array.
         *
         * \see SelectionPosition::refId()
         */
        ConstArrayRef<int> refIds() const
        {
            return ConstArrayRef<int>(data().rawPositions_.m.refid, posCount());
        }
        /*! \brief
         * Returns mapped IDs for this selection as a continuous array.
         *
         * \see SelectionPosition::mappedId()
         */
        ConstArrayRef<int> mappedIds() const
        {
            return ConstArrayRef<int>(data().rawPositions_.m.mapid, posCount());
        }

        //! Returns whether the covered fraction can change between frames.
        bool isCoveredFractionDynamic() const { return data().isCoveredFractionDynamic(); }
        //! Returns the covered fraction for the current frame.
        real coveredFraction() const { return data().coveredFraction_; }

        /*! \brief
         * Allows passing a selection directly to neighborhood searching.
         *
         * When initialized this way, AnalysisNeighborhoodPair objects return
         * indices that can be used to index the selection positions with
         * position().
         *
         * Works exactly like if AnalysisNeighborhoodPositions had a
         * constructor taking a Selection object as a parameter.
         * See AnalysisNeighborhoodPositions for rationale and additional
         * discussion.
         */
        operator AnalysisNeighborhoodPositions() const;

        /*! \brief
         * Initializes information about covered fractions.
         *
         * \param[in] type Type of covered fraction required.
         * \returns   true if the covered fraction can be calculated for the
         *      selection.
         */
        bool initCoveredFraction(e_coverfrac_t type)
        {
            return data().initCoveredFraction(type);
        }
        /*! \brief
         * Sets whether this selection evaluates velocities for positions.
         *
         * \param[in] bEnabled  If true, velocities are evaluated.
         *
         * If you request the evaluation, but then evaluate the selection for
         * a frame that does not contain velocity information, results are
         * undefined.
         *
         * \todo
         * Implement it such that in the above case, hasVelocities() will
         * return false for such frames.
         *
         * Does not throw.
         */
        void setEvaluateVelocities(bool bEnabled)
        {
            data().flags_.set(efSelection_EvaluateVelocities, bEnabled);
        }
        /*! \brief
         * Sets whether this selection evaluates forces for positions.
         *
         * \param[in] bEnabled  If true, forces are evaluated.
         *
         * If you request the evaluation, but then evaluate the selection for
         * a frame that does not contain force information, results are
         * undefined.
         *
         * Does not throw.
         */
        void setEvaluateForces(bool bEnabled)
        {
            data().flags_.set(efSelection_EvaluateForces, bEnabled);
        }

        /*! \brief
         * Sets the ID for the \p i'th position for use with
         * SelectionPosition::mappedId().
         *
         * \param[in] i  Zero-based index
         * \param[in] id Identifier to set.
         *
         * This method is not part of SelectionPosition because that interface
         * only provides access to const data by design.
         *
         * This method can only be called after compilation, before the
         * selection has been evaluated for any frame.
         *
         * \see SelectionPosition::mappedId()
         */
        void setOriginalId(int i, int id) { data().rawPositions_.m.orgid[i] = id; }

        /*! \brief
         * Prints out one-line description of the selection.
         *
         * \param[in] fp      Where to print the information.
         *
         * The output contains the name of the selection, the number of atoms
         * and the number of positions, and indication of whether the selection
         * is dynamic.
         */
        void printInfo(FILE *fp) const;
        /*! \brief
         * Prints out extended information about the selection for debugging.
         *
         * \param[in] fp      Where to print the information.
         * \param[in] nmaxind Maximum number of values to print in lists
         *      (-1 = print all).
         */
        void printDebugInfo(FILE *fp, int nmaxind) const;

    private:
        internal::SelectionData &data()
        {
            GMX_ASSERT(sel_ != NULL,
                       "Attempted to access uninitialized selection");
            return *sel_;
        }
        const internal::SelectionData &data() const
        {
            GMX_ASSERT(sel_ != NULL,
                       "Attempted to access uninitialized selection");
            return *sel_;
        }

        /*! \brief
         * Pointer to internal data for the selection.
         *
         * The memory for this object is managed by a SelectionCollection
         * object, and the \ref Selection class simply provides a public
         * interface for accessing the data.
         */
        internal::SelectionData *sel_;

        /*! \brief
         * Needed to access the data to adjust flags.
         */
        friend class SelectionOptionStorage;
};

/*! \brief
 * Provides access to information about a single selected position.
 *
 * Each position has associated coordinates, and possibly velocities and forces
 * if they have been requested and are available.  It also has a set of atoms
 * associated with it; typically the coordinates are the center-of-mass or
 * center-of-geometry coordinates for that set of atoms.  It is possible that
 * there are not atoms associated if the selection has been provided as a fixed
 * position.
 *
 * After the selection has been compiled, but not yet evaluated, the contents
 * of the coordinate, velocity and force vectors are undefined.
 *
 * Default copy constructor and assignment operators are used, and work as
 * intended: the copy references the same position and works identically.
 *
 * Methods in this class do not throw.
 *
 * \see Selection
 *
 * \inpublicapi
 * \ingroup module_selection
 */
class SelectionPosition
{
    public:
        /*! \brief
         * Constructs a wrapper object for given selection position.
         *
         * \param[in] sel    Selection from which the position is wrapped.
         * \param[in] index  Zero-based index of the position to wrap.
         *
         * Asserts if \p index is out of range.
         *
         * Only for internal use of the library.  To obtain a SelectionPosition
         * object in other code, use Selection::position().
         */
        SelectionPosition(const internal::SelectionData &sel, int index)
            : sel_(&sel), i_(index)
        {
            GMX_ASSERT(index >= 0 && index < sel.posCount(),
                       "Invalid selection position index");
        }

        /*! \brief
         * Returns type of this position.
         *
         * Currently always returns the same as Selection::type().
         */
        e_index_t type() const { return sel_->type(); }
        //! Returns coordinates for this position.
        const rvec &x() const
        {
            return sel_->rawPositions_.x[i_];
        }
        /*! \brief
         * Returns velocity for this position.
         *
         * Must not be called if Selection::hasVelocities() returns false.
         */
        const rvec &v() const
        {
            GMX_ASSERT(sel_->rawPositions_.v != NULL,
                       "Velocities accessed, but unavailable");
            return sel_->rawPositions_.v[i_];
        }
        /*! \brief
         * Returns force for this position.
         *
         * Must not be called if Selection::hasForces() returns false.
         */
        const rvec &f() const
        {
            GMX_ASSERT(sel_->rawPositions_.f != NULL,
                       "Velocities accessed, but unavailable");
            return sel_->rawPositions_.f[i_];
        }
        /*! \brief
         * Returns total mass for this position.
         *
         * Returns the total mass of atoms that make up this position.
         * If there are no atoms associated or masses are not available,
         * returns unity.
         */
        real mass() const
        {
            return sel_->posMass_[i_];
        }
        /*! \brief
         * Returns total charge for this position.
         *
         * Returns the sum of charges of atoms that make up this position.
         * If there are no atoms associated or charges are not available,
         * returns zero.
         */
        real charge() const
        {
            return sel_->posCharge_[i_];
        }
        //! Returns the number of atoms that make up this position.
        int atomCount() const
        {
            return sel_->rawPositions_.m.mapb.index[i_ + 1]
                   - sel_->rawPositions_.m.mapb.index[i_];
        }
        //! Return atom indices that make up this position.
        ConstArrayRef<int> atomIndices() const
        {
            const int *atoms = sel_->rawPositions_.m.mapb.a;
            if (atoms == NULL)
            {
                return ConstArrayRef<int>();
            }
            const int first = sel_->rawPositions_.m.mapb.index[i_];
            return ConstArrayRef<int>(&atoms[first], atomCount());
        }
        /*! \brief
         * Returns whether this position is selected in the current frame.
         *
         * The return value is equivalent to \c refid() == -1.  Returns always
         * true if SelectionOption::dynamicMask() has not been set.
         *
         * \see refId()
         */
        bool selected() const
        {
            return refId() >= 0;
        }
        /*! \brief
         * Returns reference ID for this position.
         *
         * For dynamic selections, this provides means to associate positions
         * across frames.  After compilation, these IDs are consequently
         * numbered starting from zero.  For each frame, the ID then reflects
         * the location of the position in the original array of positions.
         * If SelectionOption::dynamicMask() has been set for the parent
         * selection, the IDs for positions not present in the current
         * selection are set to -1, otherwise they are removed completely.
         *
         * Example:
         * If a dynamic selection consists of at most three positions, after
         * compilation refId() will return 0, 1, 2 for them, respectively.
         * If for a particular frame, only the first and the third are present,
         * refId() will return 0, 2.
         * If SelectionOption::dynamicMask() has been set, all three positions
         * can be accessed also for that frame and refId() will return 0, -1,
         * 2.
         */
        int refId() const
        {
            return sel_->rawPositions_.m.refid[i_];
        }
        /*! \brief
         * Returns mapped ID for this position.
         *
         * Returns ID of the position that corresponds to that set with
         * Selection::setOriginalId().
         *
         * If for an array \c id, \c setOriginalId(i, id[i]) has been called
         * for each \c i, then it always holds that
         * \c mappedId()==id[refId()].
         *
         * Selection::setOriginalId() has not been called, the default values
         * are dependent on type():
         *  - ::INDEX_ATOM: atom indices
         *  - ::INDEX_RES:  residue numbers
         *  - ::INDEX_MOL:  molecule numbers
         *  .
         * All the default values are zero-based
         */
        int mappedId() const
        {
            return sel_->rawPositions_.m.mapid[i_];
        }

        /*! \brief
         * Allows passing a selection position directly to neighborhood searching.
         *
         * When initialized this way, AnalysisNeighborhoodPair objects return
         * the index that can be used to access this position using
         * Selection::position().
         *
         * Works exactly like if AnalysisNeighborhoodPositions had a
         * constructor taking a SelectionPosition object as a parameter.
         * See AnalysisNeighborhoodPositions for rationale and additional
         * discussion.
         */
        operator AnalysisNeighborhoodPositions() const;

    private:
        const internal::SelectionData  *sel_;
        int                             i_;
};


inline SelectionPosition
Selection::position(int i) const
{
    return SelectionPosition(data(), i);
}

} // namespace gmx

#endif
