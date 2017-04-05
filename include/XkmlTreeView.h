#pragma once
#include <qtreeview.h>

namespace Qt
{
	class XkmlTreeView :
		public QTreeView
	{
		Q_OBJECT
	public:
		XkmlTreeView(QWidget *parent = 0);
		virtual ~XkmlTreeView();
	protected:
		virtual void dragMoveEvent(QDragMoveEvent* event) Q_DECL_OVERRIDE;
		virtual void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;
	};

}

