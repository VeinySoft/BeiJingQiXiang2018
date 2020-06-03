// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "FlightPathProvider.h"

goto_gis::DataProvider* gFPProvider = 0;
extern "C"
{
	__declspec(dllexport) goto_gis::DataProvider* DataProviderPluginMain()
	{
		if(gFPProvider == 0)
		{
			gFPProvider = new FlightPathProvider;
			return gFPProvider;
		}
		else
			return gFPProvider;
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

		QString strSuffix("csv");
		QString strSuffix1 = fileInfo.suffix();
		if(strSuffix1 == strSuffix)
			return true;
		else
			return false;
	}
}

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