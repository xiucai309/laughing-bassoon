#include <QApplication>
#include "NotePad.h"
#include "JumpDialog.h"


int main(int argv, char *argc[])
{
    QApplication app(argv, argc);

    NotePad notepad;
    notepad.show();
    //JumpDialog s;
    //s.exec();

    return app.exec();
}
