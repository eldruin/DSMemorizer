/// \file xmlparser.h XML files parser.
/* Copyright 2009 Diego Barrios Romero
 *
 * This file is part of DSMemorizer.
 *
 * DSMemorizer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DSMemorizer distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DSMemorizer.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef XMLPARSER_H_
#define XMLPARSER_H_

class Card;

/// XML file parsing class
class XMLParser
{
public:
  /// Default constructor
	XMLParser(){}
	/// Initializer
	/// \param filepath Path of the file to read
	void Init(const std::string& filepath);

	/// Get a card determined by its index
	Card card(int index);

	/// Get the number of records in the package
	int package_records () const;

  ~XMLParser ();
private:
  // Data members
  /// XML file
	FILE* file_;
	/// XML file version
	std::string xml_version_;
	/// Name of the package in the file (only one package per file is supported)
	std::string package_name_;
	/// Number of cards in the package
	int package_records_;
	/// Format of the package
	std::string package_format_;
	/// License of the package
	std::string package_license_;

	/// Temporal reading buffer size
  static const int BUFFER_SIZE = 512;

  // Functions
  /// Simple equality string comparing function
	inline bool compare (const char* s1, const char* s2);
	/// Simple finding function
	inline bool find(const char* s, const char* buffer, int& position);

	/// Gives the value of an attribute in a XML formatted line
	/// \param name Name of the attribute
	/// \param buffer XML line to search in
	/// \param position Position to start searching. It's updated to the last
	/// read character position
	std::string attribute_value(const char* name, const char* buffer,
                              int& position);
};

inline bool XMLParser::compare (const char* s1, const char* s2)
{
  bool equal = false;
  if (s1 && s2)
  {
    size_t i = 0;
    for (;s1[i] == s2[i] && s1[i] != '\0' && s2 != '\0'; ++i);
    if (s1[i] == '\0')
      equal = true;
  }
  return equal;
}

inline bool XMLParser::find(const char* s, const char* buffer, int& position)
{
  bool found = false;
  int i = position;
  for (; buffer[i] != '\0' && !found && i < BUFFER_SIZE ; ++i)
  {
    if (compare (s, buffer+i))
      found = true;
  }
  if (!found) // not found in [position, end] range. maybe in [0, position).
  {
    i = 0;
    for (; buffer[i] != '\0' && !found && i < position ; ++i)
    {
      if (compare (s, buffer+i))
        found = true;
    }
  }

  if (found)
    position = i - 1 ; // start of the string s in buff

  return found;
}

#endif // XMLPARSER_H_

