//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>

/**
 * @brief Model data for a node in the \link TreeModel tree model\endlink
 *
 * The class is mostly virtual : except for the root item, all items are
 * instances of TreeObjectItem.
 *
 * The item write the data in html and then relies on the \link HTMLDelegate
 * HTML delegate\endlink to display it.
 */
class TreeItem : public QObject
{
    Q_OBJECT
public:

    static TreeItem* RootItem(const QList<QVariant> &data, QObject *owner);
    virtual ~TreeItem() {}

    bool removeChildren();
    bool removeChildren(int position, int count);


    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;

    int row() const;
    TreeItem *parent();
    bool hasChildren() const;

    void load() const;
    virtual bool synchronised();
    bool synchronising() const;
    void setSynchronising(bool value);

    virtual QVariant clipboardValue() const;
    virtual bool hasStream() const;
    virtual bool hasLinkTo() const;
    virtual qint64 linkTo() const;


protected:
    TreeItem(const QList<QVariant> &data, TreeItem *parent);
    QList<QVariant>& itemData() const;
    virtual void doLoad() const;

    virtual void onChildrenRemoved();



signals:
    bool childrenRemoved();

private:
    TreeItem(const QList<QVariant> &data, TreeItem *parent, QObject *owner);
    void appendChild(TreeItem *child);

    QList<TreeItem*> _childItems;
    TreeItem* _parentItem;
    mutable QList<QVariant> _itemData;
    mutable bool _loaded;
    bool _synchronising;
};

#endif // TREEITEM_H
