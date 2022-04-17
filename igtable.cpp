#include "igtable.h"
#include <deletemsgbutton.h>

enum {
    MaxStandardId = 0x7FF,
    MaxExtendedId = 0x10000000
};

enum {
    MaxPayload = 8,
    MaxPayloadFd = 64
};

HexIntegerValidator::HexIntegerValidator(QObject *parent) :
    QValidator(parent),
    m_maximum(MaxStandardId)
{
}

QValidator::State HexIntegerValidator::validate(QString &input, int &) const
{
    bool ok;
    uint value = input.toUInt(&ok, 16);

    if (input.isEmpty())
        return Intermediate;

    if (!ok || value > m_maximum)
        return Invalid;

    return Acceptable;
}

void HexIntegerValidator::setMaximum(uint maximum)
{
    m_maximum = maximum;
}

IGTable::IGTable(QWidget *parent) :
    QTableWidget(parent)
{
    m_hexIntegerValidator = new HexIntegerValidator(this);

    this->setColumnCount(14);
    this->setRowCount(1);

    QTableWidgetItem *messageNameHeader = new QTableWidgetItem();
    messageNameHeader->setText("Message Name");
    this->setHorizontalHeaderItem(0, messageNameHeader);

    QTableWidgetItem *identifierHeader = new QTableWidgetItem();
    identifierHeader->setText("Identifier");
    this->setHorizontalHeaderItem(1, identifierHeader);

    QTableWidgetItem *channelHeader = new QTableWidgetItem();
    channelHeader->setText("Channel");
    this->setHorizontalHeaderItem(2, channelHeader);
    this->setColumnHidden(2, true); //Hide channel column

    QTableWidgetItem *sendHeader = new QTableWidgetItem();
    sendHeader->setText("Send");
    this->setHorizontalHeaderItem(3, sendHeader);

    QTableWidgetItem *cycleTimeHeader = new QTableWidgetItem();
    cycleTimeHeader->setText("Cycle Time");
    this->setHorizontalHeaderItem(4, cycleTimeHeader);

    QTableWidgetItem *d0Header = new QTableWidgetItem();
    d0Header->setText("0");
    this->setHorizontalHeaderItem(5, d0Header);

    QTableWidgetItem *d1Header = new QTableWidgetItem();
    d1Header->setText("1");
    this->setHorizontalHeaderItem(6, d1Header);

    QTableWidgetItem *d2Header = new QTableWidgetItem();
    d2Header->setText("2");
    this->setHorizontalHeaderItem(7, d2Header);

    QTableWidgetItem *d3Header = new QTableWidgetItem();
    d3Header->setText("3");
    this->setHorizontalHeaderItem(8, d3Header);

    QTableWidgetItem *d4Header = new QTableWidgetItem();
    d4Header ->setText("4");
    this->setHorizontalHeaderItem(9, d4Header);

    QTableWidgetItem * d5Header= new QTableWidgetItem();
    d5Header->setText("5");
    this->setHorizontalHeaderItem(10, d5Header);

    QTableWidgetItem *d6Header = new QTableWidgetItem();
    d6Header->setText("6");
    this->setHorizontalHeaderItem(11, d6Header);

    QTableWidgetItem *d7Header = new QTableWidgetItem();
    d7Header->setText("7");
    this->setHorizontalHeaderItem(12, d7Header);

    QTableWidgetItem *deleteHeader = new QTableWidgetItem();
    deleteHeader->setText("Delete");
    this->setHorizontalHeaderItem(13, deleteHeader);


    QTableWidgetItem *itemRow0 = new QTableWidgetItem();
    itemRow0->setText("Ignition On");
    this->setItem(0,0,itemRow0);

    QTableWidgetItem *itemRow1 = new QTableWidgetItem();
    itemRow1->setText("13E");
    this->setItem(0,1,itemRow1);

    QComboBox * channelBox = new QComboBox();
    this->setCellWidget(0,2, channelBox);

    QPushButton * sendButton = new QPushButton("Send");
    this->setCellWidget(0,3, sendButton);
    connect(sendButton, &QPushButton::clicked, this, &IGTable::sendClicked);

    QTableWidgetItem *period = new QTableWidgetItem("1000");
    period->setCheckState(Qt::Unchecked);
    this->setItem(0,4,period);

    QTableWidgetItem *itemRow5 = new QTableWidgetItem();
    itemRow5->setText("00");

    this->setItem(0,5,itemRow5);
    QTableWidgetItem *itemRow6 = new QTableWidgetItem();
    itemRow6->setText("00");
    this->setItem(0,6,itemRow6);
    QTableWidgetItem *itemRow7 = new QTableWidgetItem();
    itemRow7->setText("00");
    this->setItem(0,7,itemRow7);
    QTableWidgetItem *itemRow8 = new QTableWidgetItem();
    itemRow8->setText("00");
    this->setItem(0,8,itemRow8);
    QTableWidgetItem *itemRow9 = new QTableWidgetItem();
    itemRow9->setText("00");
    this->setItem(0,9,itemRow9);

    QTableWidgetItem *itemRow10 = new QTableWidgetItem();
    itemRow10->setText("00");
    this->setItem(0,10,itemRow10);

    QTableWidgetItem *itemRow11 = new QTableWidgetItem();
    itemRow11->setText("00");
    this->setItem(0,11,itemRow11);

    QTableWidgetItem *itemRow12 = new QTableWidgetItem();
    itemRow12->setText("00");
    this->setItem(0,12,itemRow12);

    DeleteMsgButton *deleteMsgButton = new DeleteMsgButton();
    this->setCellWidget(0, 13, dynamic_cast <QWidget*>(deleteMsgButton));
    deleteMsgButton->setDisabled(true);

    this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    this->resizeColumnsToContents();
}

