#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "penguinsettings.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settingsWindow(new PenguinSettings)
    , penguinData{0}
    , currentFilename(QString())
{
    ui->setupUi(this);
    settingsWindow->penguinData = &this->penguinData;
    {
        QFile file("penguin_settings.bin");
        if (file.exists()) {
            file.open(QIODevice::ReadOnly);
            file.read(reinterpret_cast<char*>(&penguinData.settings), sizeof(PenguinData::settings));
            file.close();
        }
    }
    settingsWindow->updateFields();
    // connect stuffs
    /* penguin */
    QObject::connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::openSettings);

    /* file i/o */
    QObject::connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    QObject::connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveFile);
    QObject::connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::saveFileAs);

    /* header */
    //QObject::connect(ui->mainTab)
    QObject::connect(ui->header_region, &QComboBox::currentIndexChanged, this, &MainWindow::header_setRegion);
    QObject::connect(ui->header_recentSlot, &QComboBox::currentIndexChanged, this, &MainWindow::header_setRecentSlot);
    QObject::connect(ui->header_unlockedWorlds, &QListWidget::itemSelectionChanged, this, &MainWindow::header_setUnlockedWorlds);
}

MainWindow::~MainWindow() {
    delete ui;
}

/* penguin */

void MainWindow::openSettings() {
    // read settings
    QFile file("penguin_settings.bin");
    if (file.exists()) {
        //displayInfo("something");
        file.open(QIODevice::ReadOnly);
        file.read(reinterpret_cast<char*>(&penguinData.settings), sizeof(PenguinData::settings));
        file.close();
    }

    settingsWindow->updateFields();
    settingsWindow->exec();

    // save settings after closing the settings window
    file.open(QIODevice::WriteOnly);
    file.write(reinterpret_cast<char*>(&penguinData.settings), sizeof(PenguinData::settings));
    file.close();
}

void MainWindow::displayInfo(QString str, int type) {
    if (this->penguinData.settings.logSite == LogSite::Log_Window) {
        switch (type) {
            case DisplayInfoType::DIT_Information: {
                QMessageBox::information(this, "Penguin", str);
                break;
            }
            case DisplayInfoType::DIT_Warning: {
                QMessageBox::warning(this, "Penguin", str);
                break;
            }
            case DisplayInfoType::DIT_Question: {
                QMessageBox::question(this, "Penguin", str);
                break;
            }
            case DisplayInfoType::DIT_Error: {
                QMessageBox::critical(this, "Penguin", str);
                break;
            }
            default: {
                // do nothing?
            }
        }
    } else if (penguinData.settings.logSite == LogSite::Log_StatusBar) {
        // status bar logging
    }
}

void MainWindow::loadFields() {
    /* header */
    for (u32 i = 0; i < sizeof(regionIndicators); i++) {
        if (penguinData.savedata.header.magic[3] == regionIndicators[i]) {
            ui->header_region->setCurrentIndex(i);
        }
    }

    ui->header_recentSlot->setCurrentIndex(penguinData.savedata.header.lastSelectedFile);

    for (int i = 0; i < ui->header_unlockedWorlds->count(); i++) {
        u16 x = PenguinData::swapEndianness16(penguinData.savedata.header.extraModesUnlockedWorlds);
        if ((x & (1 << i)))
            ui->header_unlockedWorlds->item(i)->setCheckState(Qt::CheckState::Checked);
        else
            ui->header_unlockedWorlds->item(i)->setCheckState(Qt::CheckState::Unchecked);
    }
}
/* file i/o */

bool MainWindow::openFile() {
    QString filename = QFileDialog::getOpenFileName(this, "Open an NSMBW save file.", QString(), "NSMBW save file (*.sav)");
    if (filename.isEmpty()) {
        // they probably cancelled
        displayInfo("File name is empty, or a file was not opened.", DIT_Error);
        return false;
    }
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite)) {
        displayInfo("Couldn't open file.", DIT_Error);
        return false;
    }

    file.read(reinterpret_cast<char*>(&penguinData.savedata), sizeof(SaveData));
    file.close();
    displayInfo("Successfully opened file.", DIT_Information);
    currentFilename = filename;
    penguinData.fileOpen = true;
    penguinData.fileSaved = false;
    loadFields();
    return true;
}

bool MainWindow::saveFile(bool saveAs) {
    QString filename;
    if (saveAs) {
        filename = QFileDialog::getSaveFileName(this, "Save an NSMBW save file.", QString(), "NSMBW save file (*.sav)");
    } else {
        filename = currentFilename;
    }
    if (filename.isEmpty()) {
        // they probably cancelled
        displayInfo("File name is empty, or the file was not saved", DIT_Error);
        return false;
    }

    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite)) {
        displayInfo("Couldn't save file.", DIT_Error);
        return false;
    }

    file.write(reinterpret_cast<char*>(&penguinData.savedata), sizeof(SaveData));
    file.close();
    displayInfo("Successfully saved file.", DIT_Information);
    penguinData.fileSaved = true;
    return true;
}

bool MainWindow::saveFileAs() {
    return saveFile(true);
}

/* header */

void MainWindow::header_setRegion() {
    penguinData.savedata.header.magic[3] = regionIndicators[ui->header_region->currentIndex()];
}

void MainWindow::header_setRecentSlot() {
    penguinData.savedata.header.lastSelectedFile = ui->header_recentSlot->currentIndex();
}

void MainWindow::header_setUnlockedWorlds() {
    // not done yet
    //displayInfo("something");
    for (int i = 0; i < ui->header_unlockedWorlds->count(); i++) {
        bool checked = ui->header_unlockedWorlds->item(i)->checkState() == Qt::CheckState::Checked;
        u16 x = PenguinData::swapEndianness16(penguinData.savedata.header.extraModesUnlockedWorlds);
        PenguinData::setFlag(x, checked, i);
    }
}

