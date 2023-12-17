#pragma once
#include <QMainWindow>
#include "penguinsettings.h"
#include "gui/gui.h"
#include "penguin.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE
enum DisplayInfoType {
    DIT_Information,
    DIT_Warning,
    DIT_Question,
    DIT_Error
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // popups, logs, debugging
    void displayInfo(QString str, int type = DisplayInfoType::DIT_Information);
    // loads fields based on opened save file
    void loadFields();

    PenguinData penguinData;
public slots:
    /* penguin */
    // opens the settings window
    void openSettings();

    /* file i/o */
    // return true if file opening was successful
    bool openFile();
    // returns true if saving the file was successful
    bool saveFile(bool saveAs = false);
    bool saveFileAs();

    /* header */
    void header_setRegion();
    void header_setRecentSlot();
    void header_setUnlockedWorlds();

    /* save slots */
    void saveSlots_setCurrentSlot();

    void saveSlots_setLevelScore();
    void saveSlots_setStaffCreditsScore();
    void saveSlots_setGameCompletion();
private:
    Ui::MainWindow* ui;
    PenguinSettings* settingsWindow;

    QString currentFilename;
};
