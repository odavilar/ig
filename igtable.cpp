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

enum Column {
    uuid,
    deleteButton,
    messageName,
    identifier,
    channel,
    sendButton,
    periodTime,
    d0,
    d1,
    d2,
    d3,
    d4,
    d5,
    d6,
    d7,
    maxColumns
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

    this->setColumnCount(Column::maxColumns);
    this->setRowCount(1);

    QTableWidgetItem *uuidHeader = new QTableWidgetItem();
    uuidHeader->setText("Uuid");
    this->setHorizontalHeaderItem(Column::uuid, uuidHeader);

    QTableWidgetItem *messageNameHeader = new QTableWidgetItem();
    messageNameHeader->setText("Message Name");
    this->setHorizontalHeaderItem(Column::messageName, messageNameHeader);

    QTableWidgetItem *identifierHeader = new QTableWidgetItem();
    identifierHeader->setText("Identifier");
    this->setHorizontalHeaderItem(Column::identifier, identifierHeader);

    QTableWidgetItem *channelHeader = new QTableWidgetItem();
    channelHeader->setText("Channel");
    this->setHorizontalHeaderItem(Column::channel, channelHeader);
    this->setColumnHidden(Column::channel, true); //Hide channel column

    QTableWidgetItem *sendHeader = new QTableWidgetItem();
    sendHeader->setText("Send");
    this->setHorizontalHeaderItem(Column::sendButton, sendHeader);

    QTableWidgetItem *cycleTimeHeader = new QTableWidgetItem();
    cycleTimeHeader->setText("Cycle Time");
    this->setHorizontalHeaderItem(Column::periodTime, cycleTimeHeader);

    QTableWidgetItem *d0Header = new QTableWidgetItem();
    d0Header->setText("0");
    this->setHorizontalHeaderItem(Column::d0, d0Header);

    QTableWidgetItem *d1Header = new QTableWidgetItem();
    d1Header->setText("1");
    this->setHorizontalHeaderItem(Column::d1, d1Header);

    QTableWidgetItem *d2Header = new QTableWidgetItem();
    d2Header->setText("2");
    this->setHorizontalHeaderItem(Column::d2, d2Header);

    QTableWidgetItem *d3Header = new QTableWidgetItem();
    d3Header->setText("3");
    this->setHorizontalHeaderItem(Column::d3, d3Header);

    QTableWidgetItem *d4Header = new QTableWidgetItem();
    d4Header ->setText("4");
    this->setHorizontalHeaderItem(Column::d4, d4Header);

    QTableWidgetItem * d5Header= new QTableWidgetItem();
    d5Header->setText("5");
    this->setHorizontalHeaderItem(Column::d5, d5Header);

    QTableWidgetItem *d6Header = new QTableWidgetItem();
    d6Header->setText("6");
    this->setHorizontalHeaderItem(Column::d6, d6Header);

    QTableWidgetItem *d7Header = new QTableWidgetItem();
    d7Header->setText("7");
    this->setHorizontalHeaderItem(Column::d7, d7Header);

    QTableWidgetItem *deleteHeader = new QTableWidgetItem();
    deleteHeader->setText("Delete");
    this->setHorizontalHeaderItem(Column::deleteButton, deleteHeader);

    QTableWidgetItem *itemUuid = new QTableWidgetItem();
    QUuid uuid = QUuid::createUuid();
    itemUuid->setText(uuid.toString(QUuid::WithoutBraces));
    this->setItem(0,Column::uuid,itemUuid);
    this->setColumnHidden(Column::uuid, true); //Hide uuid column

    QTableWidgetItem *itemMessageName = new QTableWidgetItem();
    itemMessageName->setText("Ignition On");
    this->setItem(0,Column::messageName,itemMessageName);

    QTableWidgetItem *itemIdentifier = new QTableWidgetItem();
    itemIdentifier->setText("13E");
    this->setItem(0,Column::identifier,itemIdentifier);

    QComboBox * channelBox = new QComboBox();
    this->setCellWidget(0,Column::channel, channelBox);

    QPushButton * sendButton = new QPushButton("Send");
    this->setCellWidget(0,Column::sendButton, sendButton);
    connect(sendButton, &QPushButton::clicked, this, &IGTable::sendClicked);

    QTableWidgetItem *period = new QTableWidgetItem("1000");
    period->setCheckState(Qt::Unchecked);
    this->setItem(0,Column::periodTime,period);

    QTableWidgetItem *itemD0 = new QTableWidgetItem();
    itemD0->setText("00");
    this->setItem(0,Column::d0,itemD0);
    QTableWidgetItem *itemD1 = new QTableWidgetItem();
    itemD1->setText("00");
    this->setItem(0,Column::d1,itemD1);
    QTableWidgetItem *itemD2 = new QTableWidgetItem();
    itemD2->setText("00");
    this->setItem(0,Column::d2,itemD2);
    QTableWidgetItem *itemD3 = new QTableWidgetItem();
    itemD3->setText("00");
    this->setItem(0,Column::d3,itemD3);
    QTableWidgetItem *itemD4 = new QTableWidgetItem();
    itemD4->setText("00");
    this->setItem(0,Column::d4,itemD4);

