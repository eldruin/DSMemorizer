/**
 * \mainpage
 * \section intro Introduction
 * DSMemorizer is a homebrew game for the Nintendo DS intended to help
 * memorizing ideograms and vocabulary, focused on japanese language.
 *
 * DSMemorizer has several game modes dedicated either to memorization
 * or to ask about memorized things.
 *
 * The idea is to play this game while you are on a travel or any
 * other place where you must wait and lose your time. This way you
 * can learn and take advantage of that time.
 *
 * It's programmed using UTF-8 and vectorial font rendering through
 * FreeType library so it can be adapted to any other language.
 *
 * Packages of words or ideograms can be written easily with a plain
 * text editor.
 *
 * \section dev-notes Developer's notes
 *
 * DSMemorizer has this structure:
 *
 * There is a game main class, which is \c DSMemorizer. This class
 * has several private methods which are the game modes funcions.
 *
 * The main class is which has a handler for both screens,
 * textboxes and XML files.
 *
 * Each mode changes the screens modes when they are initiated and all
 * the necessary textboxes are created in their places. Also point the
 * XML files parser to the right file to read the data.
 *
 * Each mode has a game loop so the game stays running inside the
 * functions until the user chooses to return to the main menu.
 *
 * Visit the wiki in the website for more.
 *
 * \section contact Contact
 * \par Website:
 * http://gitorious.org/dsmemorizer
 *
 * \author Diego Barrios Romero
 * eldruin@gmail.com
 *
 * \par License:
 * Copyright 2009 Diego Barrios Romero.
 * \par
 * DSMemorizer is free software under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * A copy of this license is included in the COPYING file.
 */
