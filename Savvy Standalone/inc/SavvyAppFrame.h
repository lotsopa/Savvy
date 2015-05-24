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
		virtual ~AppFrame();

		enum OptionID
		{
			ID_Hello = 1
		};

	private:
		void OnFileOpen(wxCommandEvent& event);
		void OnFileNew(wxCommandEvent& event);
		void OnExit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		wxDECLARE_EVENT_TABLE();

		wxMenu* m_FileMenu;
		wxMenu* m_HelpMenu;
		wxMenuBar* m_MenuBar;
	};
}

#endif // !SAVVY_APP_FRAME_H