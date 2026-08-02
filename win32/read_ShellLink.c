#include "combaseapi.h"
#include "propidl.h"
#include <fcntl.h>
#include <io.h>
#include <stdio.h>

#include <windows.h>
#include <objbase.h>
#include <shlobj.h>
#include <propsys.h>
#include <propkey.h>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "uuid.lib")

int wmain(int argc, wchar_t* argv[])
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    if (argc < 2)
    {
        wprintf(L"Usage: %s <path_to_lnk_file>\n", argv[0]);
        return 1;
    }

    HRESULT hr;
    BOOL    comInit = FALSE;
    int     retCode = 0;

    /* init COM */
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr))
    {
        fwprintf(stderr, L"Failed to initialize COM. Error: 0x%08X\n", hr);
        comInit = TRUE;
        retCode = 1;
        goto cleanup;
    }

    /* create ShellLink */
    IShellLinkW* pShellLink = NULL;

    hr = CoCreateInstance(
        &CLSID_ShellLink,
        NULL,
        CLSCTX_INPROC_SERVER,
        &IID_IShellLinkW,
        (LPVOID*)&pShellLink
    );
    if (FAILED(hr))
    {
        fwprintf(stderr, L"Failed to create ShellLink object. Error: 0x%08X\n", hr);
        retCode = 1;
        goto cleanup;
    }

    /* get IPersistFile */
    IPersistFile* pPersistFile = NULL;

    hr = pShellLink->lpVtbl->QueryInterface(pShellLink, &IID_IPersistFile, (LPVOID*)&pPersistFile);
    if (FAILED(hr))
    {
        fwprintf(stderr, L"Failed to get IPersistFile interface. Error: 0x%08X\n", hr);
        retCode = 1;
        goto cleanup;
    }

    /* load file */
    hr = pPersistFile->lpVtbl->Load(pPersistFile, argv[1], STGM_READ);
    if (FAILED(hr))
    {
        fwprintf(stderr, L"Failed to open file: %s. Error: 0x%08X\n", argv[1], hr);
        retCode = 1;
        goto cleanup;
    }

    /* read ShellLink target path */
    wchar_t target[MAX_PATH];

    hr = pShellLink->lpVtbl->GetPath(pShellLink, target, MAX_PATH, NULL, SLGP_RAWPATH);
    if (FAILED(hr))
    {
        fwprintf(stderr, L"GetPath: Failed to get path. Error: 0x%08X\n", hr);
        retCode = 1;
    }
    else if (hr == S_FALSE)
    {
        wprintf(L"GetPath: No path retrieved\n");
    }
    else
    {
        wprintf(L"GetPath: %s\n", target);
    }

    /* get IPropertyStore */
    IPropertyStore* pPropStore = NULL;
    PROPVARIANT propVal;
    PropVariantInit(&propVal);

    hr = pShellLink->lpVtbl->QueryInterface(pShellLink, &IID_IPropertyStore, (LPVOID*)&pPropStore);
    if (FAILED(hr))
    {
        fwprintf(stderr, L"Failed to get IPropertyStore interface. Error: 0x%08X\n", hr);
        retCode = 1;
    }
    else
    {
        /* read ShellLink arguments */
        hr = pPropStore->lpVtbl->GetValue(pPropStore, &PKEY_Link_Arguments, &propVal);
        if (FAILED(hr))
        {
            fwprintf(stderr, L"GetValue: Failed to read link arguments. Error: 0x%08X\n", hr);
            retCode = 1;
        }
        else
        {
            if (propVal.vt == VT_LPWSTR && propVal.pwszVal != NULL)
            {
                wprintf(L"Arguments: %s\n", propVal.pwszVal);
            }
            else // vt == VT_EMPTY
            {
                wprintf(L"No Arguments\n");
            }
        }
    }

cleanup:
    PropVariantClear(&propVal);
    if (pShellLink)    pShellLink->lpVtbl->Release(pShellLink);
    if (pPersistFile)  pPersistFile->lpVtbl->Release(pPersistFile);
    if (pPropStore)    pPropStore->lpVtbl->Release(pPropStore);
    if (comInit)       CoUninitialize();

    return retCode;
}
