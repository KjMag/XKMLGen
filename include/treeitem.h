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

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>
#include <QRegularExpressionValidator>

namespace tln
{
	namespace docutils
	{
		//! [0]
		class TreeItem : public QObject
		{
			Q_OBJECT

		signals:
			void changeOfNodeValueAttempted(const QVariant&);
			void attributeDataIncomplete();

		public:
			enum class TreeItemType
			{
				HEADER,
				NODE,
				ELEMENT,
				ATTRIBUTE
			};

			explicit TreeItem(const QVector<QVariant> &data, TreeItem *parent = 0, QObject *qparent = 0, TreeItemType type = TreeItemType::ELEMENT);
			~TreeItem();

			TreeItem *treeItemChild(int number); // child in the tree data structure sense, not in XML/JSON sense, i.e. elements and attributes combined
			TreeItem *element(const int number);
			TreeItem *attribute(const int number); 
			int treeItemChildCount() const;
			int columnCount() const;
			int elementCount() const;
			int attributeCount() const;
			QVariant data(int column) const;
			// by "children" child tree items in a view are meant, not children in the XML sense 
			// (e.g. XML attributes are also child tree items):
			bool insertElements(int position, int count, int columns);
			bool insertAttributes(int position, int count, int columns); 
			bool insertColumns(int position, int columns);
			TreeItem *parent();
			bool removeTreeItemChildren(int position, int count);
			bool removeColumns(int position, int columns);
			int treeItemChildNumber() const;
			bool setData(int column, const QVariant &value);
			TreeItemType type() const { return itemType; }

			static const int elementNameColumn{ 0 };
			static const int elementValueColumn{ 1 };
			static const int attributeNameColumn{ 2 };
			static const int attributeValueColumn{ 3 };

		private:
			void populateColumnsWithDefaultValues();

			static const QString default_header_element_name_string;
			static const QString default_header_element_value_string;
			static const QString default_header_attribute_name_string;
			static const QString default_header_attribute_value_string;
			static const QString default_element_name_string;
			static const QString default_element_value_string;
			static const QString attribute_element_name_string;
			static const QString attribute_element_value_string;

			static const QString forbidden_tag_name_characters;

			QRegularExpressionValidator tag_name_validator;
			QList<TreeItem*> childElements;
			QList<TreeItem*> attributeItems;
			QVector<QVariant> itemData; 
			TreeItem *parentItem;
			TreeItemType itemType;
			
		};
		//! [0]
	} // namespace docutils
} //namespace tln

#endif // TREEITEM_H
