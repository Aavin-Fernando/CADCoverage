//  Defines the entry point for the console application.
//

#include "stdafx.h"
#include "InventorCI.h"

#include <libloaderapi.h>
#include <strsafe.h>
#include <fileapi.h>


// Our Entrypoint. (wmain) Note that all COM related activity (including the automatic 'release' within smart
// pointers) MUST take place BEFORE CoUnitialize(). Hence the function 'block' within which
// the smart-pointers construct and destruct (and AddRef and Release) keeping the CoUnitialize
// safely out of the way.

int _tmain(int argc, _TCHAR* argv[]) {
	HRESULT Result = NOERROR;
	Result = CoInitialize (NULL);


	WCHAR* projectpath = (WCHAR*)malloc(OUR_MAX_PATH * sizeof(WCHAR));
	size_t projectpathsize = OUR_MAX_PATH;
	Result = GetProjectPath_S(projectpath, projectpathsize);



	TestProjectFileExists(projectpath, projectpathsize);



	if (Result != NOERROR) return Result;

	if (SUCCEEDED(Result))
		Result = GetInventorInformation();

	

	// Cleanup 
	CoUninitialize(); 

	return 0;
}







/**
* @return success / what failed
* @param buf string that will contain path
* @param bufsize side of prev. string
**/
static HRESULT GetProjectPath_S(WCHAR* buf, size_t bufsize) {
	WCHAR* tmpbuf = (WCHAR*) malloc(OUR_MAX_PATH*sizeof(WCHAR));
	if (!tmpbuf) return E_OUTOFMEMORY;
	if (!GetModuleFileNameW(NULL, tmpbuf, OUR_MAX_PATH)) return E_FAIL;
	for (int i = 0; i < 3; i++)
		PathCchRemoveFileSpec(tmpbuf, OUR_MAX_PATH);
	if (buf) StringCchCopyW(buf, bufsize, tmpbuf);
	return NOERROR;
}


static HRESULT TestProjectFileExists(const WCHAR* projectpath, const size_t ppathsz) {
	WCHAR* tmpbuf = (WCHAR*)malloc(OUR_MAX_PATH * sizeof(WCHAR));
	if (!tmpbuf) return E_OUTOFMEMORY;

	if (projectpath) StringCchCopyW(tmpbuf, ppathsz, projectpath);
	StringCchCatW(tmpbuf, OUR_MAX_PATH, L"\\*.ipj");
	LPWIN32_FIND_DATAW findstruc = new WIN32_FIND_DATAW;
	HANDLE find = FindFirstFileW(tmpbuf, findstruc);

	free(tmpbuf);

	if (find == INVALID_HANDLE_VALUE) {
		_tprintf_s(_T("ERROR: Cannot find inventor project file!"));
		return E_FAIL;
	}
	else {
		return NOERROR;
	}
}



static std::optional<CComPtr<Application>> GetInventorProcess() {
	HRESULT Result = NOERROR;
	CLSID InvAppClsid;
	Result = CLSIDFromProgID(L"Inventor.Application", &InvAppClsid);
	if (FAILED(Result)) return {};


	// Try to latch onto an inventor instance, if that fails create a new instance
	CComPtr<IUnknown> pInvAppUnk;

	Result = GetActiveObject(InvAppClsid, NULL, &pInvAppUnk);
	if (FAILED(Result)) {
		_tprintf_s(_T("*** Could not get hold of an active Inventor application ***\n"));
		Result = CoCreateInstance(InvAppClsid, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IUnknown), (void**)&pInvAppUnk);
		if (FAILED(Result)) {
			_tprintf_s(_T("*** Failed to create a new Inventor application ***\n"));
			return {};
		}
	}

	CComPtr<Application> pInvApp;
	Result = pInvAppUnk->QueryInterface(__uuidof(Application), (void**)&pInvApp);
	if (FAILED(Result)) { 
		return {};
	} else { 
		return pInvApp; 
	}
}


static HRESULT GetInventorInformation() {
	HRESULT Result = NOERROR;
	
	CComPtr<Application> pInvApp = GetInventorProcess().value();


	// Display some information about Inventor
	_tprintf_s(_T("\nInventor Information: \n"));

	CComBSTR bstrCaption;
	Result = pInvApp->get_Caption(&bstrCaption);
	if (SUCCEEDED(Result))
		_tprintf_s(_T("Caption: %ls \n"), bstrCaption);

	CComBSTR bstrLanguage;
	Result = pInvApp->get_LanguageName(&bstrLanguage);
	if (SUCCEEDED(Result))
		_tprintf_s(_T("Language: %ls \n"), bstrLanguage);

	CComBSTR bstrUserName;
	Result = pInvApp->get_UserName(&bstrUserName);
	if (SUCCEEDED(Result))
		_tprintf_s(_T("User Name: %ls \n"), bstrUserName);

	CComPtr<SoftwareVersion> pSoftwareVersion;
	Result =  pInvApp->get_SoftwareVersion(&pSoftwareVersion);
	if (SUCCEEDED(Result))
	{
		CComBSTR bstrSoftVerDispName;
		Result = pSoftwareVersion->get_DisplayName(&bstrSoftVerDispName);
		if (SUCCEEDED(Result))
			_tprintf_s(_T("Software Version: %ls \n"), bstrSoftVerDispName);
	}

	CComPtr<FileLocations> pFileLocations;
	Result =  pInvApp->get_FileLocations(&pFileLocations);
	if (SUCCEEDED(Result))
	{
		CComBSTR bstrFileLocationsFile;
		Result = pFileLocations->get_FileLocationsFile(&bstrFileLocationsFile);
		if (SUCCEEDED(Result))
			_tprintf_s(_T("Active Project File: %ls \n"), bstrFileLocationsFile);
	}

	CComPtr<GeneralOptions> pGeneralOptions;
	Result =  pInvApp->get_GeneralOptions(&pGeneralOptions);
	if (SUCCEEDED(Result))
	{
		CComBSTR bstrStartupProjectFile;
		Result = pGeneralOptions->get_StartupProjectFileName(&bstrStartupProjectFile);
		if (SUCCEEDED(Result))
			_tprintf_s(_T("Startup Project File: %ls \n\n"), bstrStartupProjectFile);
	}

	VARIANT_BOOL bVisible = VARIANT_FALSE;
	bool bDone = false;
	while (!bDone)
	{
		// Obtain the 'Visible' property of the active application
		VARIANT_BOOL bVisible;
		Result = pInvApp->get_Visible(&bVisible);
		if (FAILED(Result)) return Result;
		
		wchar_t _str[MAX_PATH];

		if (bVisible != VARIANT_FALSE)
			_tcscpy_s (_str, _T("Visible"));
		else
			_tcscpy_s (_str, _T("Invisible"));

		_tprintf_s(_T("Active Inventor application is %s\nWould you like to toggle this? _: "), _str);
		_tscanf_s (_T("%ls"), _str, _MAX_PATH);
	  
		// Toggle the 'Visible' property of this application
		if (toupper (_str[0]) == _T('Y'))
			pInvApp->Visible = bVisible ? FALSE : TRUE;
		else
			bDone = TRUE;
	}

	return Result;
}




void TestInterfileRef(CComPtr<Application> pInvApp) {
	//pInvApp->file
}

