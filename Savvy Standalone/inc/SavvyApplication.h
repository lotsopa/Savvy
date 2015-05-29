/*
The MIT License (MIT)

Copyright (c) 2015 Apostol Dadachev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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

		enum OptionID
		{
			ID_Timer = 1
		};

	private:
		void OnTimer(wxTimerEvent& a_Event);
		wxDECLARE_EVENT_TABLE();
		AppFrame* m_AppFrame;
		wxSplashScreen* m_SplashScreen;
		wxTimer m_SplashTimer;
	};
}

#endif // !SAVVY_EDITOR_H