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

#ifndef xylose_Dimensions_hpp
#define xylose_Dimensions_hpp

#include <ostream>

namespace xylose {

  /** Class the represent three unique dimensions.
   * The primary utility of the Dimensions<...> classes is to provide a
   * mechanism to create template metafunctions that depend on the number and
   * directions of one, two, or three dimensional coordinates.
   *
   * @see xylose::nsort::map::pivot< Dimensions< dir0, dir1, _dir > > for
   * example usage.
   */
  template < unsigned int _dir0,
             unsigned int _dir1 = _dir0,
             unsigned int _dir2 = _dir0 >
  struct Dimensions {
    static const unsigned int ndims = 3u;
    static const unsigned int dir0 = _dir0;
    static const unsigned int dir1 = _dir1;
    static const unsigned int dir2 = _dir2;

    typedef Dimensions<_dir2,_dir1,_dir0> reverse;
  };


  /** Class the represent two unique dimensions.
   * The primary utility of the Dimensions<...> classes is to provide a
   * mechanism to create template metafunctions that depend on the number and
   * directions of one, two, or three dimensional coordinates.
   *
   * @see xylose::nsort::map::pivot< Dimensions< dir0, _dir > > for
   * example usage.
   */
  template < unsigned int _dir0,
             unsigned int _dir1 >
  struct Dimensions<_dir0,_dir1,_dir0> {
    static const unsigned int ndims = 2u;
    static const unsigned int dir0 = _dir0;
    static const unsigned int dir1 = _dir1;

    typedef Dimensions<_dir1,_dir0> reverse;
  };


  /** Class the represent a one dimensional system.
   * The primary utility of the Dimensions<...> classes is to provide a
   * mechanism to create template metafunctions that depend on the number and
   * directions of one, two, or three dimensional coordinates.
   *
   * @see xylose::nsort::map::pivot< Dimensions< _dir > > for
   * example usage.
   */
  template < unsigned int _dir0 >
  struct Dimensions<_dir0,_dir0,_dir0> {
    static const unsigned int ndims = 1u;
    static const unsigned int dir0 = _dir0;

    typedef Dimensions<_dir0> reverse;
  };



  /** Stream output to show a representation of a three dimensional system. */
  template <unsigned int dir0, unsigned int dir1, unsigned int dir2>
  std::ostream & operator<< (std::ostream & out, const Dimensions<dir0,dir1,dir2> & dims) {
    out << dims.ndims << "D-{"
        << dir0 << ','
        << dir1 << ','
        << dir2 << '}';
    return out;
  }

  /** Stream output to show a representation of a two dimensional system. */
  template <unsigned int dir0, unsigned int dir1>
  std::ostream & operator<< (std::ostream & out, const Dimensions<dir0,dir1> & dims) {
    out << dims.ndims << "D-{"
        << dir0 << ','
        << dir1 << '}';
    return out;
  }

  /** Stream output to show a representation of a one dimensional system. */
  template <unsigned int dir0>
  std::ostream & operator<< (std::ostream & out, const Dimensions<dir0> & dims) {
    out << dims.ndims << "D-{"
        << dir0 << '}';
    return out;
  }

}/* namespace xylose */


#endif // xylose_Dimensions_hpp
