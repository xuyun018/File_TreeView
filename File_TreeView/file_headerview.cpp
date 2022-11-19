#include "file_headerview.h"

File_HeaderView::File_HeaderView(Qt::Orientation orientation, QWidget *parent)
    : QHeaderView(orientation, parent)
{
    setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setStretchLastSection(true);
}
File_HeaderView::~File_HeaderView(void)
{
    //
}

void File_HeaderView::paintEvent(QPaintEvent *e)
{
    QHeaderView::paintEvent(e);
}
void File_HeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    QHeaderView::paintSection(painter, rect, logicalIndex);
    painter->drawText(rect, Qt::AlignCenter, "Test");
}
