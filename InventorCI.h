#ifndef INVENTORCI_PROJ_H
#define INVENTORCI_PROJ_H

#include <pathcch.h>


static HRESULT GetInventorInformation();
static std::optional<CComPtr<Application>> GetInventorProcess();
static HRESULT GetProjectPath_S(WCHAR* a, size_t b);



static HRESULT TestProjectFileExists(const WCHAR* projectpath, const size_t ppathsz);

#endif