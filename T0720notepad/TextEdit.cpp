/**
 *  将 NotePad 类中显示的 "编辑" 菜单 _editMenu 及 查找对话框 SearchDialog 定义和封装在 TextEdit 中,
 *  在 NotePad 中通过接口 QMenu *getEditMenu() 获得并添加在菜单条中(QMenuBar)显示
 *
 *  TextEdit 中同样实现了对指定文本文件输入输出接口.
 */
#include "TextEdit.h"
#include <QDebug>
#include <QMessageBox>
#include <QTextBlock>


TextEdit::TextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    createEditMenum();                           //为文件菜单添加QAction.
    bulidConnects();                             //建立信号与槽连接.

    //_jumpDialog = new JumpDialog(this);

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setLineWrapMode(LineWrapMode::NoWrap); //设置非自动换行.
    this->setFont(QFont("宋体",12, 2));           //设置编辑框的字体.
}

void TextEdit::createEditMenum()
{
    _editMenu = new QMenu("编辑(E)");
    _undoAction = _editMenu->addAction("撤销(U)             ", this, SLOT(undo()), QKeySequence::Undo);
    _undoAction->setEnabled(false);

    _editMenu->addSeparator();

    _cutAction = _editMenu->addAction("剪切(T)", this, SLOT(cut()), QKeySequence::Cut);
    _cutAction->setEnabled(false);

    _copyAction = _editMenu->addAction("复制(C)", this, SLOT(slotCopy()), QKeySequence::Copy);
    _copyAction->setEnabled(false);

    _pasteAction = _editMenu->addAction("粘贴(P)", this, SLOT(paste()), QKeySequence::Paste);
    _pasteAction->setEnabled(this->canPaste());

    _deleteAction = _editMenu->addAction("删除(L)", this, SLOT(slotDeleteSeletion()), QKeySequence::Delete);
    _deleteAction->setEnabled(false);

    _editMenu->addSeparator();

    _searchAction = _editMenu->addAction(tr("查找(F)..."), &_searchDialog, SLOT(show()), QKeySequence::Find);
    _searchAction->setEnabled(false);
    _searchNextAction = _editMenu->addAction("查找下一个(N)", &_searchDialog, SLOT(slotFindNextText()),
                                                                                    QKeySequence::FindNext);
    _searchNextAction->setEnabled(false);
    _jumpAction = _editMenu->addAction("转到(G)...", &_jumpDialog, SLOT(exec()));

    _editMenu->addSeparator();

    _selectAllAction = _editMenu->addAction("全选(A)", this, SLOT(selectAll()), QKeySequence::SelectAll);
    _datetimeAction = _editMenu->addAction("时间/日期(D)", this, SLOT(slotDateTime()),QKeySequence("F5"));
}

void TextEdit::bulidConnects()
{
    connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(slotUndoAvailable(bool)));
    connect(this, SIGNAL(copyAvailable(bool)), this, SLOT(slotCopyAvailable(bool)));
    connect(this, SIGNAL(textChanged()), this, SLOT(slotTextChanged()));
    connect(&_searchDialog, SIGNAL(searchText(QString,bool,bool)), this, SLOT(slotSearchText(QString,bool,bool)));
    connect(&_jumpDialog, SIGNAL(jumpToSelectedLine(int)), this, SLOT(slotJumpToline(int)));
}

//设置 undo 选项根据 信号 决定_undoAction是否可选.
void TextEdit::slotUndoAvailable(bool flage)
{
    _undoAction->setEnabled(flage);
}

//设置指定的QAction 根据 信号 决定是否可选.
void TextEdit::slotCopyAvailable(bool flage)
{
    _copyAction->setEnabled(flage);
    _cutAction->setEnabled(flage);
    _deleteAction->setEnabled(flage);
}

//删除 光标(Cursor) 选中的文本.
void TextEdit::slotDeleteSeletion()
{
    //this->textCursor().deletePreviousChar();
    this->textCursor().removeSelectedText();
}

//复制选中的文本.
void TextEdit::slotCopy()
{
    this->copy();
    //复制完成后"粘贴"选项设置为可选.
    _pasteAction->setEnabled(true);
}

//在文本指定位置中插入系统时间和日期.
void TextEdit::slotDateTime()
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString strDateTime = datetime.toString(Qt::SystemLocaleDate);
    this->textCursor().insertText(strDateTime);
}

//接收到SearchDialog发出的信号后进行查找.
void TextEdit::slotSearchText(QString text, bool isUp, bool isCaseSensitive)
{
    bool hasFind;
    if(isUp)
    {
        if(isCaseSensitive)
            hasFind = this->find(text, QTextDocument::FindBackward |
                             QTextDocument::FindCaseSensitively);
        else
            hasFind = this->find(text, QTextDocument::FindBackward);
    }
    else
    {
        if(isCaseSensitive)
            hasFind = this->find(text, QTextDocument::FindCaseSensitively);
        else
            hasFind = this->find(text);
    }

    if(!hasFind)
        QMessageBox::information(&_searchDialog, tr("记事本"), tr("找不到 ") + "“" + text + "”");
}

//当文本为 Empty 时, 查找选项不可选.
void TextEdit::slotTextChanged()
{
    if(!this->toPlainText().isEmpty())
    {
        _searchAction->setEnabled(true);
        _searchNextAction->setEnabled(true);
    }
    else
    {
        _searchAction->setEnabled(false);
        _searchNextAction->setEnabled(false);
    }
}

//设置跳转选项是否可选.
void TextEdit::setJumpEnable(bool flage)
{
    _jumpAction->setEnabled(flage);
}

//转到指定行.
void TextEdit::slotJumpToline(int num)
{
    const QTextBlock &b = this->document()->findBlockByLineNumber(num-1);
    if(b.blockNumber() != -1)
    {
        this->setTextCursor(QTextCursor(b));
        _jumpDialog.close();
    }
    else
    {
        QMessageBox inform;
        inform.setWindowTitle(tr("记事本-跳行"));
        inform.setText(tr("行数超过了总行数"));
        inform.setWindowFlags(Qt::Drawer);
        inform.exec();
    }
}

//将QTextEdit输出至文件.
void TextEdit::outputFile(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        throw fileName;
    }

    QTextStream outputstream(&file);

    outputstream << this->toPlainText();
    document()->setModified(false);

    file.close();

}
//将文件内容输入至QTextEdit.
void TextEdit::inputFile(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        //错误处理.
        throw fileName;
    }

    QTextStream inputstream(&file);

    setPlainText(inputstream.readAll());
    document()->setModified(false);

    file.close();

}

TextEdit::~TextEdit()
{
    delete _editMenu;
}
