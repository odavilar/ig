#include "deletemsgbutton.h"

DeleteMsgButton::DeleteMsgButton()
{
    QIcon * deleteIcon = new QIcon(":/icons/delete");
    this->setIcon(*deleteIcon);
}
