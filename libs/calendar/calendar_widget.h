/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2011 by Eric MAEKER, MD (France) <eric.maeker@free.fr>        *
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
 *   Main Developpers :                                                    *
 *       Guillaume Denry <guillaume.denry@gmail.com>                       *
 *       Eric MAEKER, MD <eric.maeker@gmail.com>                           *
 *   Contributors :                                                        *
 *       NAME <MAIL@ADRESS>                                                *
 ***************************************************************************/
#ifndef CALENDAR_WIDGET_H
#define CALENDAR_WIDGET_H

#include <calendar/calendar_exporter.h>

#include <QScrollArea>
#include <QTimer>

#include "common.h"
#include "view.h"
#include "abstract_calendar_model.h"

class QVBoxLayout;

namespace Calendar {
struct CalendarWidgetPrivate;
class AbstractCalendarModel;

class CALENDAR_EXPORT CalendarWidget : public QWidget
{
    Q_OBJECT
	Q_PROPERTY(int dayGranularity READ dayGranularity WRITE setDayGranularity)

public:
    CalendarWidget(QWidget *parent = 0);

    AbstractCalendarModel *model() const { return m_model; }
    void setModel(AbstractCalendarModel *model);

    ViewType viewType() const;
    void setViewType(Calendar::ViewType viewType);

	/** returns the day granularity (see granularity in day_range_view.h) */
	int dayGranularity() const;
	/** set the day granularity (only dividers of 24*60 are allowed) */
	void setDayGranularity(int value);

private Q_SLOTS:
    // navigation bar slots
    void firstDateChanged();
    void viewTypeChanged();

    // timer
    void timeout();

private:
    CalendarWidgetPrivate *m_d;
    ViewType m_viewType;
    AbstractCalendarModel *m_model;
};

}  // End namespace Calendar

#endif
