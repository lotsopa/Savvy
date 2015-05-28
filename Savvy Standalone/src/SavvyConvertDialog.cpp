#include "SavvyConvertDialog.h"
#include "SavvyAppFrame.h"

// Register all events for this dialog class
wxBEGIN_EVENT_TABLE(SavvyEditor::ConvertDialog, wxDialog)
EVT_COMBOBOX(ID_InputLang, SavvyEditor::ConvertDialog::OnInputLangSelect)
EVT_COMBOBOX(ID_OutputLang, SavvyEditor::ConvertDialog::OnOutputLangSelect)
EVT_COMBOBOX(ID_ShaderType, SavvyEditor::ConvertDialog::OnShaderTypeSelect)
EVT_BUTTON(ID_OutputDirBtn, SavvyEditor::ConvertDialog::OnOutputDirBtn)
EVT_BUTTON(ID_Save, SavvyEditor::ConvertDialog::OnSave)
EVT_BUTTON(ID_SaveConvert, SavvyEditor::ConvertDialog::OnSaveConvert)
EVT_BUTTON(ID_Cancel, SavvyEditor::ConvertDialog::OnCancel)
EVT_TEXT(ID_OutputDir, SavvyEditor::ConvertDialog::OnTextChangeOutputDir)
EVT_TEXT(ID_OutputEntry, SavvyEditor::ConvertDialog::OnTextChangeOutputEntry)
EVT_TEXT(ID_InputEntry, SavvyEditor::ConvertDialog::OnTextChangeInputEntry)
wxEND_EVENT_TABLE()

SavvyEditor::ConvertDialog::ConvertDialog(AppFrame* parent, wxWindowID id, const wxString& title, const wxPoint& pos /*= wxDefaultPosition*/, const wxSize& size /*= wxDefaultSize*/, long style /*= wxDEFAULT_DIALOG_STYLE*/, const wxString& name /*= wxDialogNameStr*/) :
wxDialog(parent, id, title, pos, size, style, name), m_Owner(parent)
{
	m_Sizer = new wxFlexGridSizer(3, wxSize(10, 10));
	
	// Input Lang
	m_InputLangOptions[0] = GLSL_4_5_STRING;
	m_InputLangOptions[1] = GLSL_4_5_NO_LOC_STRING;
	m_InputLangOptions[2] = GLSL_4_5_NO_UBO_STRING;
	m_InputLangOptions[3] = GLSL_4_5_NO_UBO_NO_LOC_STRING;
	m_InputLangOptions[4] = HLSL_5_0_STRING;
	m_InputLangText = new wxStaticText(this, wxID_ANY, "Input Language :");
	m_InputLangComboBox = new wxComboBox(this, ID_InputLang, m_Owner->GetInputLang(), wxDefaultPosition, wxDefaultSize, INPUT_LANG_CHOICES, m_InputLangOptions, wxCB_READONLY);
	m_Sizer->Add(m_InputLangText, 0, wxLEFT | wxTOP, 10);
	m_Sizer->Add(m_InputLangComboBox, 0, wxTOP, 10);
	m_Sizer->Add(new wxStaticText(this, wxID_ANY, ""));

	// Output Lang
	m_OutputLangOptions[0] = GLSL_4_5_STRING;
	m_OutputLangOptions[1] = GLSL_4_5_NO_LOC_STRING;
	m_OutputLangOptions[2] = GLSL_4_5_NO_UBO_STRING;
	m_OutputLangOptions[3] = GLSL_4_5_NO_UBO_NO_LOC_STRING;
	m_OutputLangOptions[4] = HLSL_5_0_STRING;
	m_OutputLangText = new wxStaticText(this, wxID_ANY, "Output Language :");
	m_OutputLangComboBox = new wxComboBox(this, ID_OutputLang, m_Owner->GetOutputLang(), wxDefaultPosition, wxDefaultSize, OUTPUT_LANG_CHOICES, m_OutputLangOptions, wxCB_READONLY);
	m_OutputLangComboBox->FitInside();
	m_Sizer->Add(m_OutputLangText, 0, wxLEFT, 10);
	m_Sizer->Add(m_OutputLangComboBox, 0, 0, 0);
	m_Sizer->Add(new wxStaticText(this, wxID_ANY, ""));

	// Shader Type
	m_ShaderTypeOptions[0] = VERTEX_STRING;
	m_ShaderTypeOptions[1] = FRAGMENT_STRING;
	m_ShaderTypeText = new wxStaticText(this, wxID_ANY, "Shader Type :");
	m_ShaderTypeComboBox = new wxComboBox(this, ID_ShaderType, m_Owner->GetShaderType(), wxDefaultPosition, wxDefaultSize, SHADER_TYPE_CHOICES, m_ShaderTypeOptions, wxCB_READONLY);
	m_Sizer->Add(m_ShaderTypeText, 0, wxLEFT, 10);
	m_Sizer->Add(m_ShaderTypeComboBox, 0, 0, 0);
	m_Sizer->Add(new wxStaticText(this, wxID_ANY, ""));

	// Output Entry
	m_OutputEntryTextCtrl = new wxTextCtrl(this, ID_OutputEntry, m_Owner->GetOutputEntry(), wxDefaultPosition, wxDefaultSize);
	m_OutputEntryText = new wxStaticText(this, wxID_ANY, "Output Entry Function :");
	m_Sizer->Add(m_OutputEntryText, 0, wxLEFT, 10);
	m_Sizer->Add(m_OutputEntryTextCtrl, 0, wxEXPAND, 0);
	m_Sizer->Add(new wxStaticText(this, wxID_ANY, ""));

	// Input Entry
	m_InputEntryTextCtrl = new wxTextCtrl(this, ID_InputEntry, m_Owner->GetInputEntry(), wxDefaultPosition, wxDefaultSize);
	m_InputEntryText = new wxStaticText(this, wxID_ANY, "Input Entry Function :");
	m_Sizer->Add(m_InputEntryText, 0, wxLEFT, 10);
	m_Sizer->Add(m_InputEntryTextCtrl, 0, wxEXPAND, 0);
	m_Sizer->Add(new wxStaticText(this, wxID_ANY, ""));

	// Output Dir
	m_OutputDirTextCtrl = new wxTextCtrl(this, ID_OutputDir, m_Owner->GetOutputDir(), wxDefaultPosition, wxDefaultSize);
	m_OutputDirText = new wxStaticText(this, wxID_ANY, "Output File :");
	m_OutputFileButton = new wxButton(this, ID_OutputDirBtn, "Browse...");
	m_Sizer->Add(m_OutputDirText, 0, wxLEFT, 10);
	m_Sizer->Add(m_OutputDirTextCtrl, 0, wxEXPAND, 0);
	m_Sizer->Add(m_OutputFileButton, 0, wxEXPAND | wxRIGHT, 10);

	// Buttons
	m_SaveButton = new wxButton(this, ID_Save, "Save");
	m_SaveAndConvertButton = new wxButton(this, ID_SaveConvert, "Save && Convert");
	m_CancelButton = new wxButton(this, ID_Cancel, "Cancel");
	m_Sizer->Add(m_SaveButton, 0,  wxEXPAND | wxLEFT | wxBOTTOM, 10);
	m_Sizer->Add(m_SaveAndConvertButton, 0, wxEXPAND | wxBOTTOM, 10);
	m_Sizer->Add(m_CancelButton, 0, wxEXPAND | wxRIGHT | wxBOTTOM, 10);

	SetSizer(m_Sizer);
	Fit();
}

