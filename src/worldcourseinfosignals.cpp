#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::saveSlots_setCurrentEditorWorld() {
    if (penguinData.currentState != State_Normal || !penguinData.fileOpen) return;
    
    // first, update the editor state so we don't accidentally trigger other functions
    penguinData.currentState = State_SwitchingWorlds;

    // first save the current level index in the array
    penguinData.worldLevelIndices[penguinData.currentWorld] = ui->saveSlots_course_currentLevel->currentIndex();

    // update the world
    penguinData.currentWorld = ui->saveSlots_world_currentWorld->currentIndex();

    // change the text options of the following combo boxes to reflect that of our current world
    ui->saveSlots_course_currentLevel->clear();
    ui->saveSlots_world_toadRescue->clear();

    std::vector<std::string> levelLabels = penguinData.levelLabelArrays[penguinData.currentWorld];

    for (const std::string& label : levelLabels) {
        ui->saveSlots_course_currentLevel->addItem(QString::fromStdString(label));
        ui->saveSlots_world_toadRescue->addItem(QString::fromStdString(label));
    }

    
    ui->saveSlots_world_toadHouse->setCurrentIndex(penguinData.savedata.saveSlots[penguinData.currentSlot].startingMushroomHouseType[penguinData.currentWorld]);
    // set combo box indices to the new current level index
    ui->saveSlots_course_currentLevel->setCurrentIndex(penguinData.worldLevelIndices[penguinData.currentWorld]);
    ui->saveSlots_world_toadRescue->setCurrentIndex(penguinData.worldRescueIndices[penguinData.currentWorld]);
    // reset the state
    penguinData.currentState = State_Normal;
}

void MainWindow::saveSlots_setCurrentEditorLevel() {
    if (penguinData.currentState != State_Normal || !penguinData.fileOpen) return;

    penguinData.worldLevelIndices[penguinData.currentWorld] = ui->saveSlots_course_currentLevel->currentIndex();
}

void MainWindow::saveSlots_world_setToadHouse() {
    if (penguinData.currentState != State_Normal || !penguinData.fileOpen) return;
    penguinData.savedata.saveSlots[penguinData.currentSlot].startingMushroomHouseType[penguinData.currentWorld] = ui->saveSlots_world_toadHouse->currentIndex();
}

// see the function below for why this exists
u8 getLevel_editor(u8 world, u8 index) {
    u8 level = STAGE_1;
    // constants are used if applicable to the current index

    // all worlds have WX-5
    if (index <= STAGE_5) {
        level = index;
    } else {
        switch (world) {
            case WORLD_1: // fallthrough
            case WORLD_2: {
                // not doing nested switch statements because that gets confusing
                if (index == STAGE_6)   level = STAGE_6;
                else if (index == 6)    level = STAGE_TOWER;
                else if (index == 7)    level = STAGE_CASTLE;
                else if (index == 8)    level = STAGE_CANNON;
                else level = STAGE_1; // this should not happen 
                break;
            }
            case WORLD_3: // fallthrough
            case WORLD_5: {
                if (index == 5) level = STAGE_GHOST_HOUSE;
                else if (index == 6) level = STAGE_TOWER;
                else if (index == 7) level = STAGE_CASTLE;
                else if (index == 8) level = STAGE_CANNON;
                else level = STAGE_1;
                break;
            }
            case WORLD_4: {
                if (index == 5) level = STAGE_GHOST_HOUSE;
                else if (index == 6) level = STAGE_TOWER;
                else if (index == 7) level = STAGE_CASTLE;
                else if (index == 8) level = STAGE_DOOMSHIP;
                else if (index == 9) level = STAGE_CANNON;
                else level = STAGE_1;
                break;
            }
            case WORLD_6: {
                if (index == STAGE_6) level = STAGE_6;
                else if (index == 6) level = STAGE_TOWER;
                else if (index == 7) level = STAGE_CASTLE;
                else if (index == 8) level = STAGE_DOOMSHIP;
                else if (index == 9) level = STAGE_CANNON;
                else level = STAGE_1;
                break;
            }
            case WORLD_7: {
                if (index == STAGE_6) level = STAGE_6;
                else if (index == 6) level = STAGE_GHOST_HOUSE;
                else if (index == 7) level = STAGE_TOWER;
                else if (index == 8) level = STAGE_CASTLE;
                else level = STAGE_1;
                break;
            }
            case WORLD_8: {
                if (index == STAGE_6) level = STAGE_6;
                else if (index == STAGE_7) level = STAGE_7;
                else if (index == 7) level = STAGE_TOWER;
                else if (index == 8) level = STAGE_CASTLE;
                else if (index == 9) level = STAGE_DOOMSHIP;
                else level = STAGE_1;
                break;
            }
            case WORLD_9: {
                if (index <= STAGE_8) level = index;
                else index = STAGE_1;
                break;
            }
            default: {
                level = STAGE_1;
                break;
            }
        }
    }

    return level;
}
void MainWindow::saveSlots_world_setToadRescue() {
    if (penguinData.currentState != State_Normal || !penguinData.fileOpen) return;

    u8 world = penguinData.currentWorld;
    u8 index = ui->saveSlots_world_toadRescue->currentIndex();

    penguinData.savedata.saveSlots[penguinData.currentSlot].toadRescueLevel[penguinData.currentWorld] = getLevel_editor(world, index);
    penguinData.worldRescueIndices[penguinData.currentWorld] = ui->saveSlots_world_toadRescue->currentIndex();
}
