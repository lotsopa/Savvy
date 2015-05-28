#ifndef SAVVY_CONVERT_DIALOG_H
#define SAVVY_CONVERT_DIALOG_H
#include "SavvyIncludes.h"


#define GLSL_4_5_STRING "GLSL 4.5"
#define GLSL_4_5_NO_LOC_STRING "GLSL 4.5 - No Location"
#define GLSL_4_5_NO_UBO_STRING "GLSL 4.5 - No UBO"
#define GLSL_4_5_NO_UBO_NO_LOC_STRING "GLSL 4.5 - No UBO + No Location"
#define HLSL_5_0_STRING "HLSL 5.0"
#define VERTEX_STRING "Vertex"
#define FRAGMENT_STRING "Fragment/Pixel"


namespace SavvyEditor
{
	class AppFrame;

	class ConvertDialog : public wxDialog
	{
	public:
		ConvertDialog(AppFrame* parent, wxWindowID id,const wxString& title,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxDefaultSize,
			long style = wxDEFAULT_DIALOG_STYLE,const wxString& name = wxDialogNameStr);

		virtual ~ConvertDialog();

		enum OptionID
		{
			ID_InputLang = 1,
			ID_OutputLang,
			ID_OutputDir,
			ID_OutputDirBtn,
			ID_ShaderType,
			ID_InputEntry,
			ID_OutputEntry,
			ID_Save,
			ID_SaveConvert,
			ID_Cancel
		};

	private:
		void OnInputLangSelect(wxCommandEvent& a_Event);
		void OnOutputLangSelect(wxCommandEvent& a_Event);
		void OnShaderTypeSelect(wxCommandEvent& a_Event);
		void OnOutputDirBtn(wxCommandEvent& a_Event);
		void OnTextChangeOutputDir(wxCommandEvent& a_Event);
		void OnTextChangeOutputEntry(wxCommandEvent& a_Event);
		void OnTextChangeInputEntry(wxCommandEvent& a_Event);
		void OnSave(wxCommandEvent& a_Event);
		void OnSaveConvert(wxCommandEvent& a_Event);
		void OnCancel(wxCommandEvent& a_Event);
		wxDECLARE_EVENT_TABLE();

		wxStaticText* m_ShaderTypeText;
		wxComboBox* m_ShaderTypeComboBox;
		wxTextCtrl* m_OutputDirTextCtrl;
		wxTextCtrl* m_InputEntryTextCtrl;
		wxTextCtrl* m_OutputEntryTextCtrl;
		wxButton* m_OutputFileButton;
		wxButton* m_SaveButton;
		wxButton* m_SaveAndConvertButton;
		wxButton* m_CancelButton;
		wxStaticText* m_OutputDirText;
		wxStaticText* m_InputEntryText;
		wxStaticText* m_OutputEntryText;
		wxComboBox* m_InputLangComboBox;
		wxStaticText* m_InputLangText;
		wxComboBox* m_OutputLangComboBox;
		wxStaticText* m_OutputLangText;
		wxFlexGridSizer* m_Sizer;
		static const int INPUT_LANG_CHOICES = 5;
		wxString m_InputLangOptions[INPUT_LANG_CHOICES];
		static const int OUTPUT_LANG_CHOICES = 5;
		wxString m_OutputLangOptions[OUTPUT_LANG_CHOICES];
		static const int SHADER_TYPE_CHOICES = 2;
		wxString m_ShaderTypeOptions[SHADER_TYPE_CHOICES];
		AppFrame* m_Owner;
		wxString m_InputLang;
		wxString m_OutputLang;
		wxString m_ShaderType;
		wxString m_OutputDir;
		wxString m_InputEntry;
		wxString m_OutputEntry;
	};
}

#endif // !SAVVY_CONVERT_DIALOG_H