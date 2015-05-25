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
EVT_MENU(wxID_UNDO, SavvyEditor::AppFrame::OnUndo)
EVT_MENU(wxID_REDO, SavvyEditor::AppFrame::OnRedo)
EVT_MENU(wxID_CUT, SavvyEditor::AppFrame::OnCut)
EVT_MENU(wxID_COPY, SavvyEditor::AppFrame::OnCopy)
EVT_MENU(wxID_PASTE, SavvyEditor::AppFrame::OnPaste)
EVT_MENU(wxID_DELETE, SavvyEditor::AppFrame::OnDelete)
EVT_MENU(wxID_SELECTALL, SavvyEditor::AppFrame::OnSelectAll)
EVT_MENU(ID_Convert, SavvyEditor::AppFrame::OnConvert)
EVT_SIZE(SavvyEditor::AppFrame::OnResize)
EVT_TEXT(ID_TextAreaUser, SavvyEditor::AppFrame::OnTextChanged)
EVT_MENU_OPEN(SavvyEditor::AppFrame::OnMenuOpen)
wxEND_EVENT_TABLE()

SavvyEditor::AppFrame::AppFrame(const wxString& a_Title, const wxPoint& a_Pos, const wxSize& a_Size)
: wxFrame(NULL, wxID_ANY, a_Title, a_Pos, a_Size), m_TextAreaUser(NULL), m_TextAreaMargin(0), m_TextAreaWidth(30),
m_TextAreaType(1)
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
	m_FileMenu->AppendSeparator();
	m_FileMenu->Append(wxID_EXIT, "&Quit\tAlt+F4", "Quit Savvy Editor");

	// Edit Menu
	m_EditMenu = new wxMenu();
	m_EditMenu->Append(wxID_UNDO);
	m_EditMenu->Append(wxID_REDO);
	m_EditMenu->AppendSeparator();
	m_EditMenu->Append(wxID_CUT);
	m_EditMenu->Append(wxID_COPY);
	m_EditMenu->Append(wxID_PASTE);
	m_EditMenu->Append(wxID_DELETE, "&Delete\tDEL");
	m_EditMenu->Append(wxID_SELECTALL, "Select &All\tCtrl+A", "Select all the text in the document");

	// Help Menu
	m_HelpMenu = new wxMenu();
	m_HelpMenu->Append(wxID_ABOUT);

	// Convert Menu
	m_ConvertMenu = new wxMenu();
	m_ConvertMenu->Append(ID_Convert, "&Convert\tF1", "Brings up the conversion options for the file");

	// Create the toolbar that's going to contain the menus
	m_MenuBar = new wxMenuBar();
	m_MenuBar->Append(m_FileMenu, "&File");
	m_MenuBar->Append(m_EditMenu, "&Edit");
	m_MenuBar->Append(m_ConvertMenu, "&Conversion");
	m_MenuBar->Append(m_HelpMenu, "&Help");

	SetMenuBar(m_MenuBar);

	// Create a status bar on the bottom
	CreateStatusBar();

	CreateMainTextArea();
}

SavvyEditor::AppFrame::~AppFrame()
{

}

void SavvyEditor::AppFrame::OnExit(wxCommandEvent& a_Event)
{
	bool closed = Close(true);

	if (closed)
	{
		Destroy();
	}
}

void SavvyEditor::AppFrame::OnAbout(wxCommandEvent& a_Event)
{
	wxMessageBox("Savvy Editor\nVersion: 2.0\nAuthor: Apostol Dadachev",
		"About Savvy Editor", wxOK | wxICON_INFORMATION);
}

void SavvyEditor::AppFrame::OnFileOpen(wxCommandEvent& a_Event)
{
	wxFileDialog* openDialog = new wxFileDialog(this, "Open File~", "", "", "", wxFD_OPEN);
	int response = openDialog->ShowModal();

	// If everything went well, open the file
	if (response == wxID_OK) 
	{
		CreateMainTextArea();
		m_TextAreaUser->LoadFile(openDialog->GetPath());
		m_CurrDocPath = openDialog->GetPath();
		SetTitle(openDialog->GetPath() + " - "DEFAULT_FRAME_TITLE);
	}
	openDialog->Destroy();
}

void SavvyEditor::AppFrame::OnFileNew(wxCommandEvent& a_Event)
{
	// Set the Title to reflect the file open
	SetTitle("Untitled* - "DEFAULT_FRAME_TITLE);
	m_TextAreaUser->ClearAll();
}

void SavvyEditor::AppFrame::OnFileSave(wxCommandEvent& a_Event)
{
	if (m_CurrDocPath != DEFAULT_DOC_PATH)
	{
		// Save to the already-set path for the document
		m_TextAreaUser->SaveFile(m_CurrDocPath);
		SetTitle(m_CurrDocPath + " - "DEFAULT_FRAME_TITLE);
	}
	else
	{
		// Fall-back if the file hasn't been saved before, use Save As
		OnFileSaveAs(a_Event);
	}
}

