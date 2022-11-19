#ifndef FILE_ITEMMODEL_H
#define FILE_ITEMMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QSortFilterProxyModel>
#include <QSize>

#include "file_item.h"

class File_ItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit File_ItemModel(QStringList headers, QObject *parent = nullptr);
    ~File_ItemModel(void);

protected:
    //以下为自定义model需要实现的一些虚函数，将会被Qt在查询model数据时调用
    //headerData: 获取表头第section列的数据
    //data: 核心函数，获取某个索引index的元素的各种数据
    //      role决定获取哪种数据，常用有下面几种：
    //      DisplayRole（默认）：就是界面显示的文本数据
    //      TextAlignmentRole：就是元素的文本对齐属性
    //      TextColorRole、BackgroundRole：分别指文本颜色、单元格背景色
    //flags: 获取index的一些标志，一般不怎么改
    //index: Qt向你的model请求一个索引为parent的节点下面的row行column列子节点的元素，在本函数里你需要返回该元素的正确索引
    //parent：获取指定元素的父元素
    //rowCount: 获取指定元素的子节点个数（下一级行数）
    //columnCount: 获取指定元素的列数
    //setData: 修改节点内容
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

public:
    File_Item *itemFromIndex(const QModelIndex &index) const;
    QModelIndex indexFromItem(File_Item *item);
    File_Item *root(void);

private:
    // 表头内容
    QStringList _headers;
    // 根结点
    File_Item *_item_root;
    QSize ss;
};

class Host_SortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    Host_SortFilterProxyModel(QObject *parent = nullptr);
    virtual ~Host_SortFilterProxyModel(void);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const override;
};


#endif // HOST_TREEMODEL_H
