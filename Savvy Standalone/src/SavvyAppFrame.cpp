#include "SavvyAppFrame.h"

// Register all events for this frame
wxBEGIN_EVENT_TABLE(SavvyEditor::AppFrame, wxFrame)
EVT_MENU(wxID_EXIT, SavvyEditor::AppFrame::OnExit)
EVT_MENU(wxID_ABOUT, SavvyEditor::AppFrame::OnAbout)
EVT_MENU(wxID_OPEN, SavvyEditor::AppFrame::OnFileOpen)
EVT_MENU(wxID_NEW, SavvyEditor::AppFrame::OnFileNew)
EVT_MENU(wxID_SAVE, SavvyEditor::AppFrame::OnFileSave)
EVT_MENU(wxID_SAVEAS, SavvyEditor::AppFrame::OnFileSaveAs)
EVT_MENU(wxID_CLOSE, SavvyEditor::AppFrame::OnFileClose)
EVT_SIZE(SavvyEditor::AppFrame::OnResize)
wxEND_EVENT_TABLE()

SavvyEditor::AppFrame::AppFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size), m_TextAreaUser(NULL)
{
	// Reset the current File being edited
	m_CurrDocPath = DEFAULT_DOC_PATH;

	// Construct the File menu options
	m_FileMenu = new wxMenu();
	m_FileMenu->Append(wxID_NEW, "&New\tCtrl+N", "Create a new file");
	m_FileMenu->Append(wxID_OPEN, "&Open\tCtrl+O", "Open a file");
	m_FileMenu->Append(wxID_SAVE, "&Save\tCtrl+S", "Save current file to disk...");
	m_FileMenu->Append(wxID_SAVEAS, "Save &As\tCtrl+Alt+S", "Save current file to disk as...");
	m_FileMenu->Append(wxID_CLOSE, "&Close\tCtrl+W", "Close the current file");
	m_FileMenu->Append(wxID_EXIT, "&Quit\tAlt+F4", "Quit Savvy Editor");

	// Help Menu
	m_HelpMenu = new wxMenu();
	m_HelpMenu->Append(wxID_ABOUT);

	// Create the toolbar that's going to contain the menus
	m_MenuBar = new wxMenuBar();
	m_MenuBar->Append(m_FileMenu, "&File");
	m_MenuBar->Append(m_HelpMenu, "&Help");
	SetMenuBar(m_MenuBar);

	// Create a status bar on the bottom
	CreateStatusBar();
	SetStatusText("Welcome to Savvy Editor!");
}

SavvyEditor::AppFrame::~AppFrame()
{

}

void SavvyEditor::AppFrame::OnExit(wxCommandEvent& event)
{
	bool closed = Close(true);

	if (closed)
	{
		Destroy();
	}
}

void SavvyEditor::AppFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("Savvy Editor\nVersion: 2.0\nAuthor: Apostol Dadachev",
		"About Savvy Editor", wxOK | wxICON_INFORMATION);
}

void SavvyEditor::AppFrame::OnFileOpen(wxCommandEvent& event)
{
	wxFileDialog* openDialog = new wxFileDialog(this, "Open File~", "", "", "", wxFD_OPEN);
	int response = openDialog->ShowModal();

	// If everything went well, open the file
	if (response == wxID_OK) 
	{
		CreateMainTextArea();
		m_TextAreaUser->LoadFile(openDialog->GetPath());
		m_CurrDocPath = openDialog->GetPath();
		SetTitle(openDialog->GetFilename()+ " - "DEFAULT_FRAME_TITLE);
	}
	openDialog->Destroy();
}

void SavvyEditor::AppFrame::OnFileNew(wxCommandEvent& event)
{
	CreateMainTextArea();

	// Set the Title to reflect the file open
	SetTitle("Untitled * - "DEFAULT_FRAME_TITLE);
}

void SavvyEditor::AppFrame::OnFileSave(wxCommandEvent& event)
{
	if (m_CurrDocPath != DEFAULT_DOC_PATH)
	{
		// Save to the already-set path for the document
		m_TextAreaUser->SaveFile(m_CurrDocPath);
	}
	else
	{
		// Fall-back if the file hasn't been saved before, use Save As
		OnFileSaveAs(event);
	}
}

void SavvyEditor::AppFrame::OnFileSaveAs(wxCommandEvent& event)
{
	wxFileDialog* saveDialog = new wxFileDialog(this, "Save File~", "", "", "Text Files (*.txt)|*.txt|C++ Files (*.cpp)|*.cpp|GLSL Files (*.glsl)|*.glsl|HLSL Files (*.hlsl)|*.hlsl", wxFD_SAVE);
	int response = saveDialog->ShowModal();

	// If everything went well, save the file
	if (response == wxID_OK)
	{
		m_TextAreaUser->SaveFile(saveDialog->GetPath());
		m_CurrDocPath = saveDialog->GetPath();
		SetTitle(m_CurrDocPath + " - "DEFAULT_FRAME_TITLE);
	}
	saveDialog->Destroy();
}

void SavvyEditor::AppFrame::OnFileClose(wxCommandEvent& event)
{
	// Clear the Text Box
	m_TextAreaUser->Clear();
	// Reset the current File being edited
	m_CurrDocPath = DEFAULT_DOC_PATH;
	// Set the Title to reflect the file open
	SetTitle("Untitled * - "DEFAULT_FRAME_TITLE);
}

void SavvyEditor::AppFrame::CreateMainTextArea()
{
	// Create the text area
	wxSize areaSize = GetClientSize();
	areaSize.SetWidth(areaSize.GetWidth() / 2);
	m_TextAreaUser = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, areaSize, wxTE_PROCESS_ENTER | wxTE_MULTILINE);
}

void SavvyEditor::AppFrame::OnResize(wxSizeEvent& event)
{
	if (m_TextAreaUser)
	{
		wxSize areaSize = GetClientSize();
		areaSize.SetWidth(areaSize.GetWidth() / 2);
		m_TextAreaUser->SetSize(areaSize);
	}
}
