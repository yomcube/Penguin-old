#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow window;
    if (window.penguinData.settings.maintainPosition) {
        Vec2i pos = window.penguinData.settings.pos;
        window.move(pos.x, pos.y);
    }
    /*QFile file("out");
    file.open(QFile::WriteOnly);
    QDataStream outStream(&file);

    for (int i = 0; i < 0xB; i++)
        outStream << (char)0x0;*/
    window.show();
    //file.close();
    return a.exec();
}
