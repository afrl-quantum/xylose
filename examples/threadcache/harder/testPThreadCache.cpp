/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2006-2009 Spencer E. Olson
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

/** \file
 * Examples of the direct (more complex) interface to the xylose::PThreadCache
 * facility.
 *
 * The example shows work being done in each thread that produces results
 * that must be gathered together (a scatter-gather scheme).
 */

#include <xylose/PThreadCache.h>

#include <iostream>
#include <cmath>
#include <cstdlib>

namespace {

  struct Task : xylose::PThreadTask {
    /* MEMBER STORAGE */
    double xi, xf, dx;
    double retval;

    /* MEMBER FUNCTIONS */
    Task(const double & xi, const double & xf, const double dx)
      : xi(xi), xf(xf), dx(dx), retval(0) {}
    virtual ~Task() {}
    virtual void exec() {
      for ( double x = xi; x <= xf; x+= dx ) {
        retval += std::log(x);
      }
    };
  };
}

int main() {
  using xylose::pthreadCache;
  using xylose::PThreadTaskSet;

  if ( ! getenv("NUM_PTHREADS") )
    pthreadCache.set_max_threads(2);

  PThreadTaskSet tasks;

  double dx = 3e-2;
  for (double i = 1; i< 10.0; i+= dx) {
    Task * t = new Task(i, i+dx, dx*1e-5);
    tasks.insert(t);
    pthreadCache.addTask(t);
  }

  std::cout << "queued " << tasks.size() << " tasks" << std::endl;

  int n_finished = 0;
  double sum = 0;
  while ( tasks.size() ) {
    PThreadTaskSet finished = pthreadCache.waitForTasks(tasks);
    n_finished += finished.size();

    for ( PThreadTaskSet::iterator i = finished.begin();
          i != finished.end(); ++i ) {
      Task * t = static_cast<Task*>(*i);
      sum += t->retval;
      delete t;
    }

    PThreadTaskSet tmp;
    std::set_difference(tasks.begin(), tasks.end(),
                        finished.begin(), finished.end(),
                        inserter(tmp, tmp.begin()));
    tasks.swap(tmp);
  }

  std::cout << "sum:  " << sum << "\n"
               "finished " << n_finished << " tasks" << std::endl;

  return EXIT_SUCCESS;
}
