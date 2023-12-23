#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "penguinsettings.h"
#include <QMessageBox>
#include <fstream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settingsWindow(new PenguinSettings)
    , penguinData{0}
    , currentFilename(QString())
{
    ui->setupUi(this);
    /* settings */
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

    // i realise now that the slots being linked below get triggered
    // whenever the fields are updated (regardless of whether it was
    // the program or the user), but for most functions it shouldn't matter much.
    // the only ones i have to watch out for are QListWidgets.

    /* penguin */

    QObject::connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::openSettings);

    /* file i/o */
    QObject::connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    QObject::connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveFile);
    QObject::connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::saveFileAs);

    /* header */
    QObject::connect(ui->header_region, &QComboBox::currentIndexChanged, this, &MainWindow::header_setRegion);
    QObject::connect(ui->header_recentSlot, &QComboBox::currentIndexChanged, this, &MainWindow::header_setRecentSlot);
    QObject::connect(ui->header_unlockedWorlds, &QListWidget::itemChanged, this, &MainWindow::header_setUnlockedWorlds);

    /* save slots */
    QObject::connect(ui->saveSlots_currentSlot, &QComboBox::currentIndexChanged, this, &MainWindow::saveSlots_setCurrentSlot);
    QObject::connect(ui->saveSlots_currentPlayer, &QComboBox::currentIndexChanged, this, &MainWindow::saveSlots_setCurrentPlayer);

    QObject::connect(ui->levelScore, &QSpinBox::valueChanged, this, &MainWindow::saveSlots_setLevelScore);
    QObject::connect(ui->creditsScore, &QSpinBox::valueChanged, this, &MainWindow::saveSlots_setStaffCreditsScore);
    QObject::connect(ui->saveSlots_gameCompletionFlags, &QListWidget::itemChanged, this, &MainWindow::saveSlots_setGameCompletion);
    QObject::connect(ui->saveSlots_currentWorld, &QComboBox::currentIndexChanged, this, &MainWindow::saveSlots_setCurrentWorld);
    QObject::connect(ui->saveSlots_currentSubworld, &QSpinBox::valueChanged, this, &MainWindow::saveSlots_setCurrentSubworld);
    QObject::connect(ui->saveSlots_currentPathNode, &QSpinBox::valueChanged, this, &MainWindow::saveSlots_setCurrentPathNode);
    QObject::connect(ui->saveSlots_w3SwitchOn, &QCheckBox::stateChanged, this, &MainWindow::saveSlots_setW3SwitchOn);
    QObject::connect(ui->saveSlots_w5VineReshuffleCounter, &QSpinBox::valueChanged, this, &MainWindow::saveSlots_setW5VineReshuffleCounter);


}

MainWindow::~MainWindow() {
    delete ui;


    std::fstream file("penguin_settings.bin", std::ios::out | std::ios::binary);
    if (file.is_open()) {
        penguinData.settings.pos.x = this->x();
        penguinData.settings.pos.y = this->y();
        file.write(reinterpret_cast<char*>(&penguinData.settings), sizeof(PenguinData::settings));
        file.close();
    }
    // if the file doesn't exist, do nothing; the user doesn't care about settings so why make it
}

/* penguin */

void MainWindow::openSettings() {
    // read settings
    QFile file("penguin_settings.bin");
    if (file.exists()) {
        file.open(QIODevice::ReadOnly);
        file.read(reinterpret_cast<char*>(&penguinData.settings), sizeof(PenguinData::settings));
        file.close();
    }

    settingsWindow->updateFields();
    settingsWindow->exec();
    settingsWindow->updateFields();
    // save settings after closing the settings window
    file.open(QIODevice::WriteOnly);
    file.write(reinterpret_cast<char*>(&penguinData.settings), sizeof(PenguinData::settings));
    //displayInfo(QString::number(penguinData.settings.maintainPosition));
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
        u16 flags = penguinData.savedata.header.extraModesUnlockedWorlds;
        if ((flags & (1 << i)))
            ui->header_unlockedWorlds->item(i)->setCheckState(Qt::CheckState::Checked);
        else
            ui->header_unlockedWorlds->item(i)->setCheckState(Qt::CheckState::Unchecked);
    }

    /* save slots */
    ui->levelScore->setValue(penguinData.savedata.saveSlots[penguinData.currentSlot].score);
    ui->creditsScore->setValue(penguinData.savedata.saveSlots[penguinData.currentSlot].staffCreditsHighScore);

    for (int i = 0; i < ui->saveSlots_gameCompletionFlags->count(); i++) {
        u8 flags = penguinData.savedata.saveSlots[penguinData.currentSlot].gameCompletion;
        if ((flags & (1 << i)))
            ui->saveSlots_gameCompletionFlags->item(i)->setCheckState(Qt::CheckState::Checked);
        else
            ui->saveSlots_gameCompletionFlags->item(i)->setCheckState(Qt::CheckState::Unchecked);
    }

    ui->saveSlots_currentWorld->setCurrentIndex(penguinData.savedata.saveSlots[penguinData.currentSlot].currentWorld);
    ui->saveSlots_currentSubworld->setValue(penguinData.savedata.saveSlots[penguinData.currentSlot].currentSubworld);
    ui->saveSlots_currentPathNode->setValue(penguinData.savedata.saveSlots[penguinData.currentSlot].currentPathNode);
    ui->saveSlots_w3SwitchOn->setChecked(penguinData.savedata.saveSlots[penguinData.currentSlot].w3SwitchOn);
    ui->saveSlots_w5VineReshuffleCounter->setValue(penguinData.savedata.saveSlots[penguinData.currentSlot].w5VineReshuffleCounter);

    loadPlayerFields();
}

