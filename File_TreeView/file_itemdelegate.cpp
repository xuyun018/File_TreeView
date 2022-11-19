#include "file_itemdelegate.h"

#include <QHeaderView>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QtWin>

#include "file_item.h"
#include "file_itemmodel.h"

#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>

constexpr int IDENT = 20;
//const QColor COLOR_HOVER(250, 138, 56, 255);
const QColor COLOR_HOVER(229, 243, 255, 255);
//const QColor COLOR_SELECTED(248, 192, 80, 255);
const QColor COLOR_SELECTED(204, 232, 255, 255);
constexpr int LINE_HEIGHT = 30;
constexpr qreal PADDING = IDENT / 8.0;
//const QColor HIGHTLIGHT_TEXT_COLOR(255, 0, 0);
const QColor HIGHTLIGHT_TEXT_COLOR(0, 0, 0);

File_ItemDelegate::File_ItemDelegate(QTreeView *view, QObject *parent) : QStyledItemDelegate(parent)
{
    view_ = view;

    setQss();

    SHFILEINFO sfi;

    himagelist_ = (void *)SHGetFileInfo(L"", 0, &sfi, sizeof(sfi), SHGFI_SMALLICON | SHGFI_SYSICONINDEX);

    view_->setIndentation(0);
    //_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view_->setUniformRowHeights(false);

    connect(view_->header(), &QHeaderView::sectionResized, this,
            [](int logicalIndex, int oldSize, int newSize)
            {
                if (logicalIndex == 2)
                {
                    oldSize;
                    newSize;
                }
            }
    );
}
File_ItemDelegate::~File_ItemDelegate(void)
{
    ImageList_Destroy((HIMAGELIST)himagelist_);
}

void File_ItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    File_Item *item;
    QColor color;
    QRect btn_rect;
    int level;
    int indent;

    const_cast<QAbstractItemModel*>(index.model())->setData(index, option.rect.width(), Qt::UserRole+1);
    QStyleOptionViewItem op = option;
    item = index.data(Qt::UserRole).value<File_Item *>();
    if (index.column() == 0) {
        level = item->level();
        indent = level * IDENT;
        op.rect = op.rect.adjusted(indent, 0, 0, 0);

        btn_rect.setRect(op.rect.left() - IDENT, op.rect.top(), IDENT, op.rect.height());

        //QStyledItemDelegate::paint(painter, op, index);
    } else {
        //        if (index.column() == 2) {
        //            QTextDocument doc;
        //            painter->save();
        //            doc.setHtml(item->ItemData()[index.column()].toString());
        //            doc.setTextWidth(option.rect.width());
        //            painter->translate(option.rect.x(), option.rect.y());
        //            doc.drawContents(painter);
        //            painter->restore();
        //        } else
        if (index.column() == 3) {
            // Find Bug
            //_view->openPersistentEditor(index);
            QStyledItemDelegate::paint(painter, op, index);
        } else {
            QStyledItemDelegate::paint(painter, op, index);
        }

    }

    if (op.state.testFlag(QStyle::State_Selected)) {
        color = COLOR_SELECTED;
    }
    else if (op.state.testFlag(QStyle::State_MouseOver)) {
        color = COLOR_HOVER;
    }
    else
    {
        color = QColor(255, 255, 255);
    }
    painter->fillRect(option.rect, color);

    if (op.state.testFlag(QStyle::State_Selected)) {
        painter->setPen(QColor(153, 209, 255));
        painter->drawLine(option.rect.left(), option.rect.top(), option.rect.right(), option.rect.top());
        painter->drawLine(option.rect.left(), option.rect.bottom() - 1, option.rect.right(), option.rect.bottom() - 1);
        if (index.column() == 0) {
            painter->drawLine(option.rect.left(), option.rect.top(), option.rect.left(), option.rect.bottom() - 1);
        }
        else if (index.column() + 1 == index.model()->columnCount()) {
            painter->drawLine(option.rect.right() - 1, option.rect.top(), option.rect.right() - 1, option.rect.bottom() - 1);
        }
    }

    if (index.column() == 0) {
        if (index.model()->rowCount(index.siblingAtColumn(0)) >  0) {
            DrawButton(painter, btn_rect, index);
        }
        btn_rect.setRect(op.rect.left(), op.rect.top(), IDENT, op.rect.height());

        HICON hicon = ImageList_GetIcon((HIMAGELIST)himagelist_, item->ImageIndex(), ILD_NORMAL);
        if (hicon)
        {
            int left, top;
            int width, height;

            QPixmap pixmap = QtWin::fromHICON(hicon);

            width = pixmap.width();
            height = pixmap.height();

            left = (IDENT - width) >> 1;
            top = (op.rect.height() - height) >> 1;

            btn_rect.setRect(op.rect.left() + left, op.rect.top() + top, width, height);

            painter->drawPixmap(btn_rect, pixmap);
        }

        op.rect = op.rect.adjusted(IDENT, 0, 0, 0);
        //QStyledItemDelegate::paint(painter, op, index);
    }
    DrawKeyword(painter, op, index);
}

