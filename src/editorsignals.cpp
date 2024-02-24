#include "mainwindow.h"
#include "ui_mainwindow.h"

/* header */

void MainWindow::header_setRegion() {
    penguinData.savedata.header.magic[3] = regionIndicators[ui->header_region->currentIndex()];
}

void MainWindow::header_setRecentSlot() {
    penguinData.savedata.header.lastSelectedFile = ui->header_recentSlot->currentIndex();
}

void MainWindow::header_setUnlockedWorlds() {
    // if the program is in the process of opening
    // a save file, but hasn't finished, do nothing
    if (penguinData.currentState == State_OpeningFile)
        return;

    // if the file *has* been opened, assume the check boxes
    // have already been checked or unchecked accordingly.

    u16 flags = penguinData.savedata.header.extraModesUnlockedWorlds;
    for (int i = 0; i < ui->header_unlockedWorlds->count(); i++) {
        bool checked = ui->header_unlockedWorlds->item(i)->checkState() == Qt::CheckState::Checked;

        PenguinData::setFlag(flags, checked, i);
    }
    penguinData.savedata.header.extraModesUnlockedWorlds = flags;
}

/* save slots */
void MainWindow::saveSlots_setCurrentSlot() {
    penguinData.currentSlot = ui->saveSlots_currentSlot->currentIndex();
    loadFields();
}

void MainWindow::saveSlots_setCurrentPlayer() {
    penguinData.currentPlayer = ui->saveSlots_currentPlayer->currentIndex();
    loadPlayerFields();
}

void MainWindow::saveSlots_setLevelScore() {
    penguinData.savedata.saveSlots[penguinData.currentSlot].score = ui->levelScore->value();
}

void MainWindow::saveSlots_setStaffCreditsScore() {
    u16 score = ui->creditsScore->value();
    penguinData.savedata.saveSlots[penguinData.currentSlot].staffCreditsHighScore = score;
}

void MainWindow::saveSlots_setGameCompletion() {
    if (penguinData.currentState == State_OpeningFile)
        return;

    u8 flags = penguinData.savedata.saveSlots[penguinData.currentSlot].gameCompletion;
    for (int i = 0; i < ui->saveSlots_gameCompletionFlags->count(); i++) {
        bool checked = ui->saveSlots_gameCompletionFlags->item(i)->checkState() == Qt::CheckState::Checked;
        PenguinData::setFlag(flags, checked, i);
    }
    penguinData.savedata.saveSlots[penguinData.currentSlot].gameCompletion = flags;
}

void MainWindow::saveSlots_setCurrentWorld() {
    penguinData.savedata.saveSlots[penguinData.currentSlot].currentWorld = ui->saveSlots_currentWorld->currentIndex();
}

void MainWindow::saveSlots_setCurrentSubworld() {
    penguinData.savedata.saveSlots[penguinData.currentSlot].currentSubworld = ui->saveSlots_currentSubworld->value();
}

void MainWindow::saveSlots_setCurrentPathNode() {
    penguinData.savedata.saveSlots[penguinData.currentSlot].currentPathNode = ui->saveSlots_currentPathNode->value();
}

void MainWindow::saveSlots_setW3SwitchOn() {
    penguinData.savedata.saveSlots[penguinData.currentSlot].w3SwitchOn = ui->saveSlots_w3SwitchOn->isChecked();
}

void MainWindow::saveSlots_setW5VineReshuffleCounter() {
    penguinData.savedata.saveSlots[penguinData.currentSlot].w5VineReshuffleCounter = ui->saveSlots_w5VineReshuffleCounter->value();
}

void MainWindow::saveSlots_setUnlockedWorlds() {
    if (penguinData.currentState == State_OpeningFile)
        return;

    for (int i = 0; i < ui->saveSlots_unlockedWorlds->count(); i++) {
        bool checked = ui->saveSlots_unlockedWorlds->item(i)->checkState() == Qt::CheckState::Checked;
        penguinData.savedata.saveSlots[penguinData.currentSlot].worldCompletion[i] = checked;
    }
}



void MainWindow::saveSlots_setHintMovieCompletion() {
    if (penguinData.currentState == State_OpeningFile)
        return;
    
    for (int i = 0; i < ui->saveSlots_hintMovieCompletion->count(); i++) {
        bool checked = ui->saveSlots_hintMovieCompletion->item(i)->checkState() == Qt::CheckState::Checked;
        penguinData.savedata.saveSlots[penguinData.currentSlot].hintMovieBought[i] = checked;
    }
}
/* save slots -> player info */

void MainWindow::saveSlots_setPlayerContinues() {
    penguinData.savedata.saveSlots[penguinData.currentSlot].playerContinues[penguinData.currentPlayer] = ui->saveSlots_playerContinues->value();
}
void MainWindow::saveSlots_setPlayerCoins() {
    penguinData.savedata.saveSlots[penguinData.currentSlot].playerCoins[penguinData.currentPlayer] = ui->saveSlots_playerCoins->value();
}
void MainWindow::saveSlots_setPlayerLives() {
    penguinData.savedata.saveSlots[penguinData.currentSlot].playerLives[penguinData.currentPlayer] = ui->saveSlots_playerLives->value();
}
void MainWindow::saveSlots_setPlayerSpawnFlags() {
    if (penguinData.currentState == State_OpeningFile)
        return;
    u8 flags = penguinData.savedata.saveSlots[penguinData.currentSlot].playerSpawnFlags[penguinData.currentPlayer];
    for (int i = 0; i < ui->saveSlots_playerSpawnFlags->count(); i++) {
        bool checked = ui->saveSlots_playerSpawnFlags->item(i)->checkState() == Qt::CheckState::Checked;
        PenguinData::setFlag(flags, checked, i);
    }

    penguinData.savedata.saveSlots[penguinData.currentSlot].playerSpawnFlags[penguinData.currentPlayer] = flags;
}
void MainWindow::saveSlots_setPlayerCharacter() {
    penguinData.savedata.saveSlots[penguinData.currentSlot].playerCharacter[penguinData.currentPlayer] = ui->saveSlots_playerCharacter->currentIndex();
}
void MainWindow::saveSlots_setPlayerPowerup() {
    penguinData.savedata.saveSlots[penguinData.currentSlot].playerPowerup[penguinData.currentPlayer] = ui->saveSlots_playerPowerup->currentIndex();
}