void MainWindow::loadPlayerFields() {
    ui->saveSlots_playerContinues->setValue(penguinData.savedata.saveSlots[penguinData.currentSlot].playerContinues[penguinData.currentPlayer]);
    ui->saveSlots_playerCoins->setValue(penguinData.savedata.saveSlots[penguinData.currentSlot].playerCoins[penguinData.currentPlayer]);
    ui->saveSlots_playerLives->setValue(penguinData.savedata.saveSlots[penguinData.currentSlot].playerLives[penguinData.currentPlayer]);

    for (int i = 0; i < ui->saveSlots_playerSpawnFlags->count(); i++) {
        u8 flags = penguinData.savedata.saveSlots[penguinData.currentSlot].playerSpawnFlag[penguinData.currentPlayer];
        if ((flags & (1 << i)))
            ui->saveSlots_playerSpawnFlags->item(i)->setCheckState(Qt::CheckState::Checked);
        else
            ui->saveSlots_playerSpawnFlags->item(i)->setCheckState(Qt::CheckState::Unchecked);
    }

    ui->saveSlots_playerCharacter->setCurrentIndex(penguinData.savedata.saveSlots[penguinData.currentSlot].playerCharacter[penguinData.currentPlayer]);
    ui->saveSlots_playerPowerup->setCurrentIndex(penguinData.savedata.saveSlots[penguinData.currentSlot].playerPowerup[penguinData.currentPlayer]);
}
/* file i/o */

template <typename T>
void populateArrayIn(QDataStream& inStream, T* out, int count) {
    for (int i = 0; i < count; i++) {
        inStream >> out[i];
    }
}

template <typename T>
void populateArrayOut(QDataStream& outStream, T* in, int count) {
    for (int i = 0; i < count; i++) {
        outStream << in[i];
    }
}

