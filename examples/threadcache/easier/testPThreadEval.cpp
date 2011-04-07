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
 * Two examples of the simpler task evaluator interface to the xylose::PThreadCache
 * facility.
 *
 * The first example shows work being done in each thread that produces results
 * that must be gathered together (a scatter-gather scheme).
 *
 * The second example demonstrates independent work being done in each thread
 * that does not produced gatherable results (a scatter-only scheme).
 */


#include <xylose/PThreadEval.h>

#include <sstream>
#include <iostream>

#include <cmath>
#include <cstdlib>

#include <unistd.h>
#include <pthread.h>

namespace {

  /** Storage for the gathering operation. */
  struct Gather {
    double sum;
    Gather() : sum(0.0) { }
  };

  /** The work that is scattered to each thread. */
  struct DoWork : xylose::DefaultPThreadFunctor {
    /* MEMBER STORAGE */
    double xi, xf, dx;
    double retval;

    /* MEMBER FUNCTIONS */
    DoWork(const double & xi, const double & xf, const double dx)
      : xi(xi), xf(xf), dx(dx), retval(0) {}

    /** The actual work done by the thread. */
    void operator() () {
      for ( double x = xi; x <= xf; x+= dx ) {
        retval += std::log(x);
      }
    }

    /** store the results in the gatherer. */
    void accept( Gather & gatherer ) const {
      gatherer.sum += retval;
    }
  };


  /** Scatter-only (no gathering) work done by each thread. */
  struct PrintThreadId : xylose::DefaultPThreadFunctor {
    void operator() () {
      std::ostringstream ostr;
      ostr << "thread " << pthread_self() << " working!\n";
      std::cout << ostr.str() << std::flush;
      sleep(1);
    }
  };

}

int main() {

  if ( ! getenv("NUM_PTHREADS") )
    xylose::pthreadCache.set_max_threads(2);

  {
    std::cout << "work that requires a gather operation:" << std::endl;

    xylose::PThreadEval<DoWork> evaluator;

    unsigned int ntasks = 0;
    double dx = 3e-2;
    for (double i = 1; i< 10.0; i+= dx) {
      evaluator.eval( DoWork(i, i+dx, dx*1e-5) );
      ++ntasks;
    }

    std::cout << "queued " << ntasks << " tasks" << std::endl;

    Gather gather;
    evaluator.joinAll( gather );
    std::cout << "sum:  " << gather.sum << "\n"
                 "finished " << ntasks << " tasks" << std::endl;
  }

  {
    std::cout << "work that doesn't require a gather operation:" << std::endl;

    xylose::PThreadEval<PrintThreadId> evaluator;
    evaluator.eval( PrintThreadId() );
    evaluator.eval( PrintThreadId() );
    evaluator.eval( PrintThreadId() );
    evaluator.joinAll();
  }

  return EXIT_SUCCESS;
}
