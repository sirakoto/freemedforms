/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2010 by Eric MAEKER, MD (France) <eric.maeker@free.fr>        *
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
 *   Main Developper : Eric MAEKER, <eric.maeker@free.fr>                  *
 *   Contributors :                                                        *
 *       NAME <MAIL@ADRESS>                                                *
 *       NAME <MAIL@ADRESS>                                                *
 ***************************************************************************/
#ifndef PMHVIEWER_H
#define PMHVIEWER_H

#include <QWidget>

namespace PMH {
namespace Internal {
class PmhViewerPrivate;
class PmhData;
}  // End namespace Internal

class PmhViewer : public QWidget
{
    Q_OBJECT

public:
    enum EditMode {
        ReadOnlyMode,
        ReadWriteMode
    };

    PmhViewer(QWidget *parent = 0, EditMode editMode = ReadOnlyMode);
    ~PmhViewer();

    void setShowPatientInformations(bool show);
    void setEditMode(EditMode mode);
    void setPmhData(Internal::PmhData *pmh);
    void createNewPmh();
    void revert();

    Internal::PmhData *modifiedPmhData() const;

protected:
    void changeEvent(QEvent *e);

private:
    Internal::PmhViewerPrivate *d;
};

}  // End namespace PMH

#endif // PMHVIEWER_H
