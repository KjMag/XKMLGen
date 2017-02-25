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

/*
    treeitem.cpp

    A container for items of data supplied by the simple tree model.
*/

#include "treeitem.h"

#include <QStringList>
#include <QMessageBox>

using namespace tln::docutils;

//! [0]

const QString TreeItem::attribute_uneditable_column_string = "*attribute*";

TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent, QObject *qparent, TreeItemType type)
	: QObject(qparent)
	, itemType(type)
	, forbidden_tag_name_characters(R"(^:\-\+\.\,!@#$%&*;~`"'<\^\(\)\\/\|\?\[\]\{\} )")
	, tag_name_validator(QRegularExpression("[^0-9" + forbidden_tag_name_characters + "][" + forbidden_tag_name_characters + "]*"))
{
	parentItem = parent;
    itemData = data;

	return;
}
//! [0]

//! [1]
TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
	qDeleteAll(attributeItems);
}
//! [1]

//! [2]
TreeItem *TreeItem::child(int number)
{
	if (number >= attributeCount())
		return childItems.value(number - attributeCount());
	else
		return attributeItems.value(number);
}

TreeItem * TreeItem::element(const int number)
{
	if (number < 0 || number >= childItems.size())
		return nullptr;

	return childItems.value(number);
}

TreeItem * TreeItem::attribute(const int number)
{
	if (number < 0 || number > attributeItems.size())
		return nullptr;
	return attributeItems.value(number);
}
//! [2]

//! [3]
int TreeItem::childCount() const
{
    return childItems.count();
}
//! [3]

//! [4]
int TreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}
//! [4]

//! [5]
int TreeItem::columnCount() const
{
    return itemData.count();
}
int TreeItem::attributeCount() const
{
	return attributeItems.count();
}
//! [5]

//! [6]
QVariant TreeItem::data(int column) const
{
    return itemData.value(column);
}
//! [6]

//! [7]
bool TreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > (childItems.size() + attributeItems.size()) || count <= 0 || itemType == TreeItemType::ATTRIBUTE)
        return false;

	if (childItems.size() == 0 && itemType != TreeItemType::HEADER)
		this->setData(elementValueColumn, QVariant(""));

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        TreeItem *item = new TreeItem(data, this, this);
        childItems.insert(position, item);
    }

    return true;
}
//! [7]

bool TreeItem::insertAttributes(int position, int count, int columns)
{
	if (position < 0 || position > attributeItems.size() || count <= 0 || itemType == TreeItemType::ATTRIBUTE)
		return false;

	for (int row = 0; row < count; ++row) 
	{
		QVector<QVariant> data(columns);

		TreeItem *item = nullptr;
		//if (attributeItems.size() != 0)
		//{
			item = new TreeItem(data, this, this, TreeItemType::ATTRIBUTE);
		//}
		//else
		//{
			//item = new TreeItem(data, nullptr, this, TreeItemType::ATTRIBUTE);
			//this->setData(attributeNameColumn, item->data(attributeNameColumn));
			//this->setData(attributeValueColumn, item->data(attributeValueColumn));
		//}
		attributeItems.insert(position, item);
	}

	return true;
}

//! [8]
bool TreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    foreach (TreeItem *child, childItems)
        child->insertColumns(position, columns);

    return true;
}
//! [8]

//! [9]
TreeItem *TreeItem::parent()
{
    return parentItem;
}
//! [9]

//! [10]
bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}
//! [10]

bool TreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    foreach (TreeItem *child, childItems)
        child->removeColumns(position, columns);

    return true;
}

//! [11]
bool TreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

	if (childCount() == 0 || column != 1)
	{
		if (column == 0)
		{
			int pos = 0;
			QValidator::State state = tag_name_validator.validate(value.toString(), pos);
			if (state == QValidator::Acceptable)
				itemData[column] = value;
			else
			{
				QMessageBox::information(nullptr, "XKMLGen", tr("Incorrect tag name."));
				return false;
			}
		}
		else
		{
			itemData[column] = value;
		}
	}
	else 
	{
		emit changeOfNodeValueAttempted(itemData[0]);
		return false;
	}

	return true;
}
//! [11]
