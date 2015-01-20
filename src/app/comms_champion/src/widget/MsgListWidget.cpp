//
// Copyright 2014 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "MsgListWidget.h"

#include <cassert>

#include <QtCore/QVariant>

#include "comms_champion/Message.h"

namespace comms_champion
{

namespace
{

const QString EmptyStr;

}  // namespace

MsgListWidget::MsgListWidget(
    const QString& listName,
    QWidget* toolbar,
    QWidget* parent)
  : Base(parent)
{
    m_ui.setupUi(this);
    m_ui.m_groupBoxLayout->insertWidget(0, toolbar);
    m_ui.m_groupBox->setTitle(listName);

    connect(m_ui.m_listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(itemClicked(QListWidgetItem*)));
    connect(m_ui.m_listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(itemDoubleClicked(QListWidgetItem*)));
}

void MsgListWidget::addMessage(MessageInfoPtr msgInfo)
{
    assert(msgInfo);
    m_ui.m_listWidget->addItem(getMsgNameText(msgInfo));
    auto* item = m_ui.m_listWidget->item(m_ui.m_listWidget->count() - 1);
    item->setToolTip(msgTooltipImpl());
    item->setData(
        Qt::UserRole,
        QVariant::fromValue(msgInfo));

    if (m_selectOnAdd) {
        item->setSelected(true);
    }

    if (m_ui.m_listWidget->currentRow() < 0) {
        m_ui.m_listWidget->scrollToBottom();
    }
}

void MsgListWidget::updateCurrentMessage()
{
    auto* item = m_ui.m_listWidget->currentItem();
    if (item == nullptr) {
        assert(!"No item is selected for update");
        return;
    }

    auto msgInfo = getMsgFromItem(item);
    assert(msgInfo);
    item->setText(getMsgNameText(std::move(msgInfo)));
}

void MsgListWidget::selectOnAdd(bool enabled)
{
    m_selectOnAdd = enabled;
}

void MsgListWidget::clearSelection()
{
    m_ui.m_listWidget->clearSelection();
    m_ui.m_listWidget->setCurrentRow(-1);
}

void MsgListWidget::msgClickedImpl(MessageInfoPtr msgInfo)
{
    static_cast<void>(msgInfo);
}

void MsgListWidget::msgDoubleClickedImpl(MessageInfoPtr msgInfo)
{
    static_cast<void>(msgInfo);
}

QString MsgListWidget::msgPrefixImpl(const MessageInfo& msgInfo) const
{
    static_cast<void>(msgInfo);
    return QString();
}

const QString& MsgListWidget::msgTooltipImpl() const
{
    return EmptyStr;
}

void MsgListWidget::itemClicked(QListWidgetItem* item)
{
    msgClickedImpl(getMsgFromItem(item));
}

void MsgListWidget::itemDoubleClicked(QListWidgetItem* item)
{
    msgDoubleClickedImpl(getMsgFromItem(item));
}

MessageInfoPtr MsgListWidget::getMsgFromItem(QListWidgetItem* item) const
{
    auto var = item->data(Qt::UserRole);
    assert(var.canConvert<MessageInfoPtr>());
    return var.value<MessageInfoPtr>();
}

QString MsgListWidget::getMsgNameText(MessageInfoPtr msgInfo)
{
    assert(msgInfo);
    auto msg = msgInfo->getAppMessage();
    assert(msg);

    auto itemStr = msgPrefixImpl(*msgInfo);
    if (!itemStr.isEmpty()) {
        itemStr.append(": ");
    }
    itemStr.append(msg->name());
    return itemStr;
}

}  // namespace comms_champion


