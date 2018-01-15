#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <QTextCursor>
#include <QFileInfo>
#include <QEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QDateTime>
#include "SearchDialog.h"
#include "JumpDialog.h"


class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEdit(QWidget *parent = 0);
    ~TextEdit();

    void inputFile(QString);
    void outputFile(QString);

    QMenu *getEditMenu()
    {
        return _editMenu;
    }

    void setJumpEnable(bool);

protected:

    QMenu *_editMenu;
    QAction *_undoAction;
    QAction *_cutAction;
    QAction *_copyAction;
    QAction *_pasteAction;
    QAction *_deleteAction;
    QAction *_searchAction;
    QAction *_searchNextAction;
    QAction *_jumpAction;
    QAction *_selectAllAction;
    QAction *_datetimeAction;

    SearchDialog _searchDialog;
    JumpDialog _jumpDialog;

    void createEditMenum();

    void bulidConnects();

private slots:
    void slotCopyAvailable(bool);

    void slotUndoAvailable(bool);

    void slotDeleteSeletion();

    void slotCopy();

    void slotDateTime();

    void slotSearchText(QString text, bool isUp, bool isSensitive);

    void slotTextChanged();

    void slotJumpToline(int);
};

#endif // TEXTEDIT_H
