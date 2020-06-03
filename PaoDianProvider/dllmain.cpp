// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "PaoDianProvider.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

goto_gis::DataProvider* gNCProvider = 0;
extern "C"
{
	__declspec(dllexport) goto_gis::DataProvider* DataProviderPluginMain()
	{
		if(gNCProvider == 0)
		{
			gNCProvider = new PaoDianProvider;
			return gNCProvider;
		}
		else
			return gNCProvider;
	}
}

extern "C"
{
	bool __declspec(dllexport) MineData(std::string& stFileName)
	{
		QString strFileName;
		strFileName = QString::fromStdString(stFileName);
		QFileInfo fileInfo;
		fileInfo.setFile(strFileName);

		QString strSuffix("bjpd");
		QString strSuffix1 = fileInfo.suffix();
		if(strSuffix1 == strSuffix)
			return true;
		else
			return false;
	}
}