#include "mainwindow.h"
#include <QApplication>
//#include<depthparam.h>
extern int argcdep;
extern char **argvdep;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    argcdep=argc;
    argvdep=argv;
    MainWindow w;
    w.show();

    return a.exec();
}