    QTableWidgetItem *itemD5 = new QTableWidgetItem();
    itemD5->setText("00");
    this->setItem(0,Column::d5,itemD5);

    QTableWidgetItem *itemD6 = new QTableWidgetItem();
    itemD6->setText("00");
    this->setItem(0,Column::d6,itemD6);

    QTableWidgetItem *itemD7 = new QTableWidgetItem();
    itemD7->setText("00");
    this->setItem(0,Column::d7,itemD7);

    DeleteMsgButton *deleteMsgButton = new DeleteMsgButton();
    this->setCellWidget(0, Column::deleteButton, dynamic_cast <QWidget*>(deleteMsgButton));
    deleteMsgButton->setDisabled(true);

    this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    this->resizeColumnsToContents();

    m_frames = QSharedPointer<IGHash>(new IGHash());

    QByteArray data;
    data.resize(8);
    for(int i = 0; i < data.size(); i++)
    {
        data[i] = 0;
    }

    m_frames->insert(uuid.toString(QUuid::WithoutBraces), IGFrame(uuid, itemIdentifier->text().toUInt(nullptr, 16), false, period->text().toUInt(), data));

    connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(tableCellChanged(int, int)));
}

void IGTable::mouseDoubleClickEvent(QMouseEvent *event)
{
   //qDebug()<<__FUNCTION__<<this->indexAt(event->pos()).isValid();
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
    QUuid uuid;
    QByteArray data;
    QString identifier = "000";

    auto model = this->model();

    uuid = QUuid::createUuid();

    data.resize(8);
    for(int i = 0; i < data.size(); i++)
    {
        data[i] = 0;
    }

    this->setCellWidget(row, Column::deleteButton, dynamic_cast <QWidget*>(deleteMsgButton));
    this->setCellWidget(row, Column::sendButton, sendButton);
    this->setItem(row, Column::periodTime, period);
    this->setCellWidget(row, Column::channel, comboBox);

    period->setCheckState(Qt::Unchecked);

    model->setData(model->index(row,Column::messageName), "Message name");
    model->setData(model->index(row,Column::identifier), identifier);
    model->setData(model->index(row,Column::uuid), uuid.toString(QUuid::WithoutBraces));

    for(int i = Column::d0; i<=Column::d7;i++)
    {
        model->setData(model->index(row,i),QStringLiteral("00"));
    }

    IGFrame newFrame = IGFrame(uuid, identifier.toUInt(), false, period->text().toUInt(), data);

    m_frames->lock();
    m_frames->insert(uuid.toString(QUuid::WithoutBraces), newFrame);
    m_frames->unlock();

    emit updateFrame(newFrame);

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

    qDebug()<<"row deleted "<<this->item(index.row(),Column::uuid)->text();

    emit deleteFrame(this->item(index.row(),Column::uuid)->text());

    m_frames->lock();
    m_frames->remove(this->item(index.row(),Column::uuid)->text());
    m_frames->unlock();

    this->removeRow(index.row());
}

void IGTable::sendClicked()
{
    QPushButton *button = dynamic_cast<QPushButton*>(QObject::sender());
    QModelIndex index = this->indexAt(button->pos());
    bool periodic = false;

    qDebug()<<__FUNCTION__<<index.row();

    if(this->item(index.row(),Column::periodTime)->checkState() == Qt::Checked)
        periodic = true;

    QByteArray data;
    data.resize(8);
    for(int i = 0; i < data.size(); i++)
    {
        data[i] = this->item(index.row(),i + Column::d0)->text().toUInt(nullptr, 16);
    }

    IGFrame * frame =
            new IGFrame( QUuid(this->item(index.row(),Column::uuid)->text()),
                this->item(index.row(),Column::identifier)->text().toUInt(nullptr, 16),
                periodic,
                this->item(index.row(),Column::periodTime)->text().toUInt()
                ,data);

    emit sendButtonClicked(frame);

    delete frame;
    frame = nullptr;
}

void IGTable::tableCellChanged(int row, int column)
{

    QTableWidgetItem * item = nullptr;

    item = this->item(row, Column::uuid);
    if(!item)
        return;

    m_frames->lock();
    IGFrame frame = ((*m_frames).value(item->text()));
    m_frames->unlock();

    switch(column)
    {
    case Column::d0:
    case Column::d1:
    case Column::d2:
    case Column::d3:
    case Column::d4:
    case Column::d5:
    case Column::d6:
    case Column::d7:
    {
        QByteArray data;
        data.resize(8);
        for(int i = 0; i < data.size(); i++)
        {
            item = this->item(row,i + Column::d0);
            if(!item)
                return;

            data[i] = item->text().toUInt(nullptr, 16);
        }
        frame.setPayload(data);

    }
        break;
    case Column::identifier:
    {
        item = this->item(row, Column::identifier);
        if(!item)
            return;
        frame.setFrameId(item->text().toUInt(nullptr, 16));

    }
        break;
    case Column::periodTime:
    {
        item = this->item(row, Column::periodTime);
        if(!item)
            return;
        frame.setPeriodic(item->checkState() == Qt::Checked ? true : false);
        frame.setPeriod(item->text().toUInt());

    }
        break;
    }

    m_frames->lock();
    m_frames->insert(frame.getUuid(),frame);
    m_frames->unlock();

    emit updateFrame(frame);
}



