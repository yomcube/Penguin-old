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
    QObject::connect(ui->saveSlots_unlockedWorlds, &QListWidget::itemChanged, this, &MainWindow::saveSlots_setUnlockedWorlds);

    /* save slots -> hint movie completion */
    for (int i = 0; i < HINT_MOVIE_COUNT - 6; i++) {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(hintMovieTitles[i]));
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        ui->saveSlots_hintMovieCompletion->addItem(item);
    }
    QObject::connect(ui->saveSlots_hintMovieCompletion, &QListWidget::itemChanged, this, &MainWindow::saveSlots_setHintMovieCompletion);
    /* save slots -> player info */
    QObject::connect(ui->saveSlots_playerContinues, &QSpinBox::valueChanged, this, &MainWindow::saveSlots_setPlayerContinues);
    QObject::connect(ui->saveSlots_playerCoins, &QSpinBox::valueChanged, this, &MainWindow::saveSlots_setPlayerCoins);
    QObject::connect(ui->saveSlots_playerLives, &QSpinBox::valueChanged, this, &MainWindow::saveSlots_setPlayerLives);
    QObject::connect(ui->saveSlots_playerSpawnFlags, &QListWidget::itemChanged, this, &MainWindow::saveSlots_setPlayerSpawnFlags);
    QObject::connect(ui->saveSlots_playerCharacter, &QComboBox::currentIndexChanged, this, &MainWindow::saveSlots_setPlayerCharacter);
    QObject::connect(ui->saveSlots_playerPowerup, &QComboBox::currentIndexChanged, this, &MainWindow::saveSlots_setPlayerPowerup);

    /* save slots -> world/course info */
//    QObject::connect(ui->saveSlots_worldTabs, &QTabWidget::currentChanged, this, &MainWindow::saveSlots_setCurrentEditorWorld);
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

    for (int i = 0; i < ui->saveSlots_unlockedWorlds->count(); i++) {
        bool checked = penguinData.savedata.saveSlots[penguinData.currentSlot].worldCompletion[i];
        //displayInfo(QString::fromStdString(std::to_string(penguinData.savedata.saveSlots[penguinData.currentSlot].worldCompletion[i])));
        if (checked)
            ui->saveSlots_unlockedWorlds->item(i)->setCheckState(Qt::CheckState::Checked);
        else
            ui->saveSlots_unlockedWorlds->item(i)->setCheckState(Qt::CheckState::Unchecked);
    }

    for (int i = 0; i < ui->saveSlots_hintMovieCompletion->count(); i++) {
        bool checked = penguinData.savedata.saveSlots[penguinData.currentSlot].hintMovieBought;
        if (checked)
            ui->saveSlots_hintMovieCompletion->item(i)->setCheckState(Qt::CheckState::Checked);
        else
            ui->saveSlots_hintMovieCompletion->item(i)->setCheckState(Qt::CheckState::Unchecked);
    }



    loadPlayerFields();
}

