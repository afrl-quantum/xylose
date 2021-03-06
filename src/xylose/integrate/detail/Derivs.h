/*==============================================================================
 * Public Domain Contributions 2010 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
 *                                                                             *
 * This file is part of xylose                                                 *
 *                                                                             *
 * This program is free software: you can redistribute it and/or modify it     *
 * under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                                  *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
 * License for more details.                                                   *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                             *
 -----------------------------------------------------------------------------*/

/** \file
 * (derivs) calling wrapper.
 */

#ifndef xylose_integrate_detail_Derivs_h
#define xylose_integrate_detail_Derivs_h

namespace xylose {
  namespace integrate {
    namespace detail {

      struct NoOp { };

      /** Derivative calling wrapper to allow an optional argument.  */
      template < typename Optional >
      struct Derivs {
        template < typename DerivativeFunctor,
                   typename Arg >
        void operator() ( DerivativeFunctor & derivs,
                          const Arg         & x,
                          const double      & t,
                          const double      & dt,
                                Arg         & F,
                                Optional    & opt ) {
          derivs( x, t, dt, F, opt );
        }
      };

      template<>
      struct Derivs<NoOp> {
        template < typename DerivativeFunctor,
                   typename Arg >
        void operator() ( DerivativeFunctor & derivs,
                          const Arg         & x,
                          const double      & t,
                          const double      & dt,
                                Arg         & F,
                          const NoOp        & ) {
          derivs( x, t, dt, F );
        }
      };

    }/* namespace xylose::integrate::detail */
  }/* namespace xylose::integrate */
}/* namespace xylose */

#endif // xylose_integrate_detail_Derivs_h
