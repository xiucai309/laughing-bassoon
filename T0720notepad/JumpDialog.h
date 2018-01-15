#ifndef JUMPDIALOG_H
#define JUMPDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>


class JumpDialog : public QDialog
{
    Q_OBJECT
public:
    JumpDialog(QWidget *parent = 0);

    QLineEdit *_lineNumber;
    QPushButton *_jumpButton;
    QPushButton *_cancelButton;

signals:
    void jumpToSelectedLine(int num);

public slots:
    void slotButtonClicked();
    void slotTextChanged(QString);
};

#endif // JUMPDIALOG_H
