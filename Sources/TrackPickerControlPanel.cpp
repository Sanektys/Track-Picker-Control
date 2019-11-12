//---------------------------------------------------------------------------
#include <fmx.h>
#ifdef _WIN32
#include <tchar.h>
#endif
#pragma hdrstop
#include <System.StartUpCopy.hpp>
#ifdef ANDROID
#include <Androidapi.Helpers.hpp>
#include <Androidapi.JNI.App.hpp>
#include <Androidapi.JNI.GraphicsContentViewText.hpp>
#endif
//---------------------------------------------------------------------------
USEFORM("TrackPickerControl.cpp", ConceptForm);
//---------------------------------------------------------------------------
extern "C" int FMXmain()
{
	try
	{
		#ifdef ANDROID
		SharedActivity()->getWindow()->addFlags(TJWindowManager_LayoutParams::JavaClass->FLAG_KEEP_SCREEN_ON);
        #endif
		Application->Initialize();
		Application->CreateForm(__classid(TConceptForm), &ConceptForm);
		Application->FormFactor->Orientations = TScreenOrientations() << TScreenOrientation::Portrait;
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