SavvyEditor::ConvertDialog::~ConvertDialog()
{
	
}

void SavvyEditor::ConvertDialog::OnInputLangSelect(wxCommandEvent& a_Event)
{
	m_InputLang = a_Event.GetString();
}

void SavvyEditor::ConvertDialog::OnOutputLangSelect(wxCommandEvent& a_Event)
{
	m_OutputLang = a_Event.GetString();
}

void SavvyEditor::ConvertDialog::OnOutputDirBtn(wxCommandEvent& a_Event)
{
	wxFileDialog* openDialog = new wxFileDialog(this, "Choose File", "", "", "", wxFD_OPEN);
	int response = openDialog->ShowModal();

	// If everything went well, open the file
	if (response == wxID_OK)
	{
		m_OutputDirTextCtrl->Clear();
		m_OutputDirTextCtrl->AppendText(openDialog->GetPath());
		m_OutputDir = openDialog->GetPath();
	}
	openDialog->Destroy();
}

void SavvyEditor::ConvertDialog::OnShaderTypeSelect(wxCommandEvent& a_Event)
{
	m_ShaderType = a_Event.GetString();
}

void SavvyEditor::ConvertDialog::OnTextChangeOutputDir(wxCommandEvent& a_Event)
{
	m_OutputDir = m_OutputDirTextCtrl->GetLineText(0);
}

void SavvyEditor::ConvertDialog::OnTextChangeOutputEntry(wxCommandEvent& a_Event)
{
	m_OutputEntry = m_OutputEntryTextCtrl->GetLineText(0);
}

void SavvyEditor::ConvertDialog::OnTextChangeInputEntry(wxCommandEvent& a_Event)
{
	m_InputEntry = m_InputEntryTextCtrl->GetLineText(0);
}

void SavvyEditor::ConvertDialog::OnSave(wxCommandEvent& a_Event)
{
	m_Owner->SetInputLang(m_InputLang);
	m_Owner->SetOutputLang(m_OutputLang);
	m_Owner->SetInputEntry(m_InputEntry);
	m_Owner->SetOutputEntry(m_OutputEntry);
	m_Owner->SetShaderType(m_ShaderType);
	m_Owner->SetOutputDir(m_OutputDir);
	m_Owner->ConversionOptionsSet(true);
	Close(true);
}

void SavvyEditor::ConvertDialog::OnSaveConvert(wxCommandEvent& a_Event)
{
	m_Owner->SetInputLang(m_InputLang);
	m_Owner->SetOutputLang(m_OutputLang);
	m_Owner->SetInputEntry(m_InputEntry);
	m_Owner->SetOutputEntry(m_OutputEntry);
	m_Owner->SetShaderType(m_ShaderType);
	m_Owner->SetOutputDir(m_OutputDir);
	m_Owner->ConversionOptionsSet(true);
	m_Owner->TriggerConvert();
	Close(true);
}

void SavvyEditor::ConvertDialog::OnCancel(wxCommandEvent& a_Event)
{
	Close(true);
}
