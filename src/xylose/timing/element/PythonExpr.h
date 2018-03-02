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


#ifndef xylose_timing_element_PythonExpr_h
#define xylose_timing_element_PythonExpr_h

#include <xylose/Singleton.hpp>
#include <xylose/timing/element/Base.h>

#include <boost/python.hpp>

#include <string>


namespace xylose {
  namespace timing {
    namespace element {

      /** Simple class to ensure that the Python interpreter is initialized and
       * finalized correctly.
       *
       * Access is only available through the Singleton interface.
       */
      class PythonInterpreter : public Singleton<PythonInterpreter> {
        friend Singleton<PythonInterpreter>;

        public:
        /* MEMBER STORAGE */
        boost::python::object main;
        boost::python::dict globals;

        private:
        PythonInterpreter() {
          Py_Initialize();

          using namespace boost::python;

          main = import("__main__");
          globals = extract<dict>(main.attr("__dict__"));
        }

        ~PythonInterpreter() {
          Py_Finalize();
        }

        public:
        void exec(const boost::python::str & cmd,
                  boost::python::object G,
                  boost::python::object L) {
          try {
            boost::python::exec(cmd, G, L);
          } catch (boost::python::error_already_set const & e) {
            PyErr_Print();
            throw;
          }
        }

        void exec(const boost::python::str & cmd, boost::python::object L) {
          exec(cmd, globals, L);
        }
      };

      /** Python Expression timing element. */
      struct PythonExpr : timing::element::Base {
        /* TYPEDEFS */
        typedef timing::element::Base super;

        /* MEMBER STORAGE */
        /** Python expression to evaluate.
         * The following variables are automatically available:
         * - U0 : The initial value.
         * - U1 : The final value.
         * - x  : Relative time running from 0 to 1 over the duration of the
         *        timing::element.
         */
        std::string expr;

        /** Local variables to this timing element. */
        boost::python::dict locals;


        /* MEMBER FUNCTIONS */
        /** Constructor. */
        PythonExpr(const double & dt    = 0.0,
                   const std::string & expr = "U0",
                   const double & U0    = 0.0,
                   const double & U1    = 1.0,
                   const std::string & env = "") :
          super(dt), expr(expr) {
          using namespace boost::python;

          /* make sure interpreter is running */
          interpreter()->exec(
            "globals().setdefault('__Timing__',0)\n"
            "__Timing__ += 1\n", locals);
          locals["U0"] = U0;
          locals["U1"] = U1;
          if (env.length()) {
            interpreter()->exec(env.c_str(), locals);
          }
        }

        /** Required virtual destructor.  This one is a NO-OP. */
        virtual ~PythonExpr() {}

        static PythonInterpreter * interpreter() {
          return PythonInterpreter::instance();
        }

        /** Calculates the value of this timing element. */
        virtual double getValue(const double & t_rel) {
          using namespace boost::python;
          const double tau = (t_rel / dt);

          if (t_rel >= dt) {
            return extract<double>(locals["U1"]);
          } else if (t_rel <= 0.0) {
            return extract<double>(locals["U0"]);
          } else {
            dict & G = interpreter()->globals;
            locals["x"] = tau;
            return extract<double>(eval(expr.c_str(), G, locals));
          }
        }

        /** Create a clone. */
        virtual Base * clone() const {
          return new PythonExpr(*this);
        }
      };

    }/* namespace xylose::timing::element */
  }/* namespace xylose::timing */
}/* namespace xylose */

#endif // xylose_timing_element_PythonExpr_h
