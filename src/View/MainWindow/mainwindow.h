﻿// This file is part of the Silent Server.
// Copyright Aleksandr "Flone" Tretyakov (github.com/Flone-dnb).
// Licensed under the ZLib license.
// Refer to the LICENSE file included.

#pragma once

// Qt
#include <QMainWindow>
#include <QTimer>

// STL
#include <thread>
#include <vector>
#include <string>
#include <mutex>



class QMouseEvent;
class Controller;
class QListWidgetItem;
class QCloseEvent;
class QHideEvent;
class QSystemTrayIcon;
class SettingsFile;
class QMenu;
class QAction;
class SListItemUser;

namespace Ui {
class MainWindow;
}


#define ARCHIVE_HALF_TEXT_SLIDER_VALUE 100


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


class MainWindow : public QMainWindow
{
    Q_OBJECT


public:

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();



    // Updating / Printing / Showing

        void             showMessageBox             (bool bErrorBox,         const std::u16string& sMessage, bool bEmitSignal = false);
        void             printOutput                (std::string errorText,  bool bEmitSignal = false);
        void             updateOnlineUsersCount     (int iNewAmount);
        void             showOldText                (char16_t* pText);
        void             clearChatWindow            ();


    // Ping

        void             setPingToUser              (SListItemUser* pListItem, int ping);


    // Users

        SListItemUser*   addNewUserToList           (std::string userName);
        void             deleteUserFromList         (SListItemUser* pUser);


    // Menu

        void             changeStartStopActionText  (bool bStop);


signals:


    // This-to-This signals

        void             signalShowMessageBox        (bool bErrorBox,              const QString& sMessage);
        void             signalSetPingToUser         (SListItemUser* pListItem,  int ping);
        void             signalTypeOnOutput          (QString text);
        void             signalShowOldText           (char16_t* pText);
        void             signalClearChatWindow       ();

protected:

    void                 hideEvent                   (QHideEvent* event);
    void                 closeEvent                  (QCloseEvent *event);

private slots:


    // This-to-This slots

        void             slotShowMessageBox          (bool bErrorBox,              const QString& sMessage);
        void             slotSetPingToUser           (SListItemUser* pListItem,  int ping);
        void             typeSomeOnOutputLog         (QString text);
        void             slotShowOldText             (char16_t* pText);
        void             slotClearChatWindow         ();


    // Tray Icon slot

        void             slotTrayIconActivated       ();


    // Settings Window

        void             slotApplyNewSettings        (SettingsFile* pSettingsFile);


    // Menu

        void             on_actionStart_triggered    ();
        void             on_actionAbout_2_triggered  ();
        void             on_actionSettings_triggered ();


    // Slider on output log

        void             slotSliderMoved(int iValue);


    // Context Menu

        void on_listWidget_users_customContextMenuRequested(const QPoint &pos);
        void kickUser();

private:


    void checkTextSize();

    // --------------------------------------


    Ui::MainWindow*  ui;
    Controller*      pController;


    QMenu*           pMenuContextMenu;
    QAction*         pActionKick;


    QSystemTrayIcon* pTrayIcon;


    std::mutex       mtxPrintOutput;
    std::mutex       mtxListUsers;


    bool             bInternalTextWork;
};