bool MainWindow::openFile() {

    QString filename = QFileDialog::getOpenFileName(this, "Open an NSMBW save file.", QString(), "NSMBW save file (*.sav)");
    if (filename.isEmpty()) {
        // they probably cancelled
        displayInfo("File name is empty, or a file was not opened.", DIT_Error);
        penguinData.currentState = State_Normal;
        return false;
    }
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        displayInfo("Couldn't open file.", DIT_Error);
        penguinData.currentState = State_Normal;
        return false;
    }

    penguinData.currentState = State_OpeningFile;

    QDataStream inStream(&file);
    inStream.setByteOrder(QDataStream::BigEndian);

    // if i figure out how to read in big endian all at once,
    // i'll make this cleaner
    {
        SaveData savedata = penguinData.savedata;
        inStream >> savedata.header.magic[0];
        inStream >> savedata.header.magic[1];
        inStream >> savedata.header.magic[2];
        inStream >> savedata.header.magic[3];

        inStream >> savedata.header.version;
        inStream >> savedata.header.lastSelectedFile;
        inStream >> savedata.header._7;
        for (int w = 0; w < WORLD_COUNT; w++) {
            for (int s = 0; s < STAGE_COUNT; s++) {
                inStream >> savedata.header.freeModePlayCount[w][s];
            }
        }

        for (int w = 0; w < WORLD_COUNT; w++) {
            for (int s = 0; s < STAGE_COUNT; s++) {
                inStream >> savedata.header.coinBattlePlayCount[w][s];
            }
        }

        inStream >> savedata.header.extraModesUnlockedWorlds;
        inStream >> savedata.header._69A;
        inStream >> savedata.header.crc32;

        for (int i = 0; i < 6; i++) {
            SaveData::SaveSlot slot;

            inStream >> slot.version;
            inStream >> slot.gameCompletion;
            inStream >> slot.currentWorld;
            inStream >> slot.currentSubworld;
            inStream >> slot.currentPathNode;
            inStream >> slot.w5VineReshuffleCounter;
            inStream >> slot.w3SwitchOn;
            inStream >> slot._8;
            populateArrayIn(inStream, slot.stockItemCount, POWERUP_COUNT);
            populateArrayIn(inStream, slot.startingMushroomHouseType, WORLD_COUNT);
            populateArrayIn(inStream, slot.playerContinues, PLAYER_COUNT);
            populateArrayIn(inStream, slot.playerCoins, PLAYER_COUNT);
            populateArrayIn(inStream, slot.playerLives, PLAYER_COUNT);
            populateArrayIn(inStream, slot.playerSpawnFlag, PLAYER_COUNT);
            populateArrayIn(inStream, slot.playerCharacter, PLAYER_COUNT);
            populateArrayIn(inStream, slot.playerPowerup, PLAYER_COUNT);
            populateArrayIn(inStream, slot.worldCompletion, WORLD_COUNT);
            for (int j = 0; j < WORLD_COUNT; j++) {
                populateArrayIn(inStream, slot.enemyRevivalCount[j], AMBUSH_ENEMY_COUNT);
            }
            inStream >> slot._64;
            inStream >> slot.staffCreditsHighScore;
            inStream >> slot.score;
            for (int j = 0; j < WORLD_COUNT; j++) {
                populateArrayIn(inStream, slot.stageCompletion[j], STAGE_COUNT);
            }
            populateArrayIn(inStream, slot.hintMovieBought, HINT_MOVIE_COUNT - 6);
            populateArrayIn(inStream, slot.toadRescueLevel, WORLD_COUNT);
            for (int j = 0; j < WORLD_COUNT; j++) {
                populateArrayIn(inStream, slot.enemySubworldNumber[j], AMBUSH_ENEMY_COUNT);
            }
            for (int j = 0; j < WORLD_COUNT; j++) {
                populateArrayIn(inStream, slot.enemyPosIndex[j], AMBUSH_ENEMY_COUNT);
            }
            for (int j = 0; j < WORLD_COUNT; j++) {
                populateArrayIn(inStream, slot.enemyWalkDirection[j], AMBUSH_ENEMY_COUNT);
            }
            for (int j = 0; j < WORLD_COUNT; j++) {
                populateArrayIn(inStream, slot.deathCount[j], STAGE_COUNT);
            }
            inStream >> slot.deathCountW3Level4Switch;
            populateArrayIn(inStream, slot.padding, 0x13);
            inStream >> slot.crc32;

            savedata.saveSlots[i] = slot;
        }
        penguinData.savedata = savedata;

    }
    file.close();
    displayInfo("Successfully opened file.", DIT_Information);
    currentFilename = filename;

    loadFields();
    penguinData.fileOpen = true;
    penguinData.fileSaved = false;
    penguinData.currentState = State_Normal;
    return true;
}

