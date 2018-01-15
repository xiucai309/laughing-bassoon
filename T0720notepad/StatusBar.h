#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>
#include <QLabel>

class StatusBar : public QStatusBar
{
public:
    QLabel *_strachLabel;
    QLabel *_statusLabel;
    explicit StatusBar(QWidget *parent = 0);

    void showColumnAndBlock(int, int);
};

#endif // STATUSBAR_H
