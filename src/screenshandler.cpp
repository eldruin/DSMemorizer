/// \file screenshandler.cpp Screen handler
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

#include "textbox.h"
#include "textboxhandler.h"
#include "screenshandler.h"

void ScreensHandler::Init()
{
  tbh_ = new TextBoxHandler ();
  tbh_->Init ();
}

TextBoxHandler* ScreensHandler::tbh () const
{
  return tbh_;
}

ScreensHandler::~ScreensHandler ()
{
  tbh_->~TextBoxHandler();
  delete tbh_;
}
