#include "ASLauncher.h"
using namespace std;

HWND g_HWND = NULL;
std::map<std::string, int> mPID;

void ASLauncher::PrintProcessNameAndID(DWORD processID)
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    TCHAR szModName[MAX_PATH];

    // Get a handle to the process.
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processID);

    // Get the process name.
    if (NULL != hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
            &cbNeeded))
        {
            // get basename
            GetModuleBaseName(hProcess, hMod, szProcessName,
                sizeof(szProcessName) / sizeof(TCHAR));

            // Get the full path to the module's file.
            GetModuleFileNameEx(hProcess, hMod, szModName,
                sizeof(szModName) / sizeof(TCHAR));

            // remember the PID if the file is ashome
            std::vector<char> buffer;
            std::string s = szModName;
            std::string ssub = "ashome";

            std::transform(s.begin(), s.end(), s.begin(),
                [](unsigned char c) { return std::tolower(c); });

            if (s.find(ssub) != string::npos) {
                mPID.insert({ s, processID });
            //    _tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);
            //    _tprintf(TEXT("%s\n\n"), szModName);
            }

            // remember PID if autostainnerrun
            ssub = "autostainerrun";
            if (s.find(ssub) != string::npos) {
                mPID.insert({ s, processID });
            //    _tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);
            //    _tprintf(TEXT("%s\n\n"), szModName);
            }
        }
    }

    CloseHandle(hProcess);
}

BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM lParam)
{
    DWORD lpdwProcessId;
    GetWindowThreadProcessId(hwnd, &lpdwProcessId);
    if (lpdwProcessId == lParam)
    {
        g_HWND = hwnd;
        return FALSE;
    }
    return TRUE;
}

bool ASLauncher::FocusWindow(char* handleBuffer){
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    // get processes running
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return false;
    }
    cProcesses = cbNeeded / sizeof(DWORD);
    // for each process, check name and ID
    for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            PrintProcessNameAndID(aProcesses[i]);
        }
    }

    // everything to lower thanks
    std::string bbuf = handleBuffer;
    std::transform(bbuf.begin(), bbuf.end(), bbuf.begin(),
        [](unsigned char c) { return std::tolower(c); });

    int nPID = 0;
    if (mPID.find(bbuf) != mPID.end()) {
        nPID = mPID[bbuf];
    }

    // popup the window, only works if window isnt minimized or hidden
    EnumWindows(EnumWindowsProcMy, nPID);

    // you apparently need to alt tab cuz otherwise it will flash and not let you do that
    bool pressed = false;
    if ((GetAsyncKeyState(VK_MENU) & 0x8000) == 0) {
        pressed = true;
        keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
    }

    SetForegroundWindow(g_HWND);
    SetFocus(g_HWND);

    if (pressed) {
        keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    }

    return true;
}

Napi::Boolean ASLauncher::focusWrapped(const Napi::CallbackInfo& info){
    Napi::Env env = info.Env();
    char buf[60];
    if (info[0].IsString()) {
        Napi::String s;
        sprintf(buf, "%s", info[0].ToString().Utf8Value().c_str());
    }

    Napi::Boolean ret = Napi::Boolean::New(env, ASLauncher::FocusWindow(buf));
    return ret;
}

Napi::Object ASLauncher::Init(Napi::Env env, Napi::Object exports) {
    //export Napi function
    exports.Set("FocusWindow", Napi::Function::New(env, ASLauncher::focusWrapped));
    return exports;
}