QSize File_ItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    Q_UNUSED(index)
//    return QSize(option.rect.width(), LINE_HEIGHT);
//    if (m_View->header()->sectionSize(2) < 80) {
//        qDebug() << "thisxxxx";
//        return QStyledItemDelegate::sizeHint(option, index);
//    }
    if (view_->currentIndex() == index) {
        return QStyledItemDelegate::sizeHint(option, index);
    }
    return QSize (QStyledItemDelegate::sizeHint(option, index).width(), LINE_HEIGHT);
}

bool File_ItemDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
    File_Item *item;

    Q_UNUSED(model)
    if (index.column() != 0) {
        return false;
    }
    item = index.data(Qt::UserRole).value<File_Item *>();
    int ident = item->level() * IDENT;
    QRect hotArea(option.rect.x(), option.rect.y(), ident, option.rect.height());
    QMouseEvent *mEvent = static_cast<QMouseEvent *>(event);
    if (mEvent->type() == QEvent::MouseButtonPress && hotArea.contains(mEvent->pos())) {
        view_->setExpanded(index.siblingAtColumn(0), !view_->isExpanded(index.siblingAtColumn(0)));
        return true;
    }
    return false;
}

QWidget * File_ItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    File_Item *item;

    Q_UNUSED(option)
    if (index.column() != 3) {
        /*
        QLineEdit *le = new QLineEdit(parent);
        connect(le, &QLineEdit::returnPressed, this, [le, index](){
            TreeItem *item = index.data(Qt::UserRole).value<TreeItem *>();
            item->setData(index.column(), QVariant(le->text()));
        });
        return le;
        */
    }
    else {
        /*
        QPushButton *b = new QPushButton("777", parent);
        connect(b, &QPushButton::clicked, this, [b,this, option, index]() {
            for (int i = 0; i <= index.column(); i++) {
                // 提醒界面数据刷新了，重新从sizehint获取size大小
                const_cast<QAbstractItemModel*>(index.model())->dataChanged(index, index);
                // 界面刷新
                _view->doItemsLayout();
    //          _view->viewport()->update();
            }
        });
        return b;
        */
    }
}

void File_ItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    return;
}

