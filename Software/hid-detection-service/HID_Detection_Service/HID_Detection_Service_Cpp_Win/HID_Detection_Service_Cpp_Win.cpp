// HID_Detection_Service_Cpp_Win.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include <zmq.hpp>
//#include <thread>
//#include <string>
//#include <list>
//#include <algorithm>
//#include <hidapi.h>
#if defined(_WIN32)
#include <Windows.h>
//#include <Dbt.h>
#endif
using namespace std;

//std::list<struct hid_device_info> get_hid();

int main()
{
    // Program
    cout << "USB Device Lister." << endl;

    // Get Number Of Devices
    UINT nDevices = 0;
    GetRawInputDeviceList(NULL, &nDevices, sizeof(RAWINPUTDEVICELIST));

    // Got Any?
    if (nDevices < 1)
    {
        // Exit
        cout << "ERR: 0 Devices?";
        cin.get();
        return 0;
    }

    // Allocate Memory For Device List
    PRAWINPUTDEVICELIST pRawInputDeviceList;
    pRawInputDeviceList = new RAWINPUTDEVICELIST[sizeof(RAWINPUTDEVICELIST) * nDevices];

    // Got Memory?
    if (pRawInputDeviceList == NULL)
    {
        // Error
        cout << "ERR: Could not allocate memory for Device List.";
        cin.get();
        return 0;
    }

    // Fill Device List Buffer
    int nResult;
    nResult = GetRawInputDeviceList(pRawInputDeviceList, &nDevices, sizeof(RAWINPUTDEVICELIST));

    // Got Device List?
    if (nResult < 0)
    {
        // Clean Up
        delete[] pRawInputDeviceList;

        // Error
        cout << "ERR: Could not get device list.";
        cin.get();
        return 0;
    }

    // Loop Through Device List
    for (UINT i = 0; i < nDevices; i++)
    {
        // Get Character Count For Device Name
        UINT nBufferSize = 0;
        nResult = GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice, // Device
            RIDI_DEVICENAME,                // Get Device Name
            NULL,                           // NO Buff, Want Count!
            &nBufferSize);                 // Char Count Here!

                                           // Got Device Name?
        if (nResult < 0)
        {
            // Error
            cout << "ERR: Unable to get Device Name character count.. Moving to next device." << endl << endl;

            // Next
            continue;
        }

        // Allocate Memory For Device Name
        WCHAR* wcDeviceName = new WCHAR[nBufferSize + 1];

        // Got Memory
        if (wcDeviceName == NULL)
        {
            // Error
            cout << "ERR: Unable to allocate memory for Device Name.. Moving to next device." << endl << endl;

            // Next
            continue;
        }

        // Get Name
        nResult = GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice, // Device
            RIDI_DEVICENAME,                // Get Device Name
            wcDeviceName,                   // Get Name!
            &nBufferSize);                 // Char Count

                                           // Got Device Name?
        if (nResult < 0)
        {
            // Error
            cout << "ERR: Unable to get Device Name.. Moving to next device." << endl << endl;

            // Clean Up
            delete[] wcDeviceName;

            // Next
            continue;
        }

        // Set Device Info & Buffer Size
        RID_DEVICE_INFO rdiDeviceInfo;
        rdiDeviceInfo.cbSize = sizeof(RID_DEVICE_INFO);
        nBufferSize = rdiDeviceInfo.cbSize;

        // Get Device Info
        nResult = GetRawInputDeviceInfo(pRawInputDeviceList[i].hDevice,
            RIDI_DEVICEINFO,
            &rdiDeviceInfo,
            &nBufferSize);

        // Got All Buffer?
        if (nResult < 0)
        {
            // Error
            cout << "ERR: Unable to read Device Info.. Moving to next device." << endl << endl;

            // Next
            continue;
        }

        // Mouse
        if (rdiDeviceInfo.dwType == RIM_TYPEMOUSE)
        {
            // Current Device
            cout << endl << "Displaying device " << i + 1 << " information. (MOUSE)" << endl;
            wcout << L"Device Name: " << wcDeviceName << endl;
            cout << "Mouse ID: " << rdiDeviceInfo.mouse.dwId << endl;
            cout << "Mouse buttons: " << rdiDeviceInfo.mouse.dwNumberOfButtons << endl;
            cout << "Mouse sample rate (Data Points): " << rdiDeviceInfo.mouse.dwSampleRate << endl;
            if (rdiDeviceInfo.mouse.fHasHorizontalWheel)
            {
                cout << "Mouse has horizontal wheel" << endl;
            }
            else
            {
                cout << "Mouse does not have horizontal wheel" << endl;
            }
        }

        // Keyboard
        else if (rdiDeviceInfo.dwType == RIM_TYPEKEYBOARD)
        {
            // Current Device
            cout << endl << "Displaying device " << i + 1 << " information. (KEYBOARD)" << endl;
            wcout << L"Device Name: " << wcDeviceName << endl;
            cout << "Keyboard mode: " << rdiDeviceInfo.keyboard.dwKeyboardMode << endl;
            cout << "Number of function keys: " << rdiDeviceInfo.keyboard.dwNumberOfFunctionKeys << endl;
            cout << "Number of indicators: " << rdiDeviceInfo.keyboard.dwNumberOfIndicators << endl;
            cout << "Number of keys total: " << rdiDeviceInfo.keyboard.dwNumberOfKeysTotal << endl;
            cout << "Type of the keyboard: " << rdiDeviceInfo.keyboard.dwType << endl;
        }

        // Some HID
        else // (rdi.dwType == RIM_TYPEHID)
        {
            // Current Device
            cout << endl << "Displaying device " << i + 1 << " information. (HID)" << endl;
            wcout << L"Device Name: " << wcDeviceName << endl;
            cout << "Vendor Id:" << rdiDeviceInfo.hid.dwVendorId << endl;
            cout << "Product Id:" << rdiDeviceInfo.hid.dwProductId << endl;
            cout << "Version No:" << rdiDeviceInfo.hid.dwVersionNumber << endl;
            cout << "Usage for the device: " << rdiDeviceInfo.hid.usUsage << endl;
            cout << "Usage Page for the device: " << rdiDeviceInfo.hid.usUsagePage << endl;
        }

        // Delete Name Memory!
        delete[] wcDeviceName;
    }

    // Clean Up - Free Memory
    delete[] pRawInputDeviceList;

    // Exit
    cout << endl << "Finnished.";
    cin.get();
    return 0;
}

//std::list<struct hid_device_info> get_hid() {
//    struct hid_device_info* devs, * cur_dev;
//    std::list<struct hid_device_info> d;
//    devs = hid_enumerate(0x0, 0x0);
//    cur_dev = devs;
//    while (cur_dev) {
//        d.push_back(*cur_dev);
//        cur_dev = devs->next;
//    }
//    hid_free_enumeration(devs);
//    return d;
//}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
