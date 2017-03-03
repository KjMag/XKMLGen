#include "XkmlTreeView.h"

using namespace tln::docutils::gui;

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
		dropOK = false;
		break;
	case QAbstractItemView::OnViewport:
		dropOK = false;
		break;
	}
	if (dropOK)
	{
		
	}
	setDropIndicatorShown(false); 
	return;
}
