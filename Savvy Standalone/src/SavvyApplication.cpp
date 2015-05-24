#include "SavvyApplication.h"
#include "SavvyAppFrame.h"

#define DEFAULT_APP_WIDTH 800
#define DEFAULT_APP_HEIGHT 600

// Register and start the application
wxIMPLEMENT_APP(SavvyEditor::Application);

SavvyEditor::Application::Application() : m_AppFrame(NULL)
{

}

SavvyEditor::Application::~Application()
{
	
}

bool SavvyEditor::Application::OnInit()
{
	m_AppFrame = new AppFrame("Savvy Editor", wxPoint(50, 50), wxSize(DEFAULT_APP_WIDTH, DEFAULT_APP_HEIGHT));
	m_AppFrame->Show(true);
	return true;
}
