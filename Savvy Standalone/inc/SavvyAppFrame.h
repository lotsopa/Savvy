/*
The MIT License (MIT)

Copyright (c) 2015 Apostol Dadachev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef SAVVY_APP_FRAME_H
#define SAVVY_APP_FRAME_H

#define DEFAULT_DOC_PATH "C:/"
#define DEFAULT_FRAME_TITLE "Savvy Standalone"
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
	class ConvertDialog;

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
			ID_ConversionOptions = 1,
			ID_TextAreaUser,
			ID_TextAreaConverted,
			ID_GLSL,
			ID_HLSL,
			ID_None,
			ID_FindDialog,
			ID_ReplaceDialog,
			ID_ConvertDialog,
			ID_Convert,
			ID_ClassicView,
			ID_SplitView
		};

		enum Margins
		{
			MARGIN_LINE_NUMBERS,
			MARGIN_FOLD
		};

		inline wxString GetCurrentDocPath() { return m_CurrDocPath; }
		inline void SetInputLang(wxString& a_Str) { m_InputLang = a_Str; }
		inline void SetOutputLang(wxString& a_Str) { m_OutputLang = a_Str; }
		inline void SetOutputDir(wxString& a_Str) { m_OutputDir = a_Str; }
		inline void SetShaderType(wxString& a_Str) { m_ShaderType = a_Str; }
		inline void SetInputEntry(wxString& a_Str) { m_InputEntry = a_Str; }
		inline void SetOutputEntry(wxString& a_Str) { m_OutputEntry = a_Str; }
		inline void ConversionOptionsSet(bool a_Val) { m_ConvertOptionsSet = a_Val; }

		inline const wxString& GetInputLang() { return m_InputLang; }
		inline const wxString& GetOutputLang() { return m_OutputLang; }
		inline const wxString& GetOutputDir() { return m_OutputDir; }
		inline const wxString& GetShaderType() { return m_ShaderType; }
		inline const wxString& GetInputEntry() { return m_InputEntry; }
		inline const wxString& GetOutputEntry() { return m_OutputEntry; }
		
		void TriggerConvert();

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
		void OnConversionOptions(wxCommandEvent& a_Event);
		void OnConvert(wxCommandEvent& a_Event);
		void OnTextChanged(wxCommandEvent& a_Event);
		void OnResize(wxSizeEvent& a_Event);
		void OnMenuOpen(wxMenuEvent& a_Event);
		void OnMarginClick(wxStyledTextEvent& a_Event);
		void EnableMenuItem(wxMenuItem* a_Item, bool a_Enable);
		void OnLangSelectGLSL(wxCommandEvent& a_Event);
		void OnLangSelectHLSL(wxCommandEvent& a_Event);
		void OnLangSelectNone(wxCommandEvent& a_Event);
		void OnShowReplaceDialog(wxCommandEvent& a_Event);
		void OnShowFindDialog(wxCommandEvent& a_event);
		void OnClassicView(wxCommandEvent& a_event);
		void OnSplitView(wxCommandEvent& a_event);
		void OnFindDialog(wxFindDialogEvent& event);
		bool DoFind(wxString& a_FindString, int a_Flags);
		bool DoReplace(wxString& a_FindString, const wxString& a_ReplaceString, int a_Flags);
		int DoReplaceAll(wxString& a_FindString, const wxString& a_ReplaceString, int a_Flags);
		void OnFocusEditorWindow(wxFocusEvent& a_Event);
		wxDECLARE_EVENT_TABLE();

		wxMenu* m_FileMenu;
		wxMenu* m_HelpMenu;
		wxMenu* m_EditMenu;
		wxMenu* m_SearchMenu;
		wxMenu* m_ConvertMenu;
		wxMenu* m_LanguageMenu;
		wxMenu* m_ViewMenu;
		wxMenuBar* m_MenuBar;
		wxStyledTextCtrl* m_TextAreaUser; // the main text area
		wxStyledTextCtrl* m_ConvertedTextArea;
		wxSplashScreen* m_SplashScreen;
		wxString m_CurrDocPath;
		wxString m_GLSLKeyWords;
		wxString m_HLSLKeyWords;
		wxString m_GLSLFuncs;
		wxString m_HLSLFuncs;
		wxFindReplaceDialog* m_ReplaceDialog;
		wxFindReplaceDialog* m_FindDialog;
		ConvertDialog* m_ConversionDialog;
		wxFindReplaceData m_FindData;
		int m_CurrFindPos;
		bool m_ConvertOptionsSet;

		// Savvy Conversion specific members/methods
		Savvy::ResultCode RegisterDefaultConverters(Savvy::ShaderConverter* converter);
		Savvy::ResultCode RegisterDefaultShaderTypes(Savvy::ShaderConverter* converter);
		Savvy::ShaderConverter* m_ShaderConverter;
		Savvy::FileConvertOptions m_FileConvertOptions;
		Savvy::BlobFileConvertOptions m_BlobFileConvertOptions;
		wxString m_InputLang;
		wxString m_OutputLang;
		wxString m_ShaderType;
		wxString m_OutputDir;
		wxString m_InputEntry;
		wxString m_OutputEntry;
		void CreateMainTextArea();
		void SetupConvertedTextArea();
		wxStyledTextCtrl* m_LastSelectedTextCtrl;

		void SetupSyntaxRules(wxStyledTextCtrl* a_Ctrl);
	};
}

#endif // !SAVVY_APP_FRAME_H