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
 *   Main Developper : Eric MAEKER, MD <eric.maeker@gmail.com>             *
 *   Contributors :                                                        *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 *       NAME <MAIL@ADDRESS.COM>                                           *
 ***************************************************************************/
#ifndef MOLECULELINKERMODEL_H
#define MOLECULELINKERMODEL_H

#include <QAbstractTableModel>
#include <QMultiHash>
#include <QHash>
#include <QString>

namespace DrugsDB {
class DrugsDBCore;
namespace Internal {
class DrugBaseEssentials;
class MoleculeLinkData;
class MoleculeLinkerModelPrivate;
}  // namespace Internal

class MoleculeLinkerModel : public QAbstractTableModel
{
    Q_OBJECT
    friend class DrugsDB::DrugsDBCore;

protected:
    explicit MoleculeLinkerModel(QObject *parent = 0);

public:
    enum DataRepresentation {
        FancyButton = 0,
        MoleculeName,
        ATC_Code,
        Review,
        Reviewer,
        References,
        Comments,
        Date,
        AutoFoundAtcs,
        ColumnCount
    };
    ~MoleculeLinkerModel();
    bool initialize();

    QStringList availableDrugsDatabases() const;
    bool selectDatabase(const QString &dbUid);

    bool canFetchMore(const QModelIndex &parent = QModelIndex()) const;
    void fetchMore(const QModelIndex &parent = QModelIndex());

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    bool addUnreviewedMolecules(const QString &dbUid, const QStringList &molecules);
    bool addAutoFoundMolecules(const QMultiHash<QString, QString> &mol_atc, bool removeFromModel = false);

    bool moleculeLinker(Internal::MoleculeLinkData *data);

public Q_SLOTS:
    bool saveModel();
    void setActualReviewer(const QString &name);
    int removeUnreviewedMolecules();

private:
    static MoleculeLinkerModel *m_Instance;
    Internal::MoleculeLinkerModelPrivate *d;

};

}  //  End namespace DrugsDbCreator

#endif // MOLECULELINKERMODEL_H