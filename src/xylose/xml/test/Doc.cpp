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


#define BOOST_TEST_MODULE  XML_Doc

#include <xylose/xml/Doc.h>
#include <xylose/strutil.h>

#include <boost/test/unit_test.hpp>

#include <ostream>
#include <sstream>

#ifndef XML_FILENAME
#  error The filename was supposed to already be defined on the command line
#endif

namespace {
  namespace xml = xylose::xml;
}

BOOST_AUTO_TEST_SUITE( XML_tests );//{



  BOOST_AUTO_TEST_SUITE( XML_Doc );//{

    BOOST_AUTO_TEST_CASE( open ) {
      xml::Doc doc(XML_FILENAME);

      doc.assertOpen();
      doc.root_context.assertContext();

      /* I checked to make sure that the context returned by a
       * root_context.find(".") operation really did point to the xmlDoc.
       */
      BOOST_CHECK_EQUAL(
        xmlDocGetRootElement(doc.doc),
        doc.root_context.node
      );

      /* I'm including this check because this is how libxml2 currently behaves.
       * If it changes, I want to know about it because it might have other
       * implications. */
      BOOST_CHECK_EQUAL(
        doc.root_context.ctx->node,
        static_cast<xmlNodePtr>(NULL)
      );
    }

    BOOST_AUTO_TEST_CASE( eval ) {
      xml::Doc doc(XML_FILENAME);

      xml::Context::list xl = doc.eval("/Equationtest/Eq");
      BOOST_CHECK_EQUAL( xl.size(), 4u );
    }

    BOOST_AUTO_TEST_CASE( find ) {
      xml::Doc doc(XML_FILENAME);

      xml::Context x = doc.find("/Equationtest/Eq[1]");
      BOOST_CHECK_EQUAL( x.text(), "2 87Rb  -->  2 87Rb" );
    }

  BOOST_AUTO_TEST_SUITE_END();//}



  BOOST_AUTO_TEST_SUITE( XML_Context );//{

    BOOST_AUTO_TEST_CASE( eval ) {
      xml::Doc doc(XML_FILENAME);

      xml::Context x = doc.find("/Equationtest/Eq[1]");

      xml::Context::list xl = x.eval("*/T/P");
      BOOST_CHECK_EQUAL( xl.size(), 2u );
    }

    BOOST_AUTO_TEST_CASE( find ) {
      xml::Doc doc(XML_FILENAME);

      xml::Context x0 = doc.find("/Equationtest/Eq[1]");

      xml::Context x = x0.find("In/T/P[1]");
      BOOST_CHECK_EQUAL( x.text(), "87Rb" );
    }

    BOOST_AUTO_TEST_CASE( parse ) {
      xml::Doc doc(XML_FILENAME);

      xml::Context x = doc.find("/Equationtest/Eq[1]");

      BOOST_CHECK_EQUAL( x.parse<std::string>(), "2 87Rb  -->  2 87Rb" );
    }


    BOOST_AUTO_TEST_CASE( query ) {
      xml::Doc doc(XML_FILENAME);

      xml::Context x = doc.find("/Equationtest/Eq[1]");

      BOOST_CHECK_EQUAL( x.query<std::string>("In/T/P[1]"), "87Rb" );
    }

    BOOST_AUTO_TEST_CASE( query_default ) {
      xml::Doc doc(XML_FILENAME);

      xml::Context x = doc.find("/Equationtest/Eq[1]");

      BOOST_CHECK_EQUAL( x.query<std::string>("In/T/P[3]", "bob"), "bob" );
    }

    BOOST_AUTO_TEST_CASE( text ) {
      xml::Doc doc(XML_FILENAME);

      xml::Context x = doc.find("/Equationtest/Eq[1]");

      BOOST_CHECK_EQUAL( x.text(), "2 87Rb  -->  2 87Rb" );
    }

  BOOST_AUTO_TEST_SUITE_END();//}



  /** THIS LITTLE TEST SUITE shows how you can aggregate parse_item
   * implementations very easily to be able to query/parse larger structures
   * from the xml.
   */
  BOOST_AUTO_TEST_SUITE( XML_parse_item );//{
    struct Term {
      int n;
      std::string p;
    };

    struct Equation {
      std::set<Term> in, out;
    };

    inline bool operator< ( const Term & lhs, const Term & rhs ) {
      return lhs.p < rhs.p;
    }

    inline std::ostream & operator<< ( std::ostream & out,
                                       const Term & t ) {
      if (t.n > 1)
        out << t.n << ' ';
      return out << t.p;
    }

    inline std::ostream & operator<< ( std::ostream & out,
                                       const std::set<Term> & s ) {
      typedef std::set<Term>::const_iterator Iter;
      const char * sep = "";
      const char * plus = " + ";
      for ( Iter i = s.begin(); i != s.end(); ++i ) {
        out << sep << (*i);
        sep = plus;
      }

      return out;
    }

    inline std::ostream & operator<< ( std::ostream & out,
                                       const Equation & eq ) {
      return out << eq.in << "  -->  " << eq.out;
    }

    inline void parse_item( Term & t,
                            const xml::Context & x ) {
      t.n = x.query<int>("n",1);
      t.p = x.query<std::string>("P");
    }

    inline void parse_item( std::set<Term> & s,
                            const xml::Context & x ) {
      typedef xml::Context::list Clist;

      Clist xl = x.eval("T");
      for ( Clist::iterator i = xl.begin(); i != xl.end(); ++i ) {
        s.insert( i->parse<Term>() );
      }
    }

    inline void parse_item( Equation & eq,
                            const xml::Context & x ) {
      eq.in  = x.query< std::set<Term> >("In");
      eq.out = x.query< std::set<Term> >("Out");
    }


    BOOST_AUTO_TEST_CASE( query ) {
      xml::Doc doc(XML_FILENAME);

      xml::Context x = doc.find("/Equationtest/Eq[4]");

      using xylose::to_string;
      BOOST_CHECK_EQUAL(
        to_string(x.parse<Equation>()),
        "87Rb + 2 e^-  -->  87Rb + 2 e^-"
      );
    }

  BOOST_AUTO_TEST_SUITE_END();//}



BOOST_AUTO_TEST_SUITE_END();//}

