#include "viewer_window.h"
#include "pcf_core/geometry/math_constants.h"
#include "pcf_core/geometry/angle.h"
#include <Eigen/Eigen>
#include <TVirtualX.h>


namespace pcfui {

namespace {
	const int default_window_width_ = 800;
	const int default_window_height_ = 600;
	const pcf::angle rotation_per_cursor_pixel_ = pcf::pi / 500.0;
	const pcf::angle rotation_per_click_ = pcf::pi / 200.0;
	const Long_t refresh_rate_ = 10;
}


viewer_window::event_handler::event_handler(viewer_window& vwin, TGWindow* win, TObject* obj) :
	TGEventHandler("", win, obj),
	win_(vwin) { }



Bool_t viewer_window::event_handler::HandleButton(Event_t* ev) {
	win_.drag_position_x_ = ev->fX;
	win_.drag_position_y_ = ev->fY;
	
	if(ev->fCode == win_.mouse_button_scroll_up)
		win_.viewer_.roll_camera(+rotation_per_click_);
	else if(ev->fCode == win_.mouse_button_scroll_down)
		win_.viewer_.roll_camera(-rotation_per_click_);
	
	return kTRUE;
}

Bool_t viewer_window::event_handler::HandleKey(Event_t* ev) {
	char tmp[2];
	UInt_t keysym;
	
	gVirtualX->LookupString(ev, tmp, sizeof(tmp), keysym);

	std::ptrdiff_t dim;
	bool pos;
		
	if(keysym == win_.key_forwards) {
		dim = 2; pos = false;
	} else if(keysym == win_.key_backwards) {
		dim = 2; pos = true;
	} else if(keysym == win_.key_left) {
		dim = 0; pos = false;
	} else if(keysym == win_.key_right) {
		dim = 0; pos = true;
	} else if(keysym == win_.key_up) {
		dim = 1; pos = true;
	} else if(keysym == win_.key_down) {
		dim = 1; pos = false;
	} else {
		return kTRUE;
	}
		
	if(ev->fType == kGKeyPress) win_.movement_directions_[dim] = (pos ? positive : negative);
	else if(ev->fType == kKeyRelease) win_.movement_directions_[dim] = stop;
	else return kTRUE;
	
	win_.update_movement_velocity_();

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


Bool_t viewer_window::event_handler::HandleConfigureNotify(Event_t* ev) {
	win_.viewer_.resize_viewport( win_.gl_width_(), win_.gl_height_() );
	return kTRUE;
}


viewer_window::viewer_window(const std::string& title) :
	frame_(gClient->GetRoot(), default_window_width_, default_window_height_),
	gl_widget_( TGLWidget::Create(&frame_, kTRUE, kFALSE, NULL, default_window_width_, default_window_height_) ),
	event_handler_(*this, &frame_, gl_widget_),
	timer_(refresh_rate_, kTRUE),
	viewer_(gl_width_(), gl_height_())
{
	frame_.SetWindowName(title.c_str());

	TGLayoutHints* lay = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0);
	frame_.AddFrame(gl_widget_, lay);
	
	frame_.MapSubwindows();
	frame_.MapRaised();
	frame_.Layout();
	
	gVirtualX->SetKeyAutoRepeat(kFALSE);
	gVirtualX->GrabKey(frame_.GetId(), kAnyKey, kAnyModifier, kTRUE);
	gl_widget_->SetEventHandler(&event_handler_);
	viewer_.resize_viewport(gl_width_(), gl_height_());
	
	for(std::ptrdiff_t i = 0; i < 3; ++i) movement_directions_[i] = stop;
	
	init_();
	
	timer_.SetObject(this);
	timer_.TurnOn();
}


viewer_window::~viewer_window() {
	timer_.TurnOff();
}


Int_t viewer_window::gl_width_() const {
	return frame_.GetWidth() * 2;
}


Int_t viewer_window::gl_height_() const {
	return frame_.GetHeight() * 2;
}



void viewer_window::update_movement_velocity_() {
	Eigen::Vector3f vel;
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		switch(movement_directions_[i]) {
			case stop: vel[i] = 0; break;
			case positive: vel[i] = movement_speed_; break;
			case negative: vel[i] = -movement_speed_; break;
		}
	}
	viewer_.set_camera_target_velocity(vel);
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