bool MainWindow::saveFile(bool saveAs) {

    if (!penguinData.fileOpen) {
        displayInfo("No file is open!", DIT_Error);
        return false;
    }
    QString filename;
    if (saveAs) {
        filename = QFileDialog::getSaveFileName(this, "Save an NSMBW save file.", QString(), "NSMBW save file (*.sav)");
    } else {
        filename = currentFilename;
    }
    if (filename.isEmpty()) {
        // they probably cancelled
        displayInfo("File name is empty, or the file was not saved.", DIT_Error);
        return false;
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        displayInfo("Couldn't save file.", DIT_Error);
        return false;
    }

    QDataStream outStream(&file);
    outStream.setByteOrder(QDataStream::ByteOrder::BigEndian);
    {
        SaveData savedata = penguinData.savedata;
        outStream << savedata.header.magic[0];
        outStream << savedata.header.magic[1];
        outStream << savedata.header.magic[2];
        outStream << savedata.header.magic[3];

        outStream << savedata.header.version;
        outStream << savedata.header.lastSelectedFile;
        outStream << savedata.header._7;
        for (int w = 0; w < WORLD_COUNT; w++) {
            for (int s = 0; s < STAGE_COUNT; s++) {
                outStream << savedata.header.freeModePlayCount[w][s];
            }
        }

        for (int w = 0; w < WORLD_COUNT; w++) {
            for (int s = 0; s < STAGE_COUNT; s++) {
                outStream << savedata.header.coinBattlePlayCount[w][s];
            }
        }

        outStream << savedata.header.extraModesUnlockedWorlds;
        outStream << savedata.header._69A;
        outStream << savedata.header.crc32;

        for (int i = 0; i < 6; i++) {
            SaveData::SaveSlot slot = penguinData.savedata.saveSlots[i];

            outStream << slot.version;
            outStream << slot.gameCompletion;
            outStream << slot.currentWorld;
            outStream << slot.currentSubworld;
            outStream << slot.currentPathNode;
            outStream << slot.w5VineReshuffleCounter;
            outStream << slot.w3SwitchOn;
            outStream << slot._8;
            populateArrayOut(outStream, slot.stockItemCount, POWERUP_COUNT);
            populateArrayOut(outStream, slot.startingMushroomHouseType, WORLD_COUNT);
            populateArrayOut(outStream, slot.playerContinues, PLAYER_COUNT);
            populateArrayOut(outStream, slot.playerCoins, PLAYER_COUNT);
            populateArrayOut(outStream, slot.playerLives, PLAYER_COUNT);
            populateArrayOut(outStream, slot.playerSpawnFlag, PLAYER_COUNT);
            populateArrayOut(outStream, slot.playerCharacter, PLAYER_COUNT);
            populateArrayOut(outStream, slot.playerPowerup, PLAYER_COUNT);
            populateArrayOut(outStream, slot.worldCompletion, WORLD_COUNT);
            for (int j = 0; j < WORLD_COUNT; j++) {
                populateArrayOut(outStream, slot.enemyRevivalCount[j], AMBUSH_ENEMY_COUNT);
            }
            outStream << slot._64;
            outStream << slot.staffCreditsHighScore;
            outStream << slot.score;
            for (int j = 0; j < WORLD_COUNT; j++) {
                populateArrayOut(outStream, slot.stageCompletion[j], STAGE_COUNT);
            }
            populateArrayOut(outStream, slot.hintMovieBought, HINT_MOVIE_COUNT - 6);
            populateArrayOut(outStream, slot.toadRescueLevel, WORLD_COUNT);
            for (int j = 0; j < WORLD_COUNT; j++) {
                populateArrayOut(outStream, slot.enemySubworldNumber[j], AMBUSH_ENEMY_COUNT);
            }
            for (int j = 0; j < WORLD_COUNT; j++) {
                populateArrayOut(outStream, slot.enemyPosIndex[j], AMBUSH_ENEMY_COUNT);
            }
            for (int j = 0; j < WORLD_COUNT; j++) {
                populateArrayOut(outStream, slot.enemyWalkDirection[j], AMBUSH_ENEMY_COUNT);
            }
            for (int j = 0; j < WORLD_COUNT; j++) {
                populateArrayOut(outStream, slot.deathCount[j], STAGE_COUNT);
            }
            outStream << slot.deathCountW3Level4Switch;
            populateArrayOut(outStream, slot.padding, 0x13 + 0x4); // i guess this doesn't work anymore even though i changed nothing?
            //slot.crc32 = PenguinData::calculateChecksum(reinterpret_cast<const char*>(&savedata.saveSlots[i].version), sizeof(SaveData::SaveSlot) - 0x4);
            //outStream << slot.crc32;

            savedata.saveSlots[i] = slot;
        }
        penguinData.savedata = savedata;
    }
    file.close();

    {
        std::fstream temp(filename.toStdString(), std::ios::in | std::ios::binary);
        if (!temp.is_open()) {
            displayInfo("further error saving. 1");
            return false;
        }
        SaveData savedata;
        temp.read((char*)&savedata, sizeof(savedata));
        savedata.header.crc32 = PenguinData::calculateChecksum(reinterpret_cast<const char*>(&savedata.header.magic[0]) + 0x4, 0x698);
        for (int i = 0; i < 6; i++)
            savedata.saveSlots[i].crc32 = PenguinData::calculateChecksum(reinterpret_cast<const char*>(&savedata.saveSlots[i].version), sizeof(SaveData::SaveSlot) - 0x4);
        penguinData.savedata = savedata;
        temp.close();

        temp.open(filename.toStdString(), std::ios::out | std::ios::binary);
        if (!temp.is_open()) {
            displayInfo("further error saving. 2");
            return false;
        }
        temp.write((char*)&savedata, sizeof(savedata));
        temp.close();
    }


    displayInfo("Successfully saved file.", DIT_Information);
    penguinData.fileSaved = true;
    return true;
}

bool MainWindow::saveFileAs() {
    return saveFile(true);
}

