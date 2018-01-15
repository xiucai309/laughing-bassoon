#include "StatusBar.h"
#include <QAction>

StatusBar::StatusBar(QWidget *parent) :
    QStatusBar(parent)
{
    _statusLabel = new QLabel(this);
    _strachLabel = new QLabel(this);
    this->insertPermanentWidget(0, _strachLabel, 3);
    this->insertPermanentWidget(1, _statusLabel, 1);

    showColumnAndBlock(1, 1);
    this->hide();
}

void StatusBar::showColumnAndBlock(int c, int b)
{
    _statusLabel->setText(
            tr(" 第 %1 行   第 %2 列").arg(c).arg(b));
}


