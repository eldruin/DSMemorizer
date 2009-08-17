/// \file card.cpp Card class.
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

#include <string>
#include "card.h"

using std::string;

void Card::Init (int index, std::string symbol, std::string reading,
                 std::string reading2, std::string translation,
                 std::string example_symbol, std::string example_reading,
                 std::string example_translation
                )
{
  index_ = index;
  symbol_ = symbol;
  reading_ = reading;
  reading2_ = reading2;
  translation_ = translation;
  example_symbol_ = example_symbol;
  example_reading_ = example_reading;
  example_translation_ = example_translation;
}

std::string Card::symbol () const
{
  return symbol_;
}

std::string Card::reading () const
{
  return reading_;
}

std::string Card::reading2 () const
{
  return reading2_;
}

std::string Card::translation () const
{
  return translation_;
}

std::string Card::example_symbol () const
{
  return example_symbol_;
}

std::string Card::example_reading () const
{
  return example_reading_;
}

std::string Card::example_translation () const
{
  return example_translation_;
}

int Card::index () const
{
  return index_;
}

