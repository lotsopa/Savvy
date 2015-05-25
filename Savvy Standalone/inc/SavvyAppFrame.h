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
		AppFrame(const wxString& a_Title, const wxPoint& a_Pos, const wxSize& a_Size);
		virtual ~AppFrame();

		enum OptionID
		{
			ID_Convert = 1,
			ID_TextAreaUser
		};

	private:
		void OnFileOpen(wxCommandEvent& a_Event);
		void OnFileNew(wxCommandEvent& a_Event);
		void OnFileSave(wxCommandEvent& a_Event);
		void OnFileSaveAs(wxCommandEvent& a_Event);
		void OnExit(wxCommandEvent& a_Event);
		void OnFileClose(wxCommandEvent& a_Event);
		void OnAbout(wxCommandEvent& a_Event);
		void OnUndo(wxCommandEvent& a_Event);
		void OnRedo(wxCommandEvent& a_Event);
		void OnCut(wxCommandEvent& a_Event);
		void OnCopy(wxCommandEvent& a_Event);
		void OnPaste(wxCommandEvent& a_Event);
		void OnDelete(wxCommandEvent& a_Event);
		void OnSelectAll(wxCommandEvent& a_Event);
		void OnConvert(wxCommandEvent& a_Event);
		void OnTextChanged(wxCommandEvent& a_Event);
		void OnResize(wxSizeEvent& a_Event);
		void OnMenuOpen(wxMenuEvent& a_Event);
		void EnableMenuItem(wxMenuItem* a_Item, bool a_Enable);
		wxDECLARE_EVENT_TABLE();

		wxMenu* m_FileMenu;
		wxMenu* m_HelpMenu;
		wxMenu* m_EditMenu;
		wxMenu* m_ConvertMenu;
		wxMenuBar* m_MenuBar;
		wxStyledTextCtrl* m_TextAreaUser; // the main text area
		wxString m_CurrDocPath;

		void CreateMainTextArea();
	};
}

#endif // !SAVVY_APP_FRAME_H