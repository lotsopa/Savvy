#include "SavvyApplication.h"
#include "SavvyAppFrame.h"

// Register and start the application
wxIMPLEMENT_APP(SavvyEditor::Application);

SavvyEditor::Application::Application()
{

}

SavvyEditor::Application::~Application()
{

}

bool SavvyEditor::Application::OnInit()
{
	AppFrame *frame = new AppFrame("Savvy Editor", wxPoint(50, 50), wxSize(450, 340));
	frame->Show(true);
	return true;
}
