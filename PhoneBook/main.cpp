#include "PhoneBook.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PhoneBook w;
    w.show();
    return a.exec();
}
