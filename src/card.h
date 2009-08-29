/// \file card.h Card class.
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

#ifndef CARD_H_
#define CARD_H_

#include <string>

/// Card of a package
class Card
{
public:
  /// Default constructor
  Card(){}
  /// Initializer
  void Init (int index,
             std::string symbol,
             std::string reading,
             std::string reading2,
             std::string translation,
             std::string example_symbol,
             std::string example_reading,
             std::string example_translation
            );

  // Observer methods
  /// Symbol/s of the card.
  std::string symbol () const;
  /// (First) reading of the symbol/s
  std::string reading () const;
  /// Second reading of the symbol
  std::string reading2 () const;
  /// Translation of the symbol/s
  std::string translation () const;
  /// Example of the symbol written with symbols
  std::string example_symbol () const;
  /// Reading of the example
  std::string example_reading () const;
  /// Translation of the example
  std::string example_translation () const;
  /// Index of the card in the package. Is in the range [1 - records]
  int index () const;

private:
  /// Index of the card in the package. Is in the range [1 - records]
  int index_;
  std::string symbol_,              ///< Symbol/s of the card.
              reading_,             ///< (First) reading of the symbol/s
              reading2_,            ///< Second reading of the symbol
              translation_,         ///< Translation of the symbol/s
              example_symbol_,      ///< Example of the symbol written with symbols
              example_reading_,     ///< Reading of the example
              example_translation_; ///< Translation of the example
};

#endif // CARD_H_