void IGTable::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<__FUNCTION__<<this->indexAt(event->pos()).isValid();
   if(!this->indexAt(event->pos()).isValid())
   {
       this->addRow();
   }
   else
   {

       QTableWidget::mouseDoubleClickEvent(event);
   }
}

void IGTable::setRow(int row)
{
    DeleteMsgButton *deleteMsgButton = new DeleteMsgButton();
    QTableWidgetItem *period = new QTableWidgetItem("1000");
    QComboBox * comboBox = new QComboBox();
    QPushButton * sendButton = new QPushButton("Send");

    auto model = this->model();

    this->setCellWidget(row,13, dynamic_cast <QWidget*>(deleteMsgButton));
    this->setCellWidget(row,3, sendButton);
    this->setItem(row, 4, period);
    this->setCellWidget(row,2, comboBox);

    period->setCheckState(Qt::Unchecked);

    model->setData(model->index(row,1),QStringLiteral("000"));

    for(int i = 5; i<=12;i++)
    {
        model->setData(model->index(row,i),QStringLiteral("00"));
    }
    connect(deleteMsgButton, &QPushButton::clicked, this, &IGTable::deleteButtonClicked);
    connect(sendButton, &QPushButton::clicked, this, &IGTable::sendClicked);
}

void IGTable::addRow()
{
    int rowCount = this->rowCount();
    this->insertRow(rowCount);
    setRow(rowCount);
}

void IGTable::deleteButtonClicked()
{
    QPushButton *button = dynamic_cast<QPushButton*>(QObject::sender());
    QModelIndex index = this->indexAt(button->pos());
    qDebug()<<__FUNCTION__<<index.row();
    this->removeRow(index.row());
}

void IGTable::sendClicked()
{
    QPushButton *button = dynamic_cast<QPushButton*>(QObject::sender());
    QModelIndex index = this->indexAt(button->pos());
    bool periodic = false;

    qDebug()<<__FUNCTION__<<index.row();

    if(this->item(index.row(),4)->checkState() == Qt::Checked)
        periodic = true;

    QByteArray data;
    data.resize(8);
    for(int i = 0; i < data.size(); i++)
    {
        data[i] = this->item(index.row(),i + 5)->text().toUInt(nullptr, 16);
    }

    IGTableFrame * frame = new IGTableFrame(this->item(index.row(),1)->text().toUInt(nullptr, 16),periodic,this->item(index.row(),4)->text().toFloat(),data);
    //frame->print();
    emit sendButtonClicked(frame);

    delete frame;
    frame = nullptr;
}
