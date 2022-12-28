// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef _STDAFX_SYS_H
#define _STDAFX_SYS_H

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>

#include <objbase.h>
#include <fileapi.h>
#include <stdio.h>
#include <atlbase.h>

#pragma warning(disable:4192) // Automatic exclusion of system defined symbols
#pragma warning(disable:4278) // 'name':identifier in type library 'library' is already a macro

#include "InventorUtils.h"


#include <optional>


#endif