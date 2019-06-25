#include <iostream>
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qDebug() << "hello world" << "\n";

    return app.exec();
}

