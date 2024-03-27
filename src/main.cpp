#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow window;
    if (window.penguinData.settings.maintainPosition) {
        Vec2i pos = window.penguinData.settings.pos;
        window.move(pos.x, pos.y);
    }
    window.show();
    return a.exec();
}
