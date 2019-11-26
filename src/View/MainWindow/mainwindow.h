﻿#pragma once

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

namespace Ui {
class MainWindow;
}


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

        void             showMessageBox             (bool bErrorBox,         const std::wstring& sMessage, bool bEmitSignal = false);
        void             printOutput                (std::string errorText,  bool bEmitSignal = false);
        void             updateOnlineUsersCount     (int iNewAmount);
        void             clearChatWindow            ();


    // Ping

        void             setPingToUser              (QListWidgetItem* pListItem, int ping);


    // Users

        QListWidgetItem* addNewUserToList           (std::string userName);
        void             deleteUserFromList         (QListWidgetItem* pListItem);


    // Menu

        void             changeStartStopActionText  (bool bStop);


signals:


    // This-to-This signals

        void             signalShowMessageBox        (bool bErrorBox,              const QString& sMessage);
        void             signalSetPingToUser         (QListWidgetItem* pListItem,  int ping);
        void             signalTypeOnOutput          (QString text);
        void             signalClearChatWindow       ();

protected:

    void                 hideEvent                   (QHideEvent* event);
    void                 closeEvent                  (QCloseEvent *event);

private slots:


    // This-to-This slots

        void             slotShowMessageBox          (bool bErrorBox,              const QString& sMessage);
        void             slotSetPingToUser           (QListWidgetItem* pListItem,  int ping);
        void             typeSomeOnOutputLog         (QString text);
        void             slotClearChatWindow         ();


    // Tray Icon slot

        void             slotTrayIconActivated       ();


    // Settings Window

        void             slotApplyNewSettings        (SettingsFile* pSettingsFile);


    // Menu

        void             on_actionStart_triggered    ();
        void             on_actionAbout_2_triggered  ();
        void             on_actionSettings_triggered ();


    // Context Menu

        void on_listWidget_users_customContextMenuRequested(const QPoint &pos);
        void kickUser();

private:

    Ui::MainWindow*  ui;
    Controller*      pController;


    QMenu*           pMenuContextMenu;
    QAction*         pActionKick;


    QSystemTrayIcon* pTrayIcon;


    std::mutex       mtxPrintOutput;
};
