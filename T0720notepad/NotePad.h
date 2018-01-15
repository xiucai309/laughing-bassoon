#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QCoreApplication>
#include <QWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QMenu>
#include <QAction>
#include <QFileInfo>
#include <QSystemTrayIcon>
#include <QPainter>
#include <QPrinter>
#include <QStringList>

#include "TextEdit.h"
#include "StatusBar.h"


class NotePad : public QMainWindow
{
    Q_OBJECT
public:
    explicit NotePad(QWidget *parent = 0);
    ~NotePad();

    QMenuBar *_pMenuBar;

    //"文件"菜单及QAction
    QMenu *_fileMenu;
    QAction *_openFile;
    QAction *_newFile;
    QAction *_saveFile;
    QAction *_saveasFile;
    QAction *_setPage;
    QAction *_printFile;
    QAction *_exit;

    //"格式"菜单及QAction.
    QMenu *_formatMenu;
    QAction *_setWordWarp;
    QAction *_setFont;

    //"查看"菜单及QAction.
    QMenu *_viewMenu;
    QAction *_status;

    //"帮助"菜单及QAction.
    QMenu *_helpMenu;
    QAction *_about;


    StatusBar _statusBar;    //定义 自定义的状态栏组件.
    TextEdit _editArea;      //定义 自定义的文本编辑栏组件.
    QFileInfo *_currentFile;

    void loadText();
    void closeEvent(QCloseEvent *) override;

protected:
    void createMenus();

    void createFileActions();

    void createFormatAction();

    void createViewAction();

    void createHelpAction();

    void createStatusBar();

    void questionSaveOrNot() throw(int);

signals:

public slots:
    void slotNewFile();

    void slotOpenFile();

    void slotSaveFile();

    bool slotSaveas();

    void slotExit();

    void slotSetPage();
    void slotPrintFile();
    void slotAccepted(QPrinter*);

    void soltSetFontAction();

    void slotCursorPositionChanged();

    void slotShowStatus();

    void soltSetWordWrap();

    void slotShowAbout();

};

#endif // NOTEPAD_H
