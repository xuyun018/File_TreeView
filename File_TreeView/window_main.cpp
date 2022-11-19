#include "window_main.h"
#include "ui_window_main.h"

#include <QIcon>
#include <QPixmap>
#include <QFile>
#include <QDebug>

#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>

bool fileitemLessThan(File_Item *fi1, File_Item *fi2)
{
    int l1;
    int l2;
    int result = 0;

    l1 = (fi1->Attributes() & FILE_ATTRIBUTE_DIRECTORY) != 0;
    l2 = (fi2->Attributes() & FILE_ATTRIBUTE_DIRECTORY) != 0;
    result = l2 - l1;
    if (result == 0)
    {
        result = QString::compare((*fi1->ItemData())[0].toString(), (*fi2->ItemData())[0].toString(), Qt::CaseInsensitive);
    }
    return(result < 0);
}

unsigned int buil_tree(File_Item *parent,
    uint16_t *filename, unsigned int filenamelength, unsigned int filenamesize,
    WIN32_FIND_DATA *pfd)
{
    QList<File_Item *> *children;
    File_Item *item;
    HANDLE hfind;
    unsigned int l;
    unsigned int result = 0;
    uint16_t uch;

    if (filenamelength > 0 && filename[filenamelength - 1] != '\\')
    {
        filename[filenamelength++] = '\\';
    }
    filename[filenamelength + 0] = '*';
    filename[filenamelength + 1] = '.';
    filename[filenamelength + 2] = '*';
    filename[filenamelength + 3] = '\0';
    hfind = FindFirstFile((const WCHAR *)filename, pfd);
    if (hfind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (pfd->cFileName[0] != '.' || (pfd->cFileName[1] != '\0' &&
                (pfd->cFileName[1] != '.' || pfd->cFileName[2] != '\0')))
            {
                l = wcslen(pfd->cFileName);

                if (filenamelength + l < filenamesize)
                {
                    wcscpy((WCHAR *)filename + filenamelength, pfd->cFileName);

                    unsigned int imageindex;
                    unsigned int selectedindex;
                    SHFILEINFO shfi;
                    SHGetFileInfo((WCHAR *)filename, pfd->dwFileAttributes, &shfi, sizeof(shfi), SHGFI_SYSICONINDEX);
                    imageindex = shfi.iIcon;
                    selectedindex = imageindex;
                    if (pfd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        SHGetFileInfo((WCHAR *)filename, pfd->dwFileAttributes, &shfi, sizeof(shfi), SHGFI_OPENICON | SHGFI_SYSICONINDEX);

                        selectedindex = shfi.iIcon;
                    }

                    item = new File_Item(parent);
                    QVector<QVariant> data1;
                    data1 << QString::fromUtf16((const char16_t *)filename + filenamelength) << "api" << "firstapiapiapiapiapiapiapi" << "";
                    item->setItemData(data1);
                    item->setAttributes(pfd->dwFileAttributes);
                    item->setImageIndex(imageindex);
                    parent->appendChild(item);

                    if (pfd->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
                    {
                        if (filenamelength + l + 5 < filenamesize)
                        {
                            result += buil_tree(item, filename, filenamelength + l, filenamesize, pfd);
                        }
                    }
                }
            }
        } while (FindNextFile(hfind, pfd));
        FindClose(hfind);
    }

    if (children = parent->Children())
    {
        qSort(children->begin(), children->end(), fileitemLessThan);
    }

    return(result);
}

Window_Main::Window_Main(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Window_Main)
{
    ui->setupUi(this);

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &Window_Main::on_context_menu);
}

Window_Main::~Window_Main()
{
    DeletePtr();

    delete ui;
}

// 自定义信号槽
void Window_Main::on_context_menu(const QPoint &pt)
{
    QMenu menu;

    menu.addAction("第一");
    menu.addAction("第二");
    menu.exec(ui->treeView->mapToGlobal(pt));
}

void Window_Main::on_pushButton_clicked()
{
    File_Item *item_root;

    DeletePtr();
    QStringList headList;
    headList << "名称" << "修改日期" << "类型" << "大小";
    mModel = new File_ItemModel(headList, ui->treeView);
    item_root = mModel->root();

    uint16_t filename[1024];
    unsigned int filenamelength;
    WIN32_FIND_DATA wfd;

    wcscpy((WCHAR *)filename, L"C:");
    filenamelength = wcslen((WCHAR *)filename);
    buil_tree(item_root,
        filename, filenamelength, sizeof(filename) / sizeof(filename[0]),
        &wfd);

    // ui->treeView
    mFilterModel = new Host_SortFilterProxyModel(nullptr);
    mFilterModel->setSourceModel(mModel);
    mFilterModel->setFilterKeyColumn(1);
    ui->treeView->setModel(mFilterModel);
    ui->treeView->setModel(mModel);
    //mDelegate = new File_ItemDelegate(ui->treeView);
    mFilterDelegate = new File_ItemDelegate(ui->treeView);
    //ui->treeView->setItemDelegate(mDelegate);
    ui->treeView->setItemDelegate(mFilterDelegate);
    ui->treeView->setWordWrap(true);
}

void Window_Main::DeletePtr()
{
    if (mModel != nullptr) {
        delete mModel;
        mModel = nullptr;
    }
    if (mDelegate != nullptr) {
        delete mDelegate;
        mDelegate = nullptr;
    }
    if (mFilterDelegate != nullptr) {
        delete mFilterDelegate;
        mFilterDelegate = nullptr;
    }
    if (mFilterModel != nullptr) {
        delete mFilterModel;
        mFilterModel = nullptr;
    }
}
