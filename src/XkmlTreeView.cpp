#include "XkmlTreeView.h"
#include "treeitem.h"
#include "treemodel.h"
#include <QDropEvent>
#include <qmimedata.h>

//using namespace tln::docutils::gui;
//using namespace Qt;

XkmlTreeView::XkmlTreeView(QWidget *parent) :
	QTreeView(parent)
{
}


XkmlTreeView::~XkmlTreeView()
{
}

void XkmlTreeView::dragMoveEvent(QDragMoveEvent* event)
{
	setDropIndicatorShown(true);
	QTreeView::dragMoveEvent(event);
	return;
}

void XkmlTreeView::dropEvent(QDropEvent* event)
{
	bool dropOK = false;
	DropIndicatorPosition dropIndicator = dropIndicatorPosition();

	switch (dropIndicator)
	{
	case QAbstractItemView::AboveItem:
		dropOK = true;
		break;
	case QAbstractItemView::BelowItem:
		dropOK = true;
		break;
	case QAbstractItemView::OnItem:
		dropOK = true;
		break;
	case QAbstractItemView::OnViewport:
		dropOK = true;
		break;
	}
	if (dropOK)
	{
		using namespace tln::docutils::gui;
		using namespace tln::docutils;
		QModelIndex index = indexAt(event->pos());
		if (!index.isValid())
			return;
		QAbstractItemModel *model = this->model();
		TreeModel* tree = static_cast<TreeModel*>(model);
		//tree->moveRows(, , 1, index, event->pos());
		const QMimeData *mime = event->mimeData();
		QString text = mime->text();
	}
	setDropIndicatorShown(false); 
	return;
}
