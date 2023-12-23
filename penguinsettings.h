#pragma once
#include <QMainWindow>
#include "penguin.h"
#include "gui/gui.h"

QT_BEGIN_NAMESPACE
namespace Ui {class PenguinSettings;}
QT_END_NAMESPACE

class PenguinSettings : public QDialog {
    Q_OBJECT
public:
    PenguinSettings(QWidget* parent = nullptr, PenguinData* newPenguinData = nullptr);
    ~PenguinSettings();

    void updateFields();
public slots:
    void askToClose();
    void logSite();
    void maintainPosition();
public:
    PenguinData* penguinData;
private:
    Ui::PenguinSettings* settingsUi;
};