void File_ItemDelegate::DrawButton(QPainter *painter, QRect &rect, const QModelIndex &index) const
{
    int x, y;
    int left, top;
    unsigned int width, height;
    int buttonwidth = 9, buttonheight = 9;

    width = rect.width();
    height = rect.height();

    left = (width - buttonwidth) / 2;
    top = (height - buttonheight) / 2;

    x = rect.left() + left;
    y = rect.top() + top;

    //TreeItem *item = index.data(Qt::UserRole).value<TreeItem *>();
    painter->save();

    QModelIndex idx = index.siblingAtColumn(0);

    painter->setPen(QPen(QColor(128, 128, 128), 1));

    painter->drawLine(x, y, x + buttonwidth - 2, y);
    painter->drawLine(x + buttonwidth - 1, y, x + buttonwidth - 1, y + buttonheight - 2);
    painter->drawLine(x + buttonwidth - 1, y + buttonheight - 1, x + 1, y + buttonheight - 1);
    painter->drawLine(x, y + buttonheight - 1, x, y + 1);

    painter->setPen(QPen(QColor(0, 0, 0), 1));
    painter->drawLine(x + 2, y + 4, x + 2 + 5 - 1, y + 4);
    if (!view_->isExpanded(idx)) {
        painter->drawLine(x + 4, y + 2, x + 4, y + 2 + 5 - 1);
    }

    painter->restore();
}

void File_ItemDelegate::setQss()
{
    view_->header()->setDefaultAlignment(Qt::AlignCenter);
    QString qss = QString("QHeaderView::section { \
                    height:20px; \
                    border:0.5px solid rgb(210, 210, 210); \
                    background-color:qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:0 white, stop:1 rgba(239, 239, 239, 255)) \
                    } \
                    QTreeView{ \
                    outline:0px; \
                    } \
                    QTreeView::item:hover,QTreeView::branch:hover{ \
                    background:rgba(%1,%2,%3,%4); \
                    } \
                    QTreeView::item:selected,QTreeView::branch:selected{ \
                    background:rgba(%5,%6,%7,%8); \
                    }").arg(QString::number(COLOR_HOVER.red()), QString::number(COLOR_HOVER.green()), QString::number(COLOR_HOVER.blue()), QString::number(COLOR_HOVER.alpha()),
    QString::number(COLOR_SELECTED.red()), QString::number(COLOR_SELECTED.green()), QString::number(COLOR_SELECTED.blue()),QString::number(COLOR_SELECTED.alpha()));
    view_->setStyleSheet(qss);
}

void File_ItemDelegate::DrawKeyword(QPainter *painter,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const
{
    File_Item *item = index.data(Qt::UserRole).value<File_Item *>();
    QStringList list = SpliteWord((*item->ItemData())[index.column()].toString());
    QPen p = painter->pen();
    QFontMetrics fm = painter->fontMetrics();
    int ident = PADDING;
    for (int i = 0; i < list.size(); i++) {
        painter->save();
        if (list[i] == _match_word) {
            p.setColor(HIGHTLIGHT_TEXT_COLOR);
            painter->setPen(p);
            if (ident > option.rect.width()) {
                painter->restore();
                return;
            }
            QRect rc = option.rect.adjusted(ident, 0, 0, 0);
            painter->drawText(option.rect.adjusted(ident, 0, 0, 0), Qt::AlignVCenter, list[i]);
            ident += fm.width(list[i]);
        } else {
            if (option.state.testFlag(QStyle::State_Selected)) {
                //p.setColor(option.palette.highlightedText().color());
                p.setColor(option.palette.text().color());
                painter->setPen(p);
            } else {
                p.setColor(option.palette.text().color());
                painter->setPen(p);
            }
            if (ident > option.rect.width()) {
                painter->restore();
                return;
            }
            painter->drawText(option.rect.adjusted(ident, 0, 0, 0), Qt::AlignVCenter, list[i]);
            ident += fm.width(list[i]);
        }
        painter->restore();
    }
}

QStringList File_ItemDelegate::SpliteWord(QString originWord) const
{
    QStringList list;
    list = originWord.split(_match_word);
    if (list.size() == 2) {
        if (list[0] == "") {
            list.insert(0, _match_word);
        } else if (list[1] == "") {
            list.append(_match_word);
        } else {
            list.insert(1, _match_word);
        }
    }
    return list;
}
