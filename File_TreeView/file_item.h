#ifndef FILE_ITEM_H
#define FILE_ITEM_H

#include <QVariant>
#include <QList>
#include <QVector>

#include <stdint.h>

// 节点是否允许编辑
#define HTI_CAN_MODIFY    1

class File_Item
{
public:
    explicit File_Item(File_Item *parent = nullptr);
    ~File_Item(void);

    void appendChild(File_Item *child);      // 在本节点下增加子节点
    void removeChildren(void);                    // 清空所有节点

    File_Item *child(int row);               // 获取第row个子节点指针
    File_Item *parent(void);                 // 获取父节点指针
    int childCount(void) const;                 // 子节点计数
    int row(void) const;                        // 获取该节点是父节点的第几个子节点

    //核心函数：获取节点第column列的数据
    QVariant data(int column) const;

    //设置、获取节点是几级节点（就是树的层级）
    int level(void) { return level_; }

    //设置、获取节点存的数据指针
    void setItemData(QVector<QVariant> data) { itemdata_ = data; };
    QVector<QVariant> *ItemData() { return &itemdata_; };

    void setImageIndex(int imageindex) { imageindex_ = imageindex; };
    int ImageIndex(void) { return imageindex_; };
    void setAttributes(uint32_t attributes) { attributes_ = attributes; };
    uint32_t Attributes(void) { return attributes_; };

    QList<File_Item *> *Children(void) { return &children_; };

    //保存该节点是其父节点的第几个子节点，查询优化所用
    void setRow(int row) {
        row_ = row;
    }

    //修改节点内容(只对有数据的节点可修改，比如姓名、三门课，其他是根据这些内容自动求解的，会自动刷新，但是这里没法改)
    void setData(int column, QVariant value);

    QString mTab; // 记录第一层信息，搜索用

private:
    void setLevel(int level) { level_ = level; }
    QList<File_Item *> children_;   //子节点
    File_Item *parent_;          //父节点
    int level_;     //该节点是第几级节点
    int row_;       //记录该item是第几个，可优化查询效率

    uint32_t attributes_;

    int imageindex_;
    int selectedindex_;

    QVector<QVariant> itemdata_;
};
//让自定义的类型设置到QVariant
Q_DECLARE_METATYPE(File_Item *)
#endif // HOST_TREEITEM_H
