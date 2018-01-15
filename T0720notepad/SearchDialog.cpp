#include "SearchDialog.h"
#include <QLabel>
#include <QCheckBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCloseEvent>

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent)
{
    QHBoxLayout *hbMain = new QHBoxLayout(this);
    QVBoxLayout *layoutLeft = new QVBoxLayout;
    QHBoxLayout *layoutFind = new QHBoxLayout;
    QHBoxLayout *layoutChosic = new QHBoxLayout;
    QVBoxLayout *layoutPush = new QVBoxLayout;
    QHBoxLayout *layoutGroup = new QHBoxLayout;

    layoutFind->addWidget(_findLabel = new QLabel(tr("查找内容(N):"), this));
    layoutFind->addWidget(_lineEdit = new QLineEdit(this));
    layoutChosic->addWidget(_caseSensitive = new QCheckBox(tr("区分大小写"), this), 0, Qt::AlignBottom);
    layoutChosic->addWidget(_groupBox = new QGroupBox(tr("方向"), this));
    layoutGroup->addWidget(_up = new QRadioButton("向上(U)", this));
    layoutGroup->addSpacing(20);
    layoutGroup->addWidget(_down = new QRadioButton("向下(D)", this));
    _groupBox->setLayout(layoutGroup);

    layoutLeft->addLayout(layoutFind);
    layoutLeft->addLayout(layoutChosic);
    layoutLeft->setSpacing(15);

    layoutPush->addWidget(_buttonSearch = new QPushButton(tr("查找下一个(F)"), this));
    layoutPush->addWidget(_buttonCancel = new QPushButton(tr("取消"), this));
    layoutPush->addStretch(1);

    hbMain->setSpacing(10);
    hbMain->addLayout(layoutLeft);
    hbMain->addLayout(layoutPush);


    _findLabel->setFont(QFont("Courier", 10, 2));
    _lineEdit->setFixedSize(210, 27);
    _down->setChecked(true);
    _buttonSearch->setEnabled(false);
    _buttonSearch->setFixedSize(110, 29);
    _buttonCancel->setFixedSize(110, 29);

    this->setWindowTitle(tr("查找"));
    this->resize(290, 130);
    this->setWindowFlags(Qt::Drawer | Qt::WindowStaysOnTopHint);
    this->hide();


    connect(_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotTextChanged(QString)));
    connect(_buttonSearch, SIGNAL(clicked()), this, SLOT(slotButtonSearchClicked()));
    connect(_buttonCancel, SIGNAL(clicked()), this, SLOT(hide()));
}

//当查找编辑框中内容为空时,设置查找按钮不可选.
void SearchDialog::slotTextChanged(QString)
{
    if(_lineEdit->text().isEmpty())
    {
        _buttonSearch->setEnabled(false);
    }
    else
    {
        _buttonSearch->setEnabled(true);
    }
}

//判断查找选项并发射查找信号.
void SearchDialog::slotButtonSearchClicked()
{
    if(_up->isChecked())
    {
        if(_caseSensitive->isChecked())
            emit searchText(_lineEdit->text(), true, true);
        else
            emit searchText(_lineEdit->text(), true, false);
    }
    else
    {
        if(_caseSensitive->isChecked())
            emit searchText(_lineEdit->text(), false, true);
        else
            emit searchText(_lineEdit->text(), false, false);
    }
}

/* 如果_lineEdit编辑框中的查找内容不为空,
 * 则通过slotButtonSearchClicked()发射信号,
 * 否则显示对话框让用户输入查找内容.
 */
void SearchDialog::slotFindNextText()
{
    if(!_lineEdit->text().isEmpty())
    {
        slotButtonSearchClicked();
    }
    else
    {
        this->show();
    }
}

//关闭窗口时隐藏窗口.
void SearchDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
