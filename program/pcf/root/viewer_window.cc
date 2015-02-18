#include "viewer_window.h"
#include "pcf_core/geometry/math_constants.h"
#include "pcf_core/geometry/angle.h"
#include <Eigen/Eigen>

namespace pcfui {

namespace {
	const char window_title_[] = "PCF Viewer";
	const int default_window_width_ = 800;
	const int default_window_height_ = 600;
	const pcf::angle rotation_per_cursor_pixel_ = pcf::pi / 500.0;
	const pcf::angle rotation_per_scroll_offset_ = pcf::pi / 200.0;
	const Long_t refresh_rate_ = 10;
}


viewer_window::event_handler::event_handler(viewer_window& vwin, TGWindow* win, TObject* obj) :
	TGEventHandler("PCF Viewer Event Handler", win, obj),
	win_(vwin) { }



Bool_t viewer_window::event_handler::HandleButton(Event_t* ev) {
	win_.drag_position_x_ = ev->fX;
	win_.drag_position_y_ = ev->fY;
	return kTRUE;
}

Bool_t viewer_window::event_handler::HandleKey(Event_t* ev) {
	std::cout << "key: " << ev->fCode << std::endl;
	return kTRUE;
}

Bool_t viewer_window::event_handler::HandleMotion(Event_t* ev) {
	if(ev->fState & kButton1Mask) {
		Int_t diff_x = ev->fX - win_.drag_position_x_;
		Int_t diff_y = ev->fY - win_.drag_position_y_;
		win_.drag_position_x_ = ev->fX;
		win_.drag_position_y_ = ev->fY;
		win_.viewer_.rotate_camera(diff_x * rotation_per_cursor_pixel_, diff_y * rotation_per_cursor_pixel_);
	}
	return kTRUE;
}

viewer_window::viewer_window() :
	frame_(gClient->GetRoot(), default_window_width_, default_window_height_),
	gl_widget_( TGLWidget::Create(&frame_, kTRUE, kFALSE, NULL, frame_.GetWidth(), frame_.GetHeight()) ),
	event_handler_(*this, &frame_, gl_widget_),
	timer_(refresh_rate_, kTRUE),
	viewer_(frame_.GetWidth(), frame_.GetHeight())
{
	frame_.SetWindowName(window_title_);

	TGLayoutHints* lay = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0);
	frame_.AddFrame(gl_widget_, lay);
	
	frame_.MapSubwindows();
	frame_.MapRaised();
	frame_.Layout();
	
	gl_widget_->SetEventHandler(&event_handler_);
	
	for(std::ptrdiff_t i = 0; i < 3; ++i) movement_directions_[i] = stop;
	
	init_();
	
	timer_.SetObject(this);
	timer_.TurnOn();
}


viewer_window::~viewer_window() {
	timer_.TurnOff();
}


Bool_t viewer_window::HandleTimer(TTimer*) {
	draw_();
	return kTRUE;
}


void viewer_window::init_() {
	gl_widget_->MakeCurrent();
}


void viewer_window::draw_() {
	gl_widget_->MakeCurrent();
	viewer_.draw();
	gl_widget_->SwapBuffers();
}

}