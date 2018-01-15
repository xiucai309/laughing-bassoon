#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>

class SearchDialog : public QDialog
{
    Q_OBJECT
public:
    SearchDialog(QWidget *parent = 0);

    QLabel *_findLabel;
    QLineEdit *_lineEdit;
    QPushButton *_buttonSearch;
    QPushButton *_buttonCancel;
    QCheckBox *_caseSensitive;
    QGroupBox *_groupBox;
    QRadioButton *_up;
    QRadioButton *_down;

    void closeEvent(QCloseEvent *)override;

signals:
    void searchText(QString text, bool isUp, bool isCaseSensitive);
public slots:
    void slotTextChanged(QString);
    void slotButtonSearchClicked();
    void slotFindNextText();
};

#endif // SEARCHDIALOG_H
