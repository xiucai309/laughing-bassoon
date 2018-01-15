#include <QFileDialog>                     //文件选择对话框.
#include <QMessageBox>                     //消息对话框.
#include <QtPrintSupport/QPrintDialog>     //打印对话框.
#include <QPageSetupDialog>                //页面设置对话框.
#include <QFontDialog>
#include <QCursor>                         //QCursor::pos() 返回鼠标相对于屏幕的坐标.
#include <QDebug>

#include "NotePad.h"

NotePad::NotePad(QWidget *parent)
    : QMainWindow(parent)
{
    createMenus();                         //创建所有菜单项.
    createFileActions();                   //为文件菜单添加QAction.
    createFormatAction();                  //为格式菜单添加QAction.
    createViewAction();                    //为查看菜单添加QAction.
    createHelpAction();                    //为帮助菜单添加QAction.
    createStatusBar();                     //创建状态栏.

    /* 别的控件占用了之后剩下的区域为 CentralWidget */
    this->setCentralWidget(&_editArea);
    _currentFile = new QFileInfo;

    this->setWindowTitle("无标题 - 记事本");
    this->setWindowIcon(QIcon("../ConsoleToo.ico"));
    this->resize(1000, 500);

    loadText();
}

void NotePad::createMenus()
{
    _pMenuBar = menuBar();
    _fileMenu = _pMenuBar->addMenu("文件(F)");

    //从TextEdit中获取 "编辑" 菜单.
    _pMenuBar->addMenu(_editArea.getEditMenu());

    _formatMenu = _pMenuBar->addMenu("格式(O)");
    _viewMenu = _pMenuBar->addMenu("查看(V)");
    _helpMenu = _pMenuBar->addMenu("帮助(H)");
}

void NotePad::createFileActions()
{
    _newFile = _fileMenu->addAction("&新建(N)            ", this, SLOT(slotNewFile()), QKeySequence::New);
    _openFile = _fileMenu->addAction("&打开(O)...", this, SLOT(slotOpenFile()), QKeySequence::Open);
    _saveFile = _fileMenu->addAction("&保存(S)"   , this, SLOT(slotSaveFile()), QKeySequence::Save);
    _saveasFile = _fileMenu->addAction("&另存为(A)...", this, SLOT(slotSaveas()),QKeySequence::SaveAs);
    _fileMenu->addSeparator();
    _setPage = _fileMenu->addAction("&页面设置(U)...", this, SLOT(slotSetPage()));
    _printFile = _fileMenu->addAction("&打印(P)...", this, SLOT(slotPrintFile()), QKeySequence::Print);
    _fileMenu->addSeparator();
    _exit = _fileMenu->addAction("&退出(X)...", this, SLOT(slotExit()));
}

void NotePad::createViewAction()
{
    _status = _viewMenu->addAction("状态栏(S)        ", this, SLOT(slotShowStatus()));
    _status->setCheckable(true);
}

void NotePad::createFormatAction()
{
    _setWordWarp = _formatMenu->addAction("自动换行(W)        ", this, SLOT(soltSetWordWrap()));
    _setWordWarp->setCheckable(true);
    _setFont = _formatMenu->addAction("字体(F)...", this, SLOT(soltSetFontAction()));
}

void NotePad::createHelpAction()
{
    _helpMenu->addAction("查看帮助(H)          ");
    _helpMenu->addSeparator();
    _about = _helpMenu->addAction("关于记事本(A)", this, SLOT(slotShowAbout()));
}

void NotePad::createStatusBar()
{
    this->setStatusBar(&_statusBar);
    connect(&_editArea, SIGNAL(cursorPositionChanged()), this, SLOT(slotCursorPositionChanged()));
}

//新建文件.
void NotePad::slotNewFile()
{
    if(_editArea.document()->isModified())
    {
        try {
            questionSaveOrNot();
        }
        catch (int) {
            return;
        }
    }
    _currentFile->setFile(tr(""));
    _editArea.clear();
    //_editArea.document()->setModified(false);
    this->setWindowTitle("无标题 - 记事本");
}
//打开文件.
void NotePad::slotOpenFile()
{
    if(_editArea.document()->isModified())
    {
        try {
            questionSaveOrNot();
        }
        catch(int exp) {
            return;
        }
    }

    QString fileName = QFileDialog::getOpenFileName(nullptr, "打开文件", "/", "文本文档(*.txt);;"
                                                                             "所有文档(*.*)"); //打开文件选择对话框.
    if(!fileName.isEmpty())
    {
        //将选择的文件导入TextEdit...
        _editArea.inputFile(fileName);
        _currentFile->setFile(fileName);//记录当前文件信息.

        this->setWindowTitle(_currentFile->fileName() + " - 记事本");
    }
}
//保存文件.
void NotePad::slotSaveFile()
{
    if(this->windowTitle() == "无标题 - 记事本")
    {
        slotSaveas();//将文件另存为...
    }
    else if(_editArea.document()->isModified())
    {
        /*将QTextEdit导入当前文件.*/
        _editArea.outputFile(_currentFile->absoluteFilePath());
    }
}

