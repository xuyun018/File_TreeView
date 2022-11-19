#include "file_item.h"

File_Item::File_Item(File_Item *parent)
{
    parent_ = parent;
    level_ = 0;
    row_ = 0;

    imageindex_ = 0;
    selectedindex_ = 0;
}
File_Item::~File_Item(void)
{
    removeChildren();
}

void File_Item::appendChild(File_Item *item)
{
    item->setRow(children_.size());   //item存自己是第几个，可以优化效率
    children_.append(item);
    item->setLevel(level_ + 1);
    if (item->level_ == 1) {
        item->mTab = (*item->ItemData())[1].toString();
    } else {
        item->mTab = this->mTab;
    }
}

void File_Item::removeChildren(void)
{
    qDeleteAll(children_);
    children_.clear();
}

File_Item *File_Item::child(int row)
{
    return children_.value(row);
}

File_Item *File_Item::parent(void)
{
    return parent_;
}

int File_Item::childCount(void) const
{
    return children_.count();
}

int File_Item::row(void) const
{
    return row_;
}

QVariant File_Item::data(int column) const
{
    if (column < itemdata_.size()) {
        return itemdata_[column];
    }
    //if (mLevel == 1)
    //{
    //    //一级节点，班级
    //    if (column == 0)
    //    {
    //        CLASS* c = (CLASS*)mPtr;
    //        return c->name;
    //    }
    //}
    //else if (mLevel == 2)
    //{
    //    //二级节点学生信息
    //    STUDENT* s = (STUDENT*)mPtr;
    //    switch (column)
    //    {
    //    case 0: return s->name;
    //    case 1: return QString::number(s->score1);
    //    case 2: return QString::number(s->score2);
    //    case 3: return QString::number(s->score3);
    //    case 4: return QString::number(s->score1 + s->score2 + s->score3);
    //    case 5: return QString::number((s->score1 + s->score2 + s->score3) / 3);
    //    case 6:
    //    {
    //        if (s->score1 < 60 || s->score2 < 60 || s->score3 < 60)
    //        {
    //            //任意一门课不合格则不合格
    //            return QStringLiteral("不合格");
    //        }
    //        else
    //        {
    //            return QStringLiteral("合格");
    //        }
    //    }
    //    case 7:
    //    {
    //        if (s->score1 >= 90 && s->score2 >= 90 && s->score3 >= 90)
    //        {
    //            //每门课都达到90分以上评优
    //            return QStringLiteral("优秀");
    //        }
    //        else
    //        {
    //            return QStringLiteral("-");
    //        }
    //    }
    //    default:
    //        return QVariant();
    //    }
    //}
    return QVariant();
}

void File_Item::setData(int column, QVariant value)
{
    if (column >= itemdata_.size()) {
        return;
    }
    itemdata_[column] = value;
}
