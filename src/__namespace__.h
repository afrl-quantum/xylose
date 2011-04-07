/*==============================================================================
 * Public Domain Contributions 2010 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
 * Portions copyright Copyright (C) 2010 Stellar Science                       *
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
 * Dummy file for documenting some of the general namespaces. 
 */

#ifndef xylose___namespace___h
#define xylose___namespace___h

/** \namespace xylose
 * The Xylose library:  a collection of useful computational tools. */

/** \namespace xylose::binning
 * Generally useful binning utilities.
 */

/** \namespace xylose::compat
 * Header-wrappers for particular system-level include files that are
 * particularly problematic for porting code.  Mostly, these files deal with
 * mimicking a POSIX interface for some functions on Microsoft Windows.
 */

/** \namespace xylose::distribution
 * Continuous and discrete probability distribution inversion.  
 */

/** \namespace xylose::fit
 * Classes used for fitting algorithms.
 */

/** \namespace xylose::fit::appspack
 * A threaded extension to the APPSPack library.
 */

/** \namespace xylose::nsort::map
 * A collection of useful mappings to use with xylose::nsort::NSort.  The
 * xylose::nsort::map classes are what really gives meaning to the sort.  For
 * example, a map is the logic connecting the position of a particle to a
 * particular sorting bin.
 */

/** \namespace xylose::nsort::tweak
 * Classes intended to augment the nsort maps according to preliminary counting
 * statistics.
 */

/** \namespace xylose::nsort::utility
 * Extra classes related to the NSort maps.
 */

/** \namespace xylose::timing
 * Arbitrary waveform generator.
 */

/** \namespace xylose::timing::element
 * Generating elements for arbitrary waveform generator.
 */

/** \namespace xylose::random
 * Collection of random number generators that fit a common interface.
 * The generators here are suitable for different applications.  Some require
 * only a small memory foot print due to small state vectors and some require
 * a much larger memory foot print while also providing much higher quality
 * generators (think Mersenne Twister)*/

/** \namespace xylose::xml
 * Simple, C++, context-aware, XML representation to greatly ease the
 * use/integration of XML::XPath, XML::XInclude, and libxml2. */

#endif // xylose___namespace___h
