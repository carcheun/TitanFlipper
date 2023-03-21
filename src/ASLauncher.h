#include <napi.h>
#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>

namespace ASLauncher{
    bool FocusWindow(char* handleBuffer);
    void PrintProcessNameAndID(DWORD processID);

    Napi::Boolean focusWrapped(const Napi::CallbackInfo& info);
    //Export API
    Napi::Object Init(Napi::Env env, Napi::Object exports);
    NODE_API_MODULE(addon, Init)
}