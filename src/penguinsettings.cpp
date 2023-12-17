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

    QObject::connect(settingsUi->askToClose, &QCheckBox::stateChanged, this, &PenguinSettings::updateSettings);
    QObject::connect(settingsUi->logLocation, &QComboBox::currentIndexChanged, this, &PenguinSettings::updateSettings);
    QObject::connect(settingsUi->maintainPosition, &QCheckBox::stateChanged, this, &PenguinSettings::updateSettings);
}


PenguinSettings::~PenguinSettings() {
    delete settingsUi;
}

void PenguinSettings::updateSettings() { 
    penguinData->settings.askToClose = settingsUi->askToClose->isChecked();
    penguinData->settings.logSite = (LogSite)settingsUi->logLocation->currentIndex();
    penguinData->settings.maintainPosition = settingsUi->maintainPosition->isChecked();
}

void PenguinSettings::updateFields() {
    settingsUi->askToClose->setChecked(penguinData->settings.askToClose);
    settingsUi->logLocation->setCurrentIndex((int)penguinData->settings.logSite);
    if (penguinData->settings.maintainPosition)
        settingsUi->maintainPosition->setCheckState(Qt::CheckState::Checked);
}

