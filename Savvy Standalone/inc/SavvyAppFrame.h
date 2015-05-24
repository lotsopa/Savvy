#ifndef SAVVY_APP_FRAME_H
#define SAVVY_APP_FRAME_H

#include "SavvyIncludes.h"

namespace SavvyEditor
{
	/*
	The top level application frame.
	*/
	class AppFrame : public wxFrame
	{
	public:
		AppFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

		enum OptionID
		{
			ID_Hello = 1
		};

	private:
		void OnHello(wxCommandEvent& event);
		void OnExit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		wxDECLARE_EVENT_TABLE();
	};
}

#endif // !SAVVY_APP_FRAME_H