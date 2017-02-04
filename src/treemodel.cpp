/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QXMLStreamReader>

#include "treeitem.h"
#include "treemodel.h"


using namespace tln::docutils::gui;
using tln::docutils::TreeItem;

//! [0]
TreeModel::TreeModel(const QStringList &headers, const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    rootItem = new TreeItem(rootData, nullptr, nullptr, TreeItem::TreeItemType::HEADER);
    setupModelData(data.split(QString("\n")), rootItem);
}
//! [0]

//! [1]
TreeModel::~TreeModel()
{
    delete rootItem;
}

void TreeModel::setRootItem(TreeItem * root)
{
	TreeItem* tmp = rootItem; 
	emit beginResetModel();
	rootItem = root; 
	emit endResetModel();
	delete tmp;

	return;
}
//! [1]

//! [2]
int TreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}
//! [2]

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem *item = getItem(index);

    return item->data(index.column());
}

//! [3]
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}
//! [3]

//! [4]
TreeItem* TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}
//! [4]

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

//! [5]
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();
//! [5]

//! [6]
    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
//! [6]

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success;
	int children_pre = parentItem->childCount();
    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

	if (success && (children_pre == 0))
		connect(parentItem, &TreeItem::changeOfNodeValueAttempted, this, &TreeModel::on_ChangeOfNodeValueAttempted);

    return success;
}

//! [7]
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}
//! [7]

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

//! [8]
int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}
//! [8]

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            ++position;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
					connect(parents.last(), &TreeItem::changeOfNodeValueAttempted, this, &TreeModel::on_ChangeOfNodeValueAttempted);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            TreeItem *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
			for (int column = 0; column < columnData.size(); ++column)
				parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }

        ++number;
    }
}

void TreeModel::cutDownTree()
{
	if (rootItem == nullptr)
		return;
	beginResetModel();
	rootItem->removeChildren(0, rootItem->childCount());
	endResetModel();

	return;
}

void TreeModel::on_ChangeOfNodeValueAttempted(const QVariant &var)
{
	QMessageBox msgBox;
	msgBox.setText("The entry: \"" + var.toString() + "\" is a node. Nodes can't be assigned values."
		" Remove its children elements in order to assign value to this entry.");
	msgBox.exec();
	return;
}

bool TreeModel::saveTreeViewAsXMLfile(const QString& filepath) const
{
	if (filepath == "")
		return false;
	QFile file(filepath);
	bool success = file.open(QIODevice::WriteOnly);
	if (!success)
		return false;

	QXmlStreamWriter xml_writer(&file);
	xml_writer.setAutoFormatting(true);
	writeTreeViewAsXML(xml_writer);

	file.close();

	return !xml_writer.hasError();
}

bool TreeModel::writeTreeViewAsXML(QXmlStreamWriter& writer) const
{
	writer.writeStartDocument();

	int childCount = rootItem->childCount();
	for (int i = 0; i < childCount; ++i)
	{
		TreeItem* item = rootItem->child(i);
		writeTreeItemAsXML(item, writer);
	}

	writer.writeEndDocument();

	return true;
}

bool TreeModel::loadXMLfileAsTreeView(const QString & filepath)
{
	if (filepath == "")
		return false;
	QFile file(filepath);
	if (!file.open(QIODevice::ReadOnly | QFile::Text))
		return false;
	QXmlStreamReader xml_reader(&file);
	 
	return writeXMLtoTreeView(xml_reader);
}

bool TreeModel::writeXMLtoTreeView(QXmlStreamReader & reader)
{
	reader.readNext();
	if (!reader.isStartDocument())
	{
		QMessageBox::StandardButton answer = QMessageBox::question(nullptr, tr("XML header missing"),
			tr("XML header missing. If you are sure that the file contains otherwise well-formed"
				" XML 1.0 content, you may try to load it anyway.\n"
				"Are you sure that you want to load this file?"),
			QMessageBox::Yes | QMessageBox::No);

		if (answer == QMessageBox::No)
		{
			return false;
		}
	}
	if (reader.isStartDocument() && reader.documentVersion() != "1.0")
	{
		QMessageBox::StandardButton answer = QMessageBox::question(nullptr, tr("Wrong XML version"),
			tr("This program supports XML ver. 1.0 only. The version of the file you are"
				" trying to load is different, which means that some content may not be"
				" parsed correctly. Do you want to proceed?"),
			QMessageBox::Yes | QMessageBox::No);
			
		if (answer == QMessageBox::No)
		{
			return false;
		}
	}

	QStringList headers;
	headers << tr("Element/Node name") << tr("Element value") << tr("Attribute name") << tr("Attribute value");
	QVector<QVariant> rootData;
	foreach(QString header, headers)
		rootData << header;

	TreeItem* root = new TreeItem(rootData, nullptr, nullptr, TreeItem::TreeItemType::HEADER);
	int position = 0;
	bool success = writeXmlToTreeItem(reader, root, position, rootData.size());
	if (success)
		setRootItem(root);
	else
	{
		delete root;
		QMessageBox::critical
		(
			nullptr,
			tr("Failed to load XML"),
			tr("The following error has been reported while trying\n"
				"to load XML document:\n") + reader.errorString()
		);
	}

	return success;
}

bool TreeModel::writeXmlToTreeItem(QXmlStreamReader & reader, TreeItem* const item, int position, const int columns)
{
	while (!reader.atEnd())
	{
		reader.readNext();
		QXmlStreamReader::TokenType tokenType = reader.tokenType();
		switch (tokenType)
		{
		case QXmlStreamReader::StartElement:
			item->insertChildren(position, 1, columns);
			item->child(position)->setData(0, QVariant(reader.name().toString()));
			if (!writeXmlToTreeItem(reader, item->child(position), 0, columns))
			{
				QString error = reader.errorString();
				return false;
			}
			++position;
			break;
		case QXmlStreamReader::EndElement:
			goto function_exit;
		case QXmlStreamReader::EndDocument:
			goto function_exit;
		case QXmlStreamReader::Characters:
			if (!reader.isWhitespace())
			{
				item->setData(1, QVariant(reader.text().toString()));
				reader.readNext();
				goto function_exit;
			}
			break;
		case QXmlStreamReader::Comment:
			break;
		default:
			if (!reader.hasError())
				reader.raiseError("Unexpected XML element has occurred. The XML could not be processed.");
			goto function_exit;
		}
	}
	function_exit:
	return !reader.hasError();
}

void TreeModel::writeTreeItemAsXML(TreeItem* const startItem, QXmlStreamWriter & writer) const
{
	if (startItem == nullptr)
		return;

	int childCount = startItem->childCount();
	if (childCount == 0)
		writer.writeTextElement(startItem->data(0).toString(), startItem->data(1).toString());
	else
	{
		writer.writeStartElement(startItem->data(0).toString());
		for (int i = 0; i < childCount; ++i)
		{
			TreeItem* const item = startItem->child(i);
			writeTreeItemAsXML(item, writer);
		}
		writer.writeEndElement();
	}

	return;
}