#include "file_itemmodel.h"

File_ItemModel::File_ItemModel(QStringList headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    _headers = headers;
    _item_root = new File_Item;
    ss = QSize(28, 38);
}
File_ItemModel::~File_ItemModel(void)
{
    delete _item_root;
}

QVariant File_ItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
        {
            return _headers.at(section);
        }
    }
    return QVariant();
}

QVariant File_ItemModel::data(const QModelIndex &index, int role) const
{
    File_Item *item;

    if (!index.isValid()) {
        return QVariant();
    }
    item = static_cast<File_Item *>(index.internalPointer());
    if (role == Qt::UserRole) {
        return QVariant::fromValue<File_Item *>(item);
    } else
    if (Qt::SizeHintRole == role)
    {
        return QVariant(ss);
    }
    //TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    else if (role == Qt::DisplayRole) {
        return item->data(index.column());
    }
    else if (role == Qt::EditRole)
    {
        //元素打开编辑框时，显示的内容，如果没有这行，编辑框出现时默认为空白
        return item->data(index.column());
    }
    //else if (role == Qt::TextAlignmentRole)
    //{
    //    if (index.column() == 0)
    //    {
    //        //第1列左对齐
    //        return QVariant(Qt::AlignVCenter | Qt::AlignLeft);
    //    }
    //    else if (index.column() > 0 && index.column() < 6)
    //    {
    //        //第2~6列右对齐
    //        return QVariant(Qt::AlignVCenter | Qt::AlignRight);
    //    }
    //    else
    //    {
    //        //其他列居中对齐
    //        return QVariant(Qt::AlignCenter);
    //    }
    //}
    //else if (role == Qt::TextColorRole)
    //{
    //    if (index.column() == 6)
    //    {
    //        QString str = item->data(6).toString();
    //        if (str == QStringLiteral("不合格"))
    //        {
    //            //第7列（是否合格）如果不合格就设文本为红色
    //            return QVariant(QColor("#FF0000"));
    //        }
    //    }
    //    return QVariant();
    //}
    return QVariant();
}

bool File_ItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    File_Item *item;

    //TreeModel::setData 是重写的父类函数，是在编辑完后会被自动调用的函数
    //TreeItem::setData 是我自己写的函数，来真正修改数据
    if (index.isValid() && role == Qt::EditRole)
    {
        item = static_cast<File_Item *>(index.internalPointer());
        item->setData(index.column(), value);
        emit dataChanged(index, index);
        return true;
    }/* else if (index.isValid() && role == Qt::SizeHintRole) {
        ss = value.toSize();
        emit dataChanged(index, index);
        return true;
    }*/
    return false;
}

Qt::ItemFlags File_ItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags item_flags;

    if (!index.isValid())
        return 0;
    //节点是否允许编辑
    item_flags = QAbstractItemModel::flags(index);
    // Find Bug
    //item_flags |= Qt::ItemIsEditable;

    return item_flags;
}

QModelIndex File_ItemModel::index(int row, int column, const QModelIndex &parent) const
{
    File_Item *item_parent;
    File_Item *item_child;

    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid())
        item_parent = _item_root;
    else
        item_parent = static_cast<File_Item *>(parent.internalPointer());

    item_child = item_parent->child(row);
    if (item_child)
        return createIndex(row, column, item_child);
    else
        return QModelIndex();
}

QModelIndex File_ItemModel::parent(const QModelIndex &index) const
{
    File_Item *item_parent;
    File_Item *item_child;

    if (!index.isValid())
        return QModelIndex();

    item_child = static_cast<File_Item *>(index.internalPointer());
    item_parent = item_child->parent();

    if (item_parent == _item_root)
        return QModelIndex();

    return createIndex(item_parent->row(), 0, item_parent);
}

int File_ItemModel::rowCount(const QModelIndex &parent) const
{
    File_Item *item_parent;

    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        item_parent = _item_root;
    else
        item_parent = static_cast<File_Item *>(parent.internalPointer());

    return item_parent->childCount();
}

int File_ItemModel::columnCount(const QModelIndex &parent) const
{
    return _headers.size();
}

File_Item *File_ItemModel::itemFromIndex(const QModelIndex &index) const
{
    File_Item *item;

    if (!index.isValid())
        return NULL;
    item = static_cast<File_Item *>(index.internalPointer());
    return item;
}

QModelIndex File_ItemModel::indexFromItem(File_Item *item)
{
    QVector<File_Item *> item_vector;
    File_Item *tmp = item;
    QModelIndex tmpIndex;

    while (tmp->parent() != nullptr) {
        item_vector.push_back(item->parent());
        tmp = tmp->parent();
    }
    item_vector.push_back(item);
    item_vector.erase(item_vector.begin());
    for (int i = 0; i < item_vector.size(); ++i) {
        tmpIndex = index(item_vector[i]->row(), 0, tmpIndex);
    }
    return tmpIndex;
}

File_Item *File_ItemModel::root()
{
    return _item_root;
}

Host_SortFilterProxyModel::Host_SortFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

Host_SortFilterProxyModel::~Host_SortFilterProxyModel(void)
{
}

bool Host_SortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex & source_parent) const
{
    //> 设置可以搜索子节点
//    bool filter = QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);

//    if (filter) {
//        return true;
//    } else {
//        // check all decendant's
//        QModelIndex source_index = sourceModel()->index(source_row, 0, source_parent);
//        for (int k = 0; k < sourceModel()->rowCount(source_index); k++) {
//            if (filterAcceptsRow(k, source_index)) {
//                return true;
//            }
//        }
//    }
//    return false;


    // >设置可以搜索多列
//    QString str = this->filterRegExp().pattern();
//    // 搜索第1和第2列
//    for (int i = 1; i < 3; ++i) {
//        QModelIndex idx = sourceModel()->index(source_row, i, source_parent);
//        QString origin = sourceModel()->data(idx).toString();
//        if (origin.contains(str)) {
//            return true;
//        } else {
//            QModelIndex source_index = sourceModel()->index(source_row, 0, source_parent);
//            for (int k = 0; k < sourceModel()->rowCount(source_index); k++) {
//                if (filterAcceptsRow(k, source_index)) {
//                    return true;
//                }
//            }
//        }
//    }
//    return  false;

    //>只支持搜索第一层，但是要展示符合节点的全部子节点
    // 方案循环遍历，但是比较的是他们的第一层父节点
    QString str = this->filterRegExp().pattern();
    // 假设搜索第一列
    File_Item *item = source_parent.data(Qt::UserRole).value<File_Item *>();
    if (item == nullptr) {
        QModelIndex idx = sourceModel()->index(source_row, 1, source_parent);
        File_Item *it = idx.data(Qt::UserRole).value<File_Item *>();
        QString origin = it->mTab;
        if (origin.contains(str)) {
            return true;
        }
        return false;
    }
    QString origin = item->mTab;
    if (origin.contains(str)) {
        QModelIndex source_index = sourceModel()->index(source_row, 0, source_parent);
        for (int k = 0; k < sourceModel()->rowCount(source_index); k++) {
            if (filterAcceptsRow(k, source_index)) {
                return true;
            }
        }
        return true;
    }
    return false;
}
