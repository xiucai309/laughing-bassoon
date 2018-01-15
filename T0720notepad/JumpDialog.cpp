#include "JumpDialog.h"
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

JumpDialog::JumpDialog(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *vbMain = new QVBoxLayout(this);
    QHBoxLayout *layoutButton = new QHBoxLayout;
    QLabel *label;
    vbMain->setSpacing(10);
    vbMain->insertSpacing(0, 0);
    vbMain->addWidget(label = new QLabel("行号(L):", this));
    vbMain->addWidget(_lineNumber = new QLineEdit(this));
    vbMain->addSpacing(7);
    vbMain->addLayout(layoutButton);

    layoutButton->addStretch(1);
    layoutButton->addWidget(_jumpButton = new QPushButton("转到", this));
    layoutButton->addWidget(_cancelButton = new QPushButton("取消", this));

    label->setFont(QFont("华文宋体", 10, 2));
    _lineNumber->setToolTip(tr("你只能在此键入数字"));
    _lineNumber->setFixedSize(265, 30);
    _jumpButton->setFixedSize(85, 28);
    _cancelButton->setFixedSize(85, 28);

    this->setWindowTitle(tr("转到指定行"));
    this->setWindowFlags(Qt::Drawer);
    this->setFixedSize(293, 120);


    connect(_lineNumber, SIGNAL(textChanged(QString)), this, SLOT(slotTextChanged(QString)));
    connect(_jumpButton, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
    connect(_cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

void JumpDialog::slotButtonClicked()
{
    emit jumpToSelectedLine(_lineNumber->text().toInt());
}

void JumpDialog::slotTextChanged(QString c)
{
    //qDebug() << c;
}
