#ifndef SAVVY_APPLICATION_H
#define SAVVY_APPLICATION_H

#include "SavvyIncludes.h"

namespace SavvyEditor
{
	class AppFrame;

	/*
	Represents the top level application class.
	*/
	class Application : public wxApp
	{
	public:
		Application();
		virtual ~Application();
		virtual bool OnInit();

	private:
		AppFrame* m_AppFrame;
	};
}

#endif // !SAVVY_EDITOR_H