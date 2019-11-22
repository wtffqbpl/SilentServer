﻿#include "settingsmanager.h"


// STL
#include <fstream>
#include <shlobj.h>

// Custom
#include "../src/View/MainWindow/mainwindow.h"
#include "../src/Model/SettingsManager/SettingsFile.h"

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


SettingsManager::SettingsManager(MainWindow* pMainWindow)
{
    this ->pMainWindow = pMainWindow;


    pCurrentSettingsFile = nullptr;
    pCurrentSettingsFile = readSettings();
}







void SettingsManager::saveSettings(SettingsFile *pSettingsFile)
{
    // Get the path to the Documents folder.

    TCHAR   my_documents[MAX_PATH];
    HRESULT result = SHGetFolderPathW( nullptr, CSIDL_PERSONAL, nullptr, SHGFP_TYPE_CURRENT, my_documents );

    if (result != S_OK)
    {
        pMainWindow ->showMessageBox(true, L"ServerService::showSettings::SHGetFolderPathW() failed. "
                                           "Can't open the settings file.");

        delete pSettingsFile;

        return;
    }




    // Open the settings file.

    std::wstring sPathToOldSettings  = std::wstring(my_documents);
    std::wstring sPathToNewSettings  = std::wstring(my_documents);
    sPathToOldSettings  += L"\\" + std::wstring(SETTINGS_NAME);
    sPathToNewSettings  += L"\\" + std::wstring(SETTINGS_NAME) + L"~";

    std::ifstream settingsFile (sPathToOldSettings, std::ios::binary);
    std::ofstream newSettingsFile;

    if ( settingsFile .is_open() )
    {
        newSettingsFile .open(sPathToNewSettings);
    }
    else
    {
        newSettingsFile .open(sPathToOldSettings);
    }




    // Write new setting to the new file.

    // Server port
    newSettingsFile .write
            ( reinterpret_cast <char*> (&pSettingsFile ->iPort), sizeof(pSettingsFile ->iPort) );

    // Allow HTML in messages
    char cAllowHTMLInMessages = pSettingsFile ->bAllowHTMLInMessages;
    newSettingsFile .write
            ( &cAllowHTMLInMessages, sizeof(cAllowHTMLInMessages) );

    // NEW SETTINGS GO HERE
    // + don't forget to update "readSettings()"




    if ( settingsFile .is_open() )
    {
        // Close files and rename the new file.

        settingsFile    .close();
        newSettingsFile .close();

        _wremove( sPathToOldSettings .c_str() );

        _wrename( sPathToNewSettings .c_str(), sPathToOldSettings .c_str() );
    }
    else
    {
        newSettingsFile .close();
    }




    // Update our 'pCurrentSettingsFile' to the new settings

    mtxUpdateSettings .lock();


    if (pCurrentSettingsFile)
    {
        delete pCurrentSettingsFile;
        pCurrentSettingsFile = nullptr;
    }

    pCurrentSettingsFile = pSettingsFile;


    mtxUpdateSettings .unlock();
}

SettingsFile *SettingsManager::getCurrentSettings()
{
    mtxUpdateSettings .lock();
    mtxUpdateSettings .unlock();

    return pCurrentSettingsFile;
}

SettingsFile *SettingsManager::readSettings()
{
    // Get the path to the Documents folder.

    TCHAR   my_documents[MAX_PATH];
    HRESULT result = SHGetFolderPathW( nullptr, CSIDL_PERSONAL, nullptr, SHGFP_TYPE_CURRENT, my_documents );

    if (result != S_OK)
    {
        pMainWindow ->showMessageBox(true, L"ServerService::showSettings::SHGetFolderPathW() failed. "
                                           "Can't open the settings file.");

        return nullptr;
    }




    // Create settings file object

    SettingsFile* pSettingsFile = new SettingsFile();





    // Open the settings file.

    std::wstring adressToSettings = std::wstring(my_documents);
    adressToSettings             += L"\\" + std::wstring(SETTINGS_NAME);

    std::ifstream settingsFile (adressToSettings, std::ios::binary);

    if ( settingsFile .is_open() )
    {
        // Read the settings.



        // Read port
        settingsFile .read( reinterpret_cast <char*> (&pSettingsFile ->iPort), sizeof(pSettingsFile ->iPort) );




        // Settings may end here
        if (settingsFile .eof())
        {
            settingsFile .close();

            saveSettings(pSettingsFile);
        }



        // Read port
        char cAllowHTMLInMessages;
        settingsFile .read( reinterpret_cast <char*> (&cAllowHTMLInMessages), sizeof(cAllowHTMLInMessages) );

        pSettingsFile ->bAllowHTMLInMessages = cAllowHTMLInMessages;



        settingsFile .close();
    }
    else
    {
        // The settings file does not exist.
        // Create one and write default settings.


        saveSettings(pSettingsFile);
    }



    return pSettingsFile;
}
