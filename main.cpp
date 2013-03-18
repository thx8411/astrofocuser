/*
 * copyright (c) 2012 Blaise-Florentin Collin
 *
 * This file is part of astrofocuser.
 *
 * astrofocuser is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License v3
 * as published by the Free Software Foundation
 *
 * astrofocuser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * v3 along with astrofocuser; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#include <iostream>

#include <QtGui>

#include "qfwhm.hpp"

// app information
#define ASTROFOCUSER_NAME	"AstroFocuser"
#define ASTROFOCUSER_VERSION	"v1.1.1"
#define ASTROFOCUSER_BUILD	3
#define ASTROFOCUSER_COPYRIGHT	"(c) 2013 Blaise-Florentin Collin, GNU GPLv3"

int main(int argc, char *argv[])
{
   // software informations
   std::cout << ASTROFOCUSER_NAME << " " << ASTROFOCUSER_VERSION << " build " << ASTROFOCUSER_BUILD << std::endl;
   std::cout << ASTROFOCUSER_COPYRIGHT << std::endl;

   // app and object build
   QApplication app(argc, argv);
   QFwhm window;
   window.show();

   // app loop
   return app.exec();
}
