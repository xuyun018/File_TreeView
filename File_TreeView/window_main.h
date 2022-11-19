#ifndef WINDOW_MAIN_H
#define WINDOW_MAIN_H

#include <QMainWindow>
#include <QPoint>

#include "file_treeview.h"
#include "file_item.h"
#include "file_itemmodel.h"
#include "file_itemdelegate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Window_Main; }
QT_END_NAMESPACE

class File_Item;
class File_ItemModel;
class File_ItemDelegate;
class Host_SortFilterProxyModel;
class File_HeaderView;
class File_TreeView;
class Window_Main : public QMainWindow
{
    Q_OBJECT

public:
    Window_Main(QWidget *parent = nullptr);
    ~Window_Main();

private slots:
    // 自己定义的信号槽, 也可以让 TreeView 支持右键菜单
    void on_context_menu(const QPoint &pt);

    // 系统信号槽
    void on_pushButton_clicked();

private:
    Ui::Window_Main *ui;

    File_Item *selected = nullptr;
    File_ItemModel *mModel = nullptr;;
    File_ItemDelegate *mDelegate = nullptr;
    File_ItemDelegate *mFilterDelegate = nullptr;
    Host_SortFilterProxyModel *mFilterModel = nullptr;
    File_HeaderView *mHeader;

    void DeletePtr();
};
#endif // WINDOW_MAIN_H