void MainWindow::loadPlayerFields() {
    ui->saveSlots_playerContinues->setValue(penguinData.savedata.saveSlots[penguinData.currentSlot].playerContinues[penguinData.currentPlayer]);
    ui->saveSlots_playerCoins->setValue(penguinData.savedata.saveSlots[penguinData.currentSlot].playerCoins[penguinData.currentPlayer]);
    ui->saveSlots_playerLives->setValue(penguinData.savedata.saveSlots[penguinData.currentSlot].playerLives[penguinData.currentPlayer]);

    for (int i = 0; i < ui->saveSlots_playerSpawnFlags->count(); i++) {
        u8 flags = penguinData.savedata.saveSlots[penguinData.currentSlot].playerSpawnFlags[penguinData.currentPlayer];
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
    std::string filename = QFileDialog::getOpenFileName(this, "Open a New Super Mario Bros. Wii save file.", QString(), "New Super Mario Bros. Wii save file (*.sav)").toStdString();
    if (filename.empty()) {
        // probably cancelled
        displayInfo("File name is empty, or a file was not opened.", DIT_Error);
        penguinData.currentState = State_Normal;
        return false;
    }

    std::fstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        file.close();
        displayInfo("Couldn't open file.", DIT_Error);
        penguinData.currentState = State_Normal;
        return false;
    }

    penguinData.currentState = State_OpeningFile;
    SaveData savedata = penguinData.savedata;

    file.read((char*)&savedata, sizeof(SaveData));

    // swap the endianness of stuff becuase you're probably using an LE processor

    /* header */
    savedata.header.version = PenguinData::swapEndianness16(savedata.header.version);

    for (int w = 0; w < WORLD_COUNT; w++) {
        for (int s = 0; s < STAGE_COUNT; s++) {
            savedata.header.freeModePlayCount[w][s] = PenguinData::swapEndianness16(savedata.header.freeModePlayCount[w][s]);
            savedata.header.coinBattlePlayCount[w][s] = PenguinData::swapEndianness16(savedata.header.coinBattlePlayCount[w][s]);
        }
    }

    savedata.header.extraModesUnlockedWorlds = PenguinData::swapEndianness16(savedata.header.extraModesUnlockedWorlds);
    savedata.header.crc32 = PenguinData::swapEndianness32(savedata.header.crc32);

    /* save slots */
    for (int i = 0; i < 6; ++i) {
        SaveData::SaveSlot slot = savedata.saveSlots[i];

        slot.version = PenguinData::swapEndianness16(slot.version);
        slot.staffCreditsHighScore = PenguinData::swapEndianness16(slot.staffCreditsHighScore);
        slot.score = PenguinData::swapEndianness32(slot.score);
        slot.crc32 = PenguinData::swapEndianness32(slot.crc32);

        for (int w = 0; w < WORLD_COUNT; ++w) {
            for (int s = 0; s < STAGE_COUNT; ++s) {
                slot.stageCompletion[w][s] = PenguinData::swapEndianness32(slot.stageCompletion[w][s]);
            }

            for (int e = 0; e < AMBUSH_ENEMY_COUNT; ++e) {
                slot.enemyRevivalCount[w][e] = PenguinData::swapEndianness16(slot.enemyRevivalCount[w][e]);
            }
        }

        savedata.saveSlots[i] = slot;
    }


    penguinData.savedata = savedata;

    file.close();
    displayInfo("Successfully opened file.", DIT_Information);
    currentFilename = QString::fromStdString(filename);

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

    std::string filename;
    if (saveAs) {
        filename = QFileDialog::getSaveFileName(this, "Save a New Super Mario Bros. Wii save file", QString(), "New Super Mario Bros. Wii save file (*.sav)").toStdString();
    } else {
        filename = currentFilename.toStdString();
    }

    std::fstream file(filename, std::ios::out | std::ios::binary);

    if (!file.is_open()) {
        file.close();
        displayInfo("Couldn't save file.", DIT_Error);
        return false;  
    }
    SaveData savedata = penguinData.savedata;

    // swap endianness again
    savedata.header.version = PenguinData::swapEndianness16(savedata.header.version);

    for (int w = 0; w < WORLD_COUNT; w++) {
        for (int s = 0; s < STAGE_COUNT; s++) {
            savedata.header.freeModePlayCount[w][s] = PenguinData::swapEndianness16(savedata.header.freeModePlayCount[w][s]);
            savedata.header.coinBattlePlayCount[w][s] = PenguinData::swapEndianness16(savedata.header.coinBattlePlayCount[w][s]);
        }
    }

    savedata.header.extraModesUnlockedWorlds = PenguinData::swapEndianness16(savedata.header.extraModesUnlockedWorlds);
    savedata.header.crc32 = PenguinData::swapEndianness32(savedata.header.crc32);

    /* save slots */
    for (int i = 0; i < 6; ++i) {
        SaveData::SaveSlot slot = savedata.saveSlots[i];

        slot.version = PenguinData::swapEndianness16(slot.version);
        slot.staffCreditsHighScore = PenguinData::swapEndianness16(slot.staffCreditsHighScore);
        slot.score = PenguinData::swapEndianness32(slot.score);
        slot.crc32 = PenguinData::swapEndianness32(slot.crc32);

        for (int w = 0; w < WORLD_COUNT; ++w) {
            for (int s = 0; s < STAGE_COUNT; ++s) {
                slot.stageCompletion[w][s] = PenguinData::swapEndianness32(slot.stageCompletion[w][s]);
            }

            for (int e = 0; e < AMBUSH_ENEMY_COUNT; ++e) {
                slot.enemyRevivalCount[w][e] = PenguinData::swapEndianness16(slot.enemyRevivalCount[w][e]);
            }
        }

        savedata.saveSlots[i] = slot;
    }

    // calculate checksums

    savedata.header.crc32 = PenguinData::calculateChecksum((char*)&savedata.header.version, sizeof(SaveData::Header) - sizeof(savedata.header.magic) - sizeof(savedata.header.crc32));
    for (int i = 0; i < 6; i++) {
        SaveData::SaveSlot slot = savedata.saveSlots[i];
        slot.crc32 = PenguinData::calculateChecksum((char*)&slot.version, sizeof(SaveData::SaveSlot) - sizeof(slot.crc32));
        savedata.saveSlots[i] = slot;
    }

    // note to self, do not do penguinData.savedata = savedata
    // because then the values will be byteswapped *again*

    file.write((char*)&savedata, sizeof(SaveData));
    file.close();
    displayInfo("Successfully saved file.", DIT_Information);
    penguinData.fileSaved = true;
    return true;
}

bool MainWindow::saveFileAs() {
    return saveFile(true);
}

