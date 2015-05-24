#include "SavvyAppFrame.h"

// Register all events for this frame
wxBEGIN_EVENT_TABLE(SavvyEditor::AppFrame, wxFrame)
EVT_MENU(wxID_EXIT, SavvyEditor::AppFrame::OnExit)
EVT_MENU(wxID_ABOUT, SavvyEditor::AppFrame::OnAbout)
EVT_MENU(wxID_OPEN, SavvyEditor::AppFrame::OnFileOpen)
EVT_MENU(wxID_NEW, SavvyEditor::AppFrame::OnFileNew)
EVT_MENU(wxID_SAVE, SavvyEditor::AppFrame::OnFileSave)
wxEND_EVENT_TABLE()

SavvyEditor::AppFrame::AppFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	// Construct the File menu options
	m_FileMenu = new wxMenu();
	m_FileMenu->Append(wxID_NEW, "&New\tCtrl+N", "Create a new file");
	m_FileMenu->Append(wxID_OPEN, "&Open...\tCtrl+O", "Open a file");
	m_FileMenu->Append(wxID_SAVE, "Save\tCtrl+S", "Save File");
	m_FileMenu->Append(wxID_EXIT, "Exit\tCtrl+F4", "Exit Savvy Editor");

	// Help Menu
	m_HelpMenu = new wxMenu();
	m_HelpMenu->Append(wxID_ABOUT);

	// Create the toolbar that's going to contain the menus
	m_MenuBar = new wxMenuBar();
	m_MenuBar->Append(m_FileMenu, "&File");
	m_MenuBar->Append(m_HelpMenu, "&Help");
	SetMenuBar(m_MenuBar);

	// Create the text area
	m_TextArea = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_MULTILINE);
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
		m_TextArea->LoadFile(openDialog->GetPath());
	}
}

void SavvyEditor::AppFrame::OnFileNew(wxCommandEvent& event)
{
	wxMessageBox("TODO",
		"TODO", wxOK | wxICON_INFORMATION);
}

void SavvyEditor::AppFrame::OnFileSave(wxCommandEvent& event)
{
	wxFileDialog *saveDialog = new wxFileDialog(this, "Save File~", "", "", "Text Files (*.txt)|*.txt|C++ Files (*.cpp)|*.cpp", wxFD_SAVE);
	int response = saveDialog->ShowModal();

	// If everything went well, save the file
	if (response == wxID_OK) 
	{
		m_TextArea->SaveFile(saveDialog->GetPath());
	}

}
