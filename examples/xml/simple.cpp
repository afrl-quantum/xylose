/** \file
 * Simple example of parsing XML documents using xml::Doc class.
 * */


#ifndef   XML_FILENAME
#  define XML_FILENAME  file.xml
#endif

#ifndef   XML_FILENAME
#  define XML_FILENAME  file-2.xml
#endif

#include <xylose/XSTR.h>
#include <xylose/xml/Doc.h>

#include <string>
#include <iostream>
#include <cstdlib>

namespace {
  namespace xml = xylose::xml;

  struct Struct {
    double x, y;
    std::string label;
    Struct() : x(0), y(0), label("") { }
  };

  /** XML parser for for Struct. */
  void parse_item( Struct & s, const xml::Context & x ) {
    s.x = x.query<double>("X");
    s.y = x.query<double>("Y");
    s.label = x.query< std::string >("Label");
  }

  /** output stream operator for Struct. */
  std::ostream & operator<< ( std::ostream & out, const Struct & s ) {
    out << "{ "
           "x:" << s.x << ", "
           "y:" << s.y << ", "
           "label:'" << s.label << "' }";
    return out;
  }
}


int main(int argc, char **argv) {
  const char * xml_filename = getenv("XML_FILENAME");
  if ( !xml_filename )
    xml_filename = XSTR(XML_FILENAME);

  xml::Doc doc(xml_filename);
  xml::Doc doc2(XSTR(XML_FILENAME2));

  /* very simple queries */
  std::cout
    << "thing[n0]:  " <<  doc.query< std::string >("//thing[@name='n0']") << "\n"
       "Double   :  " << doc2.query< double >("//Double") << "\n"
       "Int      :  " << doc2.query< int >("//Int") << "\n"
       "void*    :  " << doc2.query< void* >("//VoidP") << "\n"
       "Struct   :  " << doc2.query< Struct >("//Struct") << "\n"
    << std::endl;

  /* This is simple demonstration of modifiying the XML document */
  xml::Context stuff = doc.find("/Stuff");
  stuff.extend( doc2.find("/other") );
  stuff.extend( xml::Text("\n    ") );
  stuff.extend( doc2.eval("//thingies") );
  stuff.extend( xml::Text("\n") );
  stuff.extend( doc2.root_context );
  std::cout << "BEGIN OF XML DOCUMENT...\n";
  doc.save("-");
  std::cout << "...END OF XML DOCUMENT" << std::endl;

  return 0;
}


