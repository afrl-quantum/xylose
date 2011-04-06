// -*- c++ -*-
// $Id$
/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2004-2008 Spencer Olson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *  
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *                                                                                 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.                                                                           .
 * 
 * Questions? Contact Spencer Olson (olsonse@umich.edu) 
 */

/** \example timing/testtiming.cpp
 * Demonstrates the xylose::timing classes.  xylose::timing classes can be
 * thought of a way to represent the output of an arbitrary waveform generator.
 * The voltage output of a waveform generator is typically used in experimental
 * work to control experimental apparatus and data acquisition hardware.  While
 * xylose::timing classes can thus be ideal for simulations that compare with k
 * experimental timing procedures, xylose::timing can be used for specifying any
 * type of timing control needed within a simulation.
 */

#ifndef xylose_timing_Timing_h
#define xylose_timing_Timing_h

#include <xylose/timing/element/Base.h>

#include <boost/ptr_container/ptr_vector.hpp>

#include <vector>
#include <stdexcept>


namespace xylose {
  namespace timing {

    typedef boost::ptr_vector<element::Base> TimingsVector;

    /** Generic timing class.  
     * This class can be used to change a particular value in a controlled
     * fashion over a given set of time intervals (defined by an array of
     * element::Base instances).
     *
     * @see timing::element::Base
     */
    class Timing {
      /* MEMBER STORAGE */
    public:
      /** Vector of timing elements for this Timing class. */
      TimingsVector timings;

    private:
      /** Value of the timer. 
       * The meaning of this value is arbitrary, and the magnitude depends on
       * the particular timing element. */
      double current_val;

      /** vector of saved times.  Useful for printing as a function of t without
       * messing up with default time settings. */
      std::vector<double> time_stack;

      /** Current absolute time:  last set time. */
      double current_time_absolute;



      /* MEMBER FUNCTIONS */
    public:
      /** Constructor zeros current time and current value. */
      Timing() : timings(),
                 current_val(0.0),
                 time_stack(),
                 current_time_absolute(0.0) {}

      /** Obtain the current value of the timing. */
      const double & getVal() const { return current_val; }

      /** Obtain the current absolute time the timing is set to. */
      const double & getTime() const { return current_time_absolute; }

      /** saves an absolute time on the time stack. */
      Timing & push_time(const double & t_abs) {
        time_stack.push_back(t_abs);
        return *this;
      }

      /** save current absolute time on the time stack.
       * @return Reference to self.  
       */
      Timing & push_time() {
        return push_time(current_time_absolute);
      }

      /** pop and restore an old timing value from the time stack. 
       * @return Reference to self.
       */
      Timing & pop_time() {
        double t_abs = time_stack.back();
        this->set_time(t_abs);
        time_stack.pop_back();
        return *this;
      }

      void incr_time( const double & dt ) {
        set_time( current_time_absolute + dt );
      }

      /** Set the current value of the timed change.
       * @param t_absolute The absolute time will define which element in the
       * time interval array is used. 
       */
      void set_time(const double & t_absolute) {
        /* set the current time for possible later use. */
        current_time_absolute = t_absolute;
        double t_i = 0.0, t_f = 0.0;
        int i = 0;
        for (; ((unsigned int)i) < timings.size(); ++i) {
          element::Base & te = timings[i];

          t_i = t_f;         /* absolute start of this interval. */
          t_f = t_i + te.dt; /* absolute   end of this interval. */

          if (t_absolute <= t_f) {
            ++i;
            break;
          }
        }
        --i;

        if (i < 0)
          throw std::runtime_error("There are no timing elements!");

        /* set the current value according to the relative time for the ith
         * time interval. */
        current_val = timings[i].getValue(t_absolute - t_i);
      }
    };

  }/* namespace xylose::timing */
}/* namespace xylose */

#endif // xylose_timing_Timing_h
