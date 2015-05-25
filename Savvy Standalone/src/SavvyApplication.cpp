#include "SavvyApplication.h"
#include "SavvyAppFrame.h"

#define DEFAULT_APP_WIDTH 800
#define DEFAULT_APP_HEIGHT 600
#define SPLASH_SCREEN_TIMEOUT_MS 2000

// Register and start the application
wxIMPLEMENT_APP(SavvyEditor::Application);

wxBEGIN_EVENT_TABLE(SavvyEditor::Application, wxApp)
EVT_TIMER(ID_Timer, SavvyEditor::Application::OnTimer)
wxEND_EVENT_TABLE()

SavvyEditor::Application::Application() : m_AppFrame(NULL), m_SplashTimer(this, ID_Timer)
{

}

SavvyEditor::Application::~Application()
{
	
}

bool SavvyEditor::Application::OnInit()
{
	wxInitAllImageHandlers();

	wxBitmap bitmap;
	if (bitmap.LoadFile("res/Savvy_Logo.png", wxBITMAP_TYPE_PNG))
	{
		m_SplashScreen = new wxSplashScreen(bitmap,wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_NO_TIMEOUT,
			0, NULL, -1, wxDefaultPosition, wxDefaultSize,wxBORDER_SIMPLE | wxSTAY_ON_TOP);
	}
	wxYield();
	m_SplashTimer.StartOnce(SPLASH_SCREEN_TIMEOUT_MS);

	return true;
}

void SavvyEditor::Application::OnTimer(wxTimerEvent& a_Event)
{
	m_AppFrame = new AppFrame(DEFAULT_FRAME_TITLE, wxDefaultPosition, wxSize(DEFAULT_APP_WIDTH, DEFAULT_APP_HEIGHT));
	m_AppFrame->Show(true);
	m_SplashScreen->Destroy();
}

