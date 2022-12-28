#ifndef INVENTORCI_PROJ_H
#define INVENTORCI_PROJ_H


static HRESULT GetInventorInformation();
static std::optional<CComPtr<Application>> GetInventorProcess();
static BOOL GetProjectExists();

#endif