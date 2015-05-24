#ifndef SAVVY_APP_FRAME_H
#define SAVVY_APP_FRAME_H

#define DEFAULT_DOC_PATH "C:/"
#define DEFAULT_FRAME_TITLE "Savvy Editor"

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
		void OnFileSave(wxCommandEvent& event);
		void OnFileSaveAs(wxCommandEvent& event);
		void OnExit(wxCommandEvent& event);
		void OnFileClose(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void OnResize(wxSizeEvent& event);
		wxDECLARE_EVENT_TABLE();

		wxMenu* m_FileMenu;
		wxMenu* m_HelpMenu;
		wxMenuBar* m_MenuBar;
		wxTextCtrl* m_TextAreaUser; // the main text area
		wxString m_CurrDocPath;

		void CreateMainTextArea();
	};
}

#endif // !SAVVY_APP_FRAME_H