#ifndef FILE_ITEMDELEGATE_H
#define FILE_ITEMDELEGATE_H

#include <QWidget>
#include <QStyledItemDelegate>
#include <QTreeView>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QEvent>
#include <QAbstractItemModel>
#include <QString>
#include <QStringList>

// QStyledItemDelegate::paint介绍及简单使用
// https://it1995.blog.csdn.net/article/details/82225266
// 对QItemDelegate自定义委托的理解
// https://it1995.blog.csdn.net/article/details/80817741

class File_ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    File_ItemDelegate(QTreeView *view, QObject *parent = nullptr);
    virtual ~File_ItemDelegate(void);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void setMatchWord(QString str);

private:
    void DrawButton(QPainter *painter, QRect &rect, const QModelIndex &index) const;
    void setQss();
    void DrawKeyword(QPainter *painter,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) const;
    QStringList SpliteWord(QString originWord) const;

    QTreeView *view_;
    QString _match_word;
    bool has_proxy;

    void *himagelist_;
};

#endif // HOST_STYLEITEMDELEGATE_H
