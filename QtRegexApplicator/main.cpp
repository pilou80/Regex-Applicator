#include "regexapp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    regexapp w;
    w.show();
    
    return a.exec();
}
