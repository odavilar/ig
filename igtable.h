#ifndef IGTABLE_H
#define IGTABLE_H

#include <QTableWidget>
#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QComboBox>

class IGTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit IGTable(QWidget *parent = nullptr);
    void addRow(void);
    void init(void);
    void setRow(int row);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

public slots:
    void deleteButtonClicked();


};

#endif // IGTABLE_H
