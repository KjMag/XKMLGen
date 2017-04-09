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

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QXMLStreamWriter>

#include "treeitem.h"

namespace tln
{
	namespace docutils
	{
		namespace gui
		{
		class TreeModel : public QAbstractItemModel
		{
			Q_OBJECT

		public:
			TreeModel(const QStringList &headers, const QString &data = "", QObject *parent = 0);
			~TreeModel();

			QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
			QVariant headerData(int section, Qt::Orientation orientation,
				int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

			QModelIndex index(int row, int column,
				const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
			QModelIndexList indices(); // returns indices of all elements contained in the model
			QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

			int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
			int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
			
			Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
			Qt::DropActions supportedDropActions() const Q_DECL_OVERRIDE;
			bool setData(const QModelIndex &index, const QVariant &value,
				int role = Qt::EditRole) Q_DECL_OVERRIDE;
			bool setHeaderData(int section, Qt::Orientation orientation,
				const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

			bool insertColumns(int position, int columns,
				const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
			bool removeColumns(int position, int columns,
				const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
			bool insertRows(int position, int rows,
				const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
			bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
				const QModelIndex &destinationParent, int destinationChild) Q_DECL_OVERRIDE;
			bool insertAttributes(int position, int rows, const QModelIndex &parent = QModelIndex());
			bool insertElements(int position, int rows, const QModelIndex & parent);
			bool removeRows(int position, int rows,
				const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

			void setRootItem(TreeItem* root);
			void cutDownTree();

			// XML I/O utilities
			bool saveTreeViewAsXMLfile(const QString & filepath) const;
			bool writeTreeViewAsXML(QXmlStreamWriter & writer) const;
			bool loadXMLfileAsTreeView(const QString & filepath);
			bool writeXMLtoTreeView(QXmlStreamReader & reader);

		public slots:
			void on_ChangeOfNodeValueAttempted(const QVariant &var);

		private:
			// This function is intended to be used together with writeTreeViewAsXML() function as
			// it is assumed that before this function is called, QXMLStreamWriter::writeStartDocument()
			// function had already been called, and that the code invoking the function calls
			// XMLStreamWriter::writeEndDocument() afterwards:
			void writeTreeItemAsXML(TreeItem* const startItem, QXmlStreamWriter & writer) const;
			void writeElementAttributes(TreeItem * const item, QXmlStreamWriter & writer) const;
			// This function is intended to be used together with writeXMLtoTreeView() function as
			// it is assumed that before this function is called, QXmlStreamReader::readNext()
			// function had already been called once in order to parse the XML header:
			bool writeXmlToTreeItem(QXmlStreamReader & reader, TreeItem * const item, int position, const int columns);
			void loadAttributesToTreeItem(QXmlStreamReader & reader, TreeItem * const item, const int columns);
			void setupModelData(const QStringList &lines, TreeItem *parent);
			TreeItem *getItem(const QModelIndex &index) const;
			TreeItem *rootItem;
			QModelIndexList getItemIndices(TreeItem* item);
			TreeItem::TreeItemType type_of_item_about_to_be_inserted{ TreeItem::TreeItemType::ELEMENT };
		};
			//! [2]
		} // namespace gui
	} // namespace docutils
} // namespace tln

#endif // TREEMODEL_H
