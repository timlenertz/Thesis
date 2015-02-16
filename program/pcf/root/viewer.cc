#include "RQ_OBJECT.h"

#include "../pcf_viewer/pcf_viewer.h"

namespace pcfui {

class viewer : public TObject {
private:
	class event_handler : public TGEventHandler {
	public:
		event_handler(TGWindow* win, TObject* obj);
		Bool_t HandleButton(Event_t*) override;
	};

	const static std::size_t initial_width_;
	const static std::size_t initial_height_;
	const static Long_t refresh_rate_;

	TGMainFrame frame_;
	TGLWidget* gl_widget_;
	event_handler event_handler_;
	TTimer timer_;
	pcf::viewer viewer_;

	void init_();
	void draw_();

	Bool_t HandleTimer(TTimer*);
	
	viewer(const viewer&) = delete;
	viewer& operator=(const viewer&) = delete;

public:
	viewer();
	~viewer();
	
	pcf::viewer& operator->() { return viewer_; }
};


const std::size_t viewer::initial_width_ = 800;
const std::size_t viewer::initial_height_ = 600;
const Long_t viewer::refresh_rate_ = 10;


viewer::event_handler::event_handler(TGWindow* win, TObject* obj) :
	TGEventHandler("PCF Viewer Event Handler", win, obj) { }


Bool_t viewer::event_handler::HandleButton(Event_t*) {
	std::cout << "event" << std::endl;
	return kTRUE;
}

viewer::viewer() :
	frame_(gClient->GetRoot(), initial_width_, initial_height_),
	gl_widget_( TGLWidget::Create(&frame_, kTRUE, kFALSE, NULL, frame_.GetWidth(), frame_.GetHeight()) ),
	event_handler_(&frame_, gl_widget_),
	timer_(refresh_rate_, kTRUE),
	viewer_(frame_.GetWidth(), frame_.GetHeight())
{
	TGLayoutHints* lay = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0);
	frame_.AddFrame(gl_widget_, lay);
	
	frame_.MapSubwindows();
	frame_.MapRaised();
	frame_.Layout();
	
	gl_widget_->SetEventHandler(&event_handler_);
	
	init_();
	
	timer_.SetObject(this);
	timer_.TurnOn();
}


viewer::~viewer() {
	timer_.TurnOff();
}


Bool_t viewer::HandleTimer(TTimer*) {
	draw_();
	return kTRUE;
}


void viewer::init_() {
	gl_widget_->MakeCurrent();
}


void viewer::draw_() {
	gl_widget_->MakeCurrent();
	viewer_.draw();
	gl_widget_->SwapBuffers();
}

}