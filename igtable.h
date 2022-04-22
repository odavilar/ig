#ifndef IGTABLE_H
#define IGTABLE_H

#include <QTableWidget>
#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QComboBox>
#include "igtableframe.h"
#include <QSharedPointer>

class HexIntegerValidator : public QValidator
{
    Q_OBJECT
public:
    explicit HexIntegerValidator(QObject *parent = nullptr);

    QValidator::State validate(QString &input, int &) const;

    void setMaximum(uint maximum);

private:
    uint m_maximum = 0;
};

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


signals:
    void sendButtonClicked(IGTableFrame *frame);
    void updatePeriodicFrames(QSharedPointer<QHash<QString, IGTableFrame>> *periodicFrames);

public slots:
    void deleteButtonClicked();
    void sendClicked();
    void tableCellChanged(int row, int column);

private:
    HexIntegerValidator *m_hexIntegerValidator = nullptr;
    QSharedPointer<QHash<QString, IGTableFrame>> m_frames;
};


#endif // IGTABLE_H
