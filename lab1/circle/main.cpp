#include "Canvas.h"
#include <memory>
#include <vector>
#include <wx/wx.h>

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

class MyFrame final : public wxFrame
{
public:
	MyFrame()
		: wxFrame(nullptr, wxID_ANY, "Initials", wxDefaultPosition, wxSize(WIDTH, HEIGHT))
	{
		m_canvas = std::make_shared<Canvas>(this);
	}

private:
	std::shared_ptr<Canvas> m_canvas;
};

class MyApp final : public wxApp
{
public:
	bool OnInit() override
	{
		m_frame = std::make_unique<MyFrame>();
		m_frame->Show(true);
		return true;
	}

private:
	std::unique_ptr<MyFrame> m_frame;
};

wxIMPLEMENT_APP(MyApp);
