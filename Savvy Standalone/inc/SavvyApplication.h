#ifndef SAVVY_APPLICATION_H
#define SAVVY_APPLICATION_H

#include "SavvyIncludes.h"

namespace SavvyEditor
{
	/*
	Represents the top level application class.
	*/
	class Application : public wxApp
	{
	public:
		Application();
		virtual ~Application();
		virtual bool OnInit();
	};
}

#endif // !SAVVY_EDITOR_H