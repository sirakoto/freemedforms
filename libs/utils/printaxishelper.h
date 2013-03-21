/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2012 by Eric MAEKER, MD (France) <eric.maeker@gmail.com>      *
 *  All rights reserved.                                                   *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program (COPYING.FREEMEDFORMS file).                   *
 *  If not, see <http://www.gnu.org/licenses/>.                            *
 ***************************************************************************/
/***************************************************************************
 *   Main developers : Eric MAEKER, <eric.maeker@gmail.com>                *
 *   Contributors :                                                        *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 ***************************************************************************/
#ifndef UTILS_PRINTAXISHELPER_H
#define UTILS_PRINTAXISHELPER_H

#include <utils/global_exporter.h>
#include <QPoint>
#include <QSize>
#include <QRect>

/**
 * \file printaxishelper.h
 * \author Eric Maeker
 * \version 0.8.4
 * \date 21 Mar 2013
*/

namespace Utils {

class UTILS_EXPORT PrintAxisHelper
{
public:
    PrintAxisHelper();
    void setPageSize(const QRect &pageRect, const QSizeF &pageSizeInMillimeters);

    // Millimeters to pixels
    QPoint pointToPixels(double x_millimeter, double y_millimeter);
    QSize sizeToPixels(double width_millimeter, double height_millimeter);

private:
    double _pixToMmCoefX, _pixToMmCoefY;
};

} // namespace Utils

#endif // UTILS_PRINTAXISHELPER_H