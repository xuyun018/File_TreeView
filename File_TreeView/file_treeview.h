#ifndef FILE_TREEVIEW_H
#define FILE_TREEVIEW_H

#include <QTreeView>
// 鼠标右键菜单事件
#include <QContextMenuEvent>
#include "file_headerview.h"

class File_TreeView : public QTreeView
{
    Q_OBJECT

public:
    File_TreeView(QWidget *parent = nullptr);
    virtual ~File_TreeView()
    {

    };

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    std::unique_ptr<File_HeaderView> _header;
};

#endif // HOST_TREEVIEW_H
