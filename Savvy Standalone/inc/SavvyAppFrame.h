#ifndef SAVVY_APP_FRAME_H
#define SAVVY_APP_FRAME_H

#define DEFAULT_DOC_PATH "C:/"
#define DEFAULT_FRAME_TITLE "Savvy Editor"
#define MARGIN_LINE_NUMBERS_WIDTH 50
#define MARGIN_FOLD_WIDTH 15

// Color Defines
#define C_PREPROCESSOR_COLOR wxColour(153,0,153)
#define C_STRING_COLOR wxColour(150, 0, 0)
#define C_IDENTIFIER_COLOR wxColour(40, 0, 60)
#define C_NUMBER_COLOR wxColour(140, 140, 140)
#define C_CHARACTER_COLOR wxColour(150, 0, 0)
#define C_WORD_COLOR wxColour(0, 0, 150)
#define C_WORD2_COLOR wxColour(244,122,0)
#define C_COMMENT_COLOR wxColour(0, 102, 0)
#define C_COMMENTLINE_COLOR wxColour(0, 102, 0)
#define C_COMMENTDOC_COLOR wxColour(0, 102, 0)
#define C_COMMENTDOCKEYWORD_COLOR wxColour(0, 0, 200)
#define C_COMMENTDOCKEYWORDERROR_COLOR wxColour(0, 0, 200)
#define FOLDING_COLOR wxColour(100, 100, 100)
#define C_BLOCK_BACKGROUND_COLOR wxColour(255, 255, 255)

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
			ID_TextAreaUser,
			ID_GLSL,
			ID_HLSL,
			ID_None
		};

		enum Margins
		{
			MARGIN_LINE_NUMBERS,
			MARGIN_FOLD
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
		void OnMarginClick(wxStyledTextEvent& a_Event);
		void EnableMenuItem(wxMenuItem* a_Item, bool a_Enable);
		void OnLangSelectGLSL(wxCommandEvent& a_Event);
		void OnLangSelectHLSL(wxCommandEvent& a_Event);
		void OnLangSelectNone(wxCommandEvent& a_Event);
		wxDECLARE_EVENT_TABLE();

		wxMenu* m_FileMenu;
		wxMenu* m_HelpMenu;
		wxMenu* m_EditMenu;
		wxMenu* m_ConvertMenu;
		wxMenu* m_LanguageMenu;
		wxMenuBar* m_MenuBar;
		wxStyledTextCtrl* m_TextAreaUser; // the main text area
		wxSplashScreen* m_SplashScreen;
		wxString m_CurrDocPath;
		wxString m_GLSLKeyWords;
		wxString m_HLSLKeyWords;
		wxString m_GLSLFuncs;
		wxString m_HLSLFuncs;

		void CreateMainTextArea();
	};
}

#endif // !SAVVY_APP_FRAME_H