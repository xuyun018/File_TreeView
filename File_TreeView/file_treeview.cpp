#include "file_treeview.h"
#include <QMenu>

File_TreeView::File_TreeView(QWidget *parent)
    : QTreeView(parent)
{
    _header = std::unique_ptr<File_HeaderView>(new File_HeaderView(Qt::Orientation::Horizontal, this));
    setHeader(_header.get());
}

void File_TreeView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu;

    menu.addAction("第一");
    menu.addAction("第二");
    menu.exec(event->globalPos());
}
