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
	XMLParser();
	/// Initializer
	/// \param file_path Path of the file to read
	void Init(const std::string& file_path);

	/// Get a card determined by its index
	Card card(unsigned index, unsigned grade_min = 0, unsigned grade_max = 0,
            unsigned strokes_min = 0, unsigned strokes_max = 0);

	/// Get the number of records in the package
	int package_records () const;

	int QueryResultSize (unsigned grade_min, unsigned grade_max,
                       unsigned strokes_min, unsigned strokes_max);

  ~XMLParser ();
private:
  enum mode
  {
    XML_KANJI,
    XML_VOCABULARY
  };
  // Data members
  /// XML file
	FILE* file_;
	/// File path
	std::string file_path_;
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
	/// File format
	mode file_format_;

  /// File cards positions vector
  fpos_t* file_cursor_;
  /// Vector of cards numbers organized by grade
  std::vector< int >* grade_;
  /// Vector of cards numbers organized by strokes
  std::vector< int >* strokes_;

  /// Vector of the results of the query in the database for the selected
  /// grades and strokes range
  std::vector<int> query_result_;

  unsigned q_grade_min, q_grade_max, q_strokes_min, q_strokes_max;

	/// Temporal reading buffer size
  static const int BUFFER_SIZE = 512;

  // Functions
  /// Simple equality string comparing function
	inline bool compare (const char* s1, const char* s2);
	/// Simple finding function
	inline bool find(const char* s, const char* buffer, int& position);
	/// Fill the indexes file_cursor_ , grade_ and strokes_
	void GenerateIndexes();
  /// Gets the package card index acording to the grade and strokes if given.
  /// \param index Number of the card to retrieve regarding given grade and
  /// strokes
  /// \param grade_min Minimum grade of the kanji to retrieve.
  /// \param grade_max Maximum grade of the kanji to retrieve.
  /// \param strokes_min Minimum number of strokes of the kanji to retrieve.
  /// \param strokes_max Maximum number of strokes of the kanji to retrieve.
  /// \return A number [1 - package_records_] if the kanji could be found.
  /// \return 0 if wasn't found any kanji regarding the parameters.
	unsigned GetIndex(unsigned index, unsigned grade_min, unsigned grade_max,
                    unsigned strokes_min, unsigned strokes_max);
  /// Generate the result vector for the given grades and strokes ranges
  void QueryResult (unsigned grade_min, unsigned grade_max,
                    unsigned strokes_min, unsigned strokes_max);

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
