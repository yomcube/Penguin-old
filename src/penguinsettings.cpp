#include "mainwindow.h"
#include "penguinsettings.h"
#include "ui_penguinsettings.h"
#include "gui/gui.h"
PenguinSettings::PenguinSettings(QWidget* parent, PenguinData* newPenguinData)
    : QDialog(parent)
    , settingsUi(new Ui::PenguinSettings)
    , penguinData(newPenguinData)
{
    settingsUi->setupUi(this);

    QObject::connect(settingsUi->askToClose, &QCheckBox::stateChanged, this, &PenguinSettings::askToClose);
    QObject::connect(settingsUi->logLocation, &QComboBox::currentIndexChanged, this, &PenguinSettings::logSite);
    QObject::connect(settingsUi->maintainPosition, &QCheckBox::stateChanged, this, &PenguinSettings::maintainPosition);
}


PenguinSettings::~PenguinSettings() {
    delete settingsUi;
}

void PenguinSettings::askToClose() {
    penguinData->settings.askToClose = settingsUi->askToClose->isChecked();
}

void PenguinSettings::logSite() {
    penguinData->settings.logSite = (LogSite)settingsUi->logLocation->currentIndex();
}

void PenguinSettings::maintainPosition() {
    penguinData->settings.maintainPosition = settingsUi->maintainPosition->isChecked();
}

void PenguinSettings::updateFields() {
    if (penguinData->settings.askToClose)
        settingsUi->askToClose->setCheckState(Qt::CheckState::Checked);
    settingsUi->logLocation->setCurrentIndex((int)penguinData->settings.logSite);
    if (penguinData->settings.maintainPosition)
        settingsUi->maintainPosition->setCheckState(Qt::CheckState::Checked);
}

