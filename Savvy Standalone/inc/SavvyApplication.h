#ifndef SAVVY_APPLICATION_H
#define SAVVY_APPLICATION_H

#include "SavvyIncludes.h"

namespace SavvyEditor
{
	class AppFrame;

	/*
	Represents the top level application class.
	*/
	class Application : public wxApp
	{
	public:
		Application();
		virtual ~Application();
		virtual bool OnInit();

		enum OptionID
		{
			ID_Timer = 1
		};

	private:
		void OnTimer(wxTimerEvent& a_Event);
		wxDECLARE_EVENT_TABLE();
		AppFrame* m_AppFrame;
		wxSplashScreen* m_SplashScreen;
		wxTimer m_SplashTimer;
	};
}

#endif // !SAVVY_EDITOR_H