void SavvyEditor::AppFrame::OnFileSaveAs(wxCommandEvent& a_Event)
{
	wxFileDialog* saveDialog = new wxFileDialog(this, "Save File~", "", "", "Text Files (*.txt)|*.txt|C++ Files (*.cpp)|*.cpp|GLSL Files (*.glsl)|*.glsl|HLSL Files (*.hlsl)|*.hlsl", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
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

void SavvyEditor::AppFrame::OnFileClose(wxCommandEvent& a_Event)
{
	// Clear the Text Box
	m_TextAreaUser->ClearAll();
	// Reset the current File being edited
	m_CurrDocPath = DEFAULT_DOC_PATH;
	// Set the Title to reflect the file open
	SetTitle("Untitled* - "DEFAULT_FRAME_TITLE);
}

void SavvyEditor::AppFrame::CreateMainTextArea()
{
	// Create the text area
	wxSize areaSize = GetClientSize();
	areaSize.SetWidth(areaSize.GetWidth() / 2);
	m_TextAreaUser = new wxStyledTextCtrl(this, ID_TextAreaUser, wxDefaultPosition, areaSize, wxTE_PROCESS_ENTER | wxTE_MULTILINE);
	
	// Set up the margins for the line numbers
	m_TextAreaUser->SetMarginWidth(m_TextAreaMargin, m_TextAreaWidth);
	m_TextAreaUser->SetMarginType(m_TextAreaMargin, m_TextAreaType);

	SetTitle("Untitled* - "DEFAULT_FRAME_TITLE);
}

void SavvyEditor::AppFrame::OnResize(wxSizeEvent& a_Event)
{
	if (m_TextAreaUser)
	{
		wxSize areaSize = GetClientSize();
		areaSize.SetWidth(areaSize.GetWidth() / 2);
		m_TextAreaUser->SetSize(areaSize);
	}
}

void SavvyEditor::AppFrame::OnTextChanged(wxCommandEvent& event)
{
	if (m_CurrDocPath != DEFAULT_DOC_PATH)
	{
		SetTitle(m_CurrDocPath + "* - "DEFAULT_FRAME_TITLE);
	}
}

void SavvyEditor::AppFrame::OnUndo(wxCommandEvent& a_Event)
{
	m_TextAreaUser->Undo();
}

void SavvyEditor::AppFrame::OnRedo(wxCommandEvent& a_Event)
{
	m_TextAreaUser->Redo();
}

void SavvyEditor::AppFrame::OnCut(wxCommandEvent& a_Event)
{
	m_TextAreaUser->Cut();
}

void SavvyEditor::AppFrame::OnCopy(wxCommandEvent& a_Event)
{
	m_TextAreaUser->Copy();
}

void SavvyEditor::AppFrame::OnPaste(wxCommandEvent& a_Event)
{
	m_TextAreaUser->Paste();
}

void SavvyEditor::AppFrame::OnDelete(wxCommandEvent& a_Event)
{
	m_TextAreaUser->RemoveSelection();
}

void SavvyEditor::AppFrame::OnSelectAll(wxCommandEvent& a_Event)
{
	m_TextAreaUser->SelectAll();
}

void SavvyEditor::AppFrame::OnConvert(wxCommandEvent& a_Event)
{
	wxMessageBox("TODO",
		"TODO", wxOK | wxICON_INFORMATION);
}

void SavvyEditor::AppFrame::OnMenuOpen(wxMenuEvent& a_Event)
{
	wxMenu* currMenu = a_Event.GetMenu();
	wxMenuItem* currItem = NULL;
	if (currMenu->GetTitle() == "&Edit")
	{
		// Redo enable/disable
		if (m_TextAreaUser->CanRedo())
		{
			currItem = currMenu->FindItem(wxID_REDO);
			EnableMenuItem(currItem, true);
		}
		else
		{
			currItem = currMenu->FindItem(wxID_REDO);
			EnableMenuItem(currItem, false);
		}

		// Undo enable/disable
		if (m_TextAreaUser->CanUndo())
		{
			currItem = currMenu->FindItem(wxID_UNDO);
			EnableMenuItem(currItem, true);
		}
		else
		{
			currItem = currMenu->FindItem(wxID_UNDO);
			EnableMenuItem(currItem, false);
		}

		// Delete enable/disable
		if (m_TextAreaUser->HasSelection())
		{
			currItem = currMenu->FindItem(wxID_DELETE);
			EnableMenuItem(currItem, true);
		}
		else
		{
			currItem = currMenu->FindItem(wxID_DELETE);
			EnableMenuItem(currItem, false);
		}

		// Copy
		if (m_TextAreaUser->CanCopy())
		{
			currItem = currMenu->FindItem(wxID_COPY);
			EnableMenuItem(currItem, true);
		}
		else
		{
			currItem = currMenu->FindItem(wxID_COPY);
			EnableMenuItem(currItem, false);
		}

		// Cut
		if (m_TextAreaUser->CanCut())
		{
			currItem = currMenu->FindItem(wxID_CUT);
			EnableMenuItem(currItem, true);
		}
		else
		{
			currItem = currMenu->FindItem(wxID_CUT);
			EnableMenuItem(currItem, false);
		}

		// Paste
		if (m_TextAreaUser->CanPaste())
		{
			currItem = currMenu->FindItem(wxID_PASTE);
			EnableMenuItem(currItem, true);
		}
		else
		{
			currItem = currMenu->FindItem(wxID_PASTE);
			EnableMenuItem(currItem, false);
		}
	}
	else if (currMenu->GetTitle() == "&File")
	{
		if (m_TextAreaUser->IsModified())
		{
			currItem = currMenu->FindItem(wxID_SAVE);
			EnableMenuItem(currItem, true);
		}
		else
		{
			currItem = currMenu->FindItem(wxID_SAVE);
			EnableMenuItem(currItem, false);
		}
	}
}

void SavvyEditor::AppFrame::EnableMenuItem(wxMenuItem* a_Item, bool a_Enable)
{
	if (a_Item != NULL)
	{
		a_Item->Enable(a_Enable);
	}
}