//另存为... 如果保存过程中出现"取消",返回false, 保存成功返回true.
bool NotePad::slotSaveas()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "另存为", "*.txt", "文本文档(*.txt);"
                                                                               "C files(*.c);"
                                                                               "C++ files(*.cpp);"
                                                                               "Head files(*.h)");
    if(!fileName.isEmpty())
    {
        _editArea.outputFile(fileName);//将QTextEdit导入选择的文件...
        _currentFile->setFile(fileName);//记录当前文件信息.
        this->setWindowTitle(_currentFile->fileName() + " - 记事本");

        return true;
    }
    return false;
}

//退出时检查保存文件.
void NotePad::slotExit()
{
    if(_editArea.document()->isModified())
    {
        try {
            questionSaveOrNot();
        }
        catch(int) {
            return;
        }
    }
    this->close();
}

/**
 *  如果在保存过程中出现"取消"保存,则抛出异常.
 */
void NotePad::questionSaveOrNot() throw(int)
{
    if(this->windowTitle() == "无标题 - 记事本")
    {
        int res = QMessageBox::question(this, "记事本", "是否将文件保存至 无标题",
                                                           QMessageBox::Save|
                                                           QMessageBox::No|
                                                           QMessageBox::Cancel);    //提示是否保存...
        switch(res)
        {
        case QMessageBox::Save:
            if(!slotSaveas())
                throw res;
            else
                break;
        case QMessageBox::No:
            break;
        case QMessageBox::Cancel:
            throw res;
        }
    }
    else
    {
        int res = QMessageBox::question(this, "记事本","是否将更改保存至"+
                                                          _currentFile->absoluteFilePath(),
                                                                      QMessageBox::Save |
                                                                      QMessageBox::No |
                                                                      QMessageBox::Cancel);
        switch(res)
        {
        case QMessageBox::Save:
            /*将QTextEdit导入当前文件.*/
            _editArea.outputFile(_currentFile->absoluteFilePath());
            break;
        case QMessageBox::No:
            break;
        case QMessageBox::Cancel:
            throw res;
        }
    }
}

//打印.
void NotePad::slotPrintFile()
{
    QPrintDialog dlg;
    connect(&dlg, SIGNAL(accepted(QPrinter*)),this, SLOT(slotAccepted(QPrinter*)));
    dlg.exec();
}
void NotePad::slotAccepted(QPrinter *printer)
{
    QPainter p(printer);
    _editArea.render(&p);
}

//页面设置.
void NotePad::slotSetPage()
{
    QPrinter printer(QPrinter::ScreenResolution);
    QPageSetupDialog pageDlg(&printer);
    pageDlg.exec();
}

//字体设置.
void NotePad::soltSetFontAction()
{
    bool accepted(false);
    QFont font = QFontDialog::getFont(&accepted, _editArea.font(), this, "字体", QFontDialog::ProportionalFonts);
    if(accepted)
    {
        _editArea.setFont(font);
    }
}

//将行列信息显示在状态栏中.
void NotePad::slotCursorPositionChanged()
{
    _statusBar.showColumnAndBlock(
                _editArea.textCursor().blockNumber() + 1,
                _editArea.textCursor().columnNumber() + 1);
}

//显示状态栏.
void NotePad::slotShowStatus()
{
    if(_status->isChecked())
    {
        _statusBar.show();
    }
    else
    {
        _statusBar.hide();
    }
}

//自动换行选项.
void NotePad::soltSetWordWrap()
{
    if(_setWordWarp->isChecked())
    {
        _editArea.setLineWrapMode(QTextEdit::WidgetWidth);              //TextEdit设置自动换行
        _editArea.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //水平滚动条设置为不可见.
        _status->setEnabled(false);                                     //QAction状态栏设置为不可选项.
        _statusBar.hide();                                              //状态栏隐藏.
        _editArea.setJumpEnable(false);                                 //QAction"跳转"选项设置不可选.
    }
    else
    {
        _editArea.setLineWrapMode(QTextEdit::NoWrap);                   //TextEdit设置非自动换行
        _editArea.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);  //水平滚动条设置为可见.
        _status->setEnabled(true);                                      //QAction状态栏设置为可选项.
        _editArea.setJumpEnable(true);                                  //QAction"跳转"选项设置可选.
        //如果状态栏已经被选中,显示状态栏.
        if(_status->isChecked())
        {
            _statusBar.show();
        }
    }
}

//显示About.
void NotePad::slotShowAbout()
{
    QMessageBox::aboutQt(this, tr("Qt 制作"));
}

//窗口关闭前检查判断是否保存.
void NotePad::closeEvent(QCloseEvent *event)
{
    if(_editArea.document()->isModified())
    {
        try {
            questionSaveOrNot();
            event->accept();
        }
        catch(int) {
            event->ignore();
        }
    }
}

void NotePad::loadText()
{
    const QStringList &list = QCoreApplication::arguments();

    //qDebug() << list.count() << list.at(0);
    if(list.count() == 2)
    {
        _currentFile->setFile(list.at(1));
        _editArea.inputFile(list.at(1));
        this->setWindowTitle(_currentFile->fileName() + tr(" - 记事本"));
    }
}

NotePad::~NotePad()
{
    delete _currentFile;
}
