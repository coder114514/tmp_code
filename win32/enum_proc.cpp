// msvc
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <psapi.h>
#include <TlHelp32.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

std::vector<DWORD> get_pids() {
    std::vector<DWORD> pids(8);
    DWORD bytes_returned;
    while (true) {
        if (!EnumProcesses(pids.data(), pids.size() * sizeof(DWORD), &bytes_returned)) {
            std::cerr << "Failed to run EnumProcesses" << std::endl;
            return {};
        }
        auto sz = bytes_returned / sizeof(DWORD);
        if (sz < pids.size()) {
            pids.resize(sz);
            break;
        }
        pids.resize(pids.size() * 2);
    }
    for (auto iter = pids.begin(); iter != pids.end(); iter++) {
        if (*iter == 0) {
            pids.erase(iter);
            break;
        }
    }
    return pids;
}

void print_proc_name_and_id(DWORD pid) {
    HANDLE proc_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);
    if (!proc_handle) {
        std::cout << "Failed to open process with pid " << pid << " with error code: " << GetLastError() << std::endl;
        return;
    }
    HMODULE mod_handle;
    DWORD bytes_returned;
    if (!EnumProcessModules(proc_handle, &mod_handle, sizeof(mod_handle), &bytes_returned)) {
        std::cout << "Failed to open module of pid " << pid << " with error code: " << GetLastError() << std::endl;
        CloseHandle(proc_handle);
        return;
    }
    wchar_t proc_name[MAX_PATH+1]; // +1 for \0
    GetModuleBaseNameW(proc_handle, mod_handle, proc_name, MAX_PATH);
    std::wcout << pid << " -> " << proc_name << std::endl;
}

// will get a bunch of "access denied"
// void print_all_procs() {
//     auto pids = get_pids();
//     std::sort(pids.begin(), pids.end());
//     for (auto pid : pids) {
//         print_proc_name_and_id(pid);
//     }
// }

// better
void print_all_procs() {
    PROCESSENTRY32 proc_info;
    proc_info.dwSize = sizeof(proc_info);
    HANDLE proc_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (proc_snapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "Cannot get a process snapshot. GetLastError: " << GetLastError() << std::endl;
        return;
    }
    std::map<DWORD, std::wstring> procs;
    for (BOOL ok = Process32First(proc_snapshot, &proc_info); ok; ok = Process32Next(proc_snapshot, &proc_info)) {
        procs[proc_info.th32ProcessID] = proc_info.szExeFile;
    }
    if (GetLastError() != ERROR_NO_MORE_FILES) {
        std::cerr << "Error when enumerating processes. GetLastError: " << GetLastError() << std::endl;
        return;
    }
    for (const auto& proc : procs) {
        std::wcout << proc.first << " -> " << proc.second << std::endl;
    }
    CloseHandle(proc_snapshot);
}

int main() {
    print_all_procs();
    return 0;
}
