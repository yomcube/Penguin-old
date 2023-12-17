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

}
