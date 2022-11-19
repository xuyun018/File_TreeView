#ifndef FILE_HEADERVIEW_H
#define FILE_HEADERVIEW_H

#include <QHeaderView>
#include <QPainter>

class File_HeaderView : public QHeaderView
{
    Q_OBJECT
public:
    File_HeaderView(Qt::Orientation orientation, QWidget *parent = nullptr);
    ~File_HeaderView(void);

protected:
    void paintEvent(QPaintEvent *e) override;
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
};

#endif // HOST_HEADERVIEW_H
