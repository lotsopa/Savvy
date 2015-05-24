#include "SavvyApplication.h"
#include "SavvyAppFrame.h"

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
	m_AppFrame = new AppFrame("Savvy Editor", wxPoint(50, 50), wxSize(450, 340));
	m_AppFrame->Show(true);
	return true;
}
