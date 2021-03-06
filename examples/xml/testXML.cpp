/** \file
 * Example for parsing runtime::physical::calc::Driver expressions using
 * xml::Doc class.
 *
 * @see runtime::physical, \ref physical_cpp "physical::C++"
 * */


#ifndef   XML_FILENAME
#  define XML_FILENAME  file.xml
#endif

#include <xylose/XSTR.h>
#include <xylose/data_set.h>
#include <xylose/xml/physical_parse.h>

#include <physical/runtime.h>

#include <iostream>
#include <cstdlib>

using namespace runtime; /* strip runtime:: from physical::* */
typedef xylose::data_set<physical::Quantity,physical::Quantity> pqdata_set;

namespace xml = xylose::xml;

void prepareCalculator(const xml::Doc & doc) {
  /* prepare infix units calculator. */
  using physical::calc::Driver;
  using physical::calc::symbol;
  Driver & calc = Driver::instance();

  /* clear the old symbols out */
  calc.symbols.clear();
  calc.addMathLib();
  calc.addPhysicalUnits();

  xml::Context::list xl = doc.eval("//calc-commands/command");
  xml::Context::list::iterator i = xl.begin();
  for (; i != xl.end(); i++) {
    const xml::Context & x = (*i);
    calc.exec(x.parse<std::string>());
  }
}


void usage( const char * arg0, const char * xml_filename ) {
  std::cout
    << "testXML program of xylose package (taken from olson-tools).\n"
       "$Id$\n\n"
       "usage:  "
    << arg0 << " [options] [modifier]QUERY1 [[modifier]QUERY2] ...\n"
       "where the valid modifiers are q,d,r [default : q].\n"
       "The meaning of the modifiers is given by:\n\n"
       "\tq\tParse the query as a runtime::physical::Quanity item.\n"
        "\t\tIf there are multiple items returned by the query\n"
        "\t\teach will be parsed as a runtime::physical::Quantity.\n"
        "\t\tThis is the default when no option is given.\n"
       "\td\tParse the query as a xylose::data_set<A,B>\n"
        "\t\twhere A := B := runtime::physical::Quantity.\n"
        "\t\tIf there are multiple items returned by the query\n"
        "\t\teach will be parsed as a xylose::data_set<A,B>.\n"
       "\tr\tObtain the raw text of the query result.\n"
        "\t\tIf there are multiple items returned by the query\n"
        "\t\tthe text of each be shown.\n\n"
       "options:\n"
       "\t--help\tShow this help.\n\n"
       "Influential environment variables:\n"
       "\tXML_FILENAME\tSpecify an alternate xml file to query from\n"
       "\t\tCurrent XML_FILENAME:  " << xml_filename << "\n"
    << std::endl;
}/* usage */

static const std::string help_str = "--help";

enum query_type {
  QUANTITY,
  DATASET,
  RAW
};

void showResults( std::ostream & out, xml::Doc & db,
                  const enum query_type qt, const char * query ) {
  using namespace xylose::xml;
  using physical::Quantity;
  using xylose::data_set;
  //using xylose::convert_data_set;

  xml::Context::list xl = db.eval(query);
  xml::Context::list::iterator i = xl.begin();
  for (; i != xl.end(); ++i) {
    const xml::Context & x = (*i);
    switch (qt) {
      case RAW:
        out << x.text() << std::endl;
        break;

      case DATASET:
        try {
          enum Quantity::PRINT_TYPE old = Quantity::getPrintMode();
          Quantity::setPrintMode( Quantity::MATH_PRINT );
          std::cout << db.query<pqdata_set>(query) << std::endl;
          Quantity::setPrintMode( old );
        } catch (physical::exception & e) {
          std::cout << e.what() << std::endl;
        }
        break;

      default:
      case QUANTITY:
        try {
          std::cout << x.parse<Quantity>() << std::endl;
        } catch (physical::exception & e) {
          std::cout << x.text() << std::endl;
        }
        break;
    }/* switch */
  }/* for */
}/* showResults() */

int main(int argc, char **argv) {
  const char * xml_filename = getenv("XML_FILENAME");
  if ( !xml_filename )
    xml_filename = XSTR(XML_FILENAME);


  for (int i = 1; i < argc; i++) {
    if ( help_str == argv[i] ) {
      usage(argv[0], xml_filename);
      return EXIT_SUCCESS;
    }
  }

  xml::Doc db(xml_filename);
  prepareCalculator(db);


  /* Evaluate xpath expression */
  for (int i = 1; i < argc; ++i) {
    switch(argv[i][0]) {
      case 'd':
        showResults(std::cout, db, DATASET, argv[i]+1);
        break;
      case 'r':
        showResults(std::cout, db, RAW, argv[i]+1);
        break;
      case 'q':
        showResults(std::cout, db, QUANTITY, argv[i]+1);
        break;
      default:
        showResults(std::cout, db, QUANTITY, argv[i]);
        break;
    }
  }
  return 0;
}


