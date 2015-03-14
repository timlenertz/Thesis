#include "viewer_window.h"
#include <Eigen/Eigen>
#include <TVirtualX.h>
#include <map>


namespace {
	const int default_window_width_ = 800;
	const int default_window_height_ = 600;
	const pcf::angle rotation_per_cursor_pixel_ = pcf::pi / 500.0;
	const pcf::angle rotation_per_click_ = pcf::pi / 200.0;
	const Long_t refresh_rate_ = 20;
	const float slow_factor_ = 6.0;
	
}


viewer_window::event_handler::event_handler(viewer_window& vwin, TGWindow* win, TObject* obj) :
	TGEventHandler("", win, obj),
	win_(vwin) { }



Bool_t viewer_window::event_handler::HandleButton(Event_t* ev) {
	// Register mouse position
	win_.drag_position_x_ = ev->fX;
	win_.drag_position_y_ = ev->fY;
	
	// Handle scroll wheel (roll camera or object)
	bool obj = (ev->fState & win_.key_mask_for_object);
	pcf::motion_controller& ctr = (obj ? win_.object_motion_controller_ : win_.camera_motion_controller_);
	if(ev->fCode == win_.mouse_button_scroll_up)
		ctr.roll(+rotation_per_click_);
	else if(ev->fCode == win_.mouse_button_scroll_down)
		ctr.roll(-rotation_per_click_);
	
	return kTRUE;
}


Bool_t viewer_window::event_handler::HandleMotion(Event_t* ev) {
	bool obj = (ev->fState & win_.key_mask_for_object);
	pcf::motion_controller& ctr = (obj ? win_.object_motion_controller_ : win_.camera_motion_controller_);

	if(ev->fState & kButton1Mask) {
		Int_t diff_x = ev->fX - win_.drag_position_x_;
		Int_t diff_y = ev->fY - win_.drag_position_y_;
		win_.drag_position_x_ = ev->fX;
		win_.drag_position_y_ = ev->fY;
		ctr.rotate(diff_x * rotation_per_cursor_pixel_, diff_y * rotation_per_cursor_pixel_);
	}
	return kTRUE;
}


Bool_t viewer_window::event_handler::HandleKey(Event_t* ev) {
	struct response {
		std::ptrdiff_t dimension;
		movement_direction direction;
		bool is_object;
		
		response(std::ptrdiff_t dim, movement_direction dir, bool obj) :
			dimension(dim), direction(dir), is_object(obj) { }
	};
	
	const std::map<UInt_t, response> responses = {
		{ win_.key_camera_forwards,  response(2, negative, false) },
		{ win_.key_camera_backwards, response(2, positive, false) },
		{ win_.key_camera_left,      response(0, negative, false) },
		{ win_.key_camera_right,     response(0, positive, false) },
		{ win_.key_camera_up,        response(1, positive, false) },
		{ win_.key_camera_down,      response(1, negative, false) },
		{ win_.key_object_forwards,  response(2, negative, true) },
		{ win_.key_object_backwards, response(2, positive, true) },
		{ win_.key_object_left,      response(0, negative, true) },
		{ win_.key_object_right,     response(0, positive, true) },
		{ win_.key_object_up,        response(1, positive, true) },
		{ win_.key_object_down,      response(1, negative, true) }
	};
	
	char tmp[2];
	UInt_t keysym;
	
	gVirtualX->LookupString(ev, tmp, sizeof(tmp), keysym);

	auto it = responses.find(keysym);
	if(it != responses.end()) {
		const response& resp = it->second;
		movement_directions& dir = (resp.is_object ? win_.object_movement_ : win_.camera_movement_);
		if(ev->fType == kGKeyPress) dir[resp.dimension] = resp.direction;
		else if(ev->fType == kKeyRelease) dir[resp.dimension] = stop;
		win_.update_movement_velocity_(ev->fState & win_.key_mask_for_slow);
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
	viewer_(gl_width_(), gl_height_()),
	camera_motion_controller_(viewer_->camera()),
	object_motion_controller_(viewer_->camera())
{
	camera_movement_.fill(stop);
	object_movement_.fill(stop);

	// Open window (frame) with viewer
	frame_.SetWindowName(title.c_str());
	TGLayoutHints* lay = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0);
	frame_.AddFrame(gl_widget_, lay);	
	frame_.MapSubwindows();
	frame_.MapRaised();
	frame_.Layout();
	viewer_.resize_viewport(gl_width_(), gl_height_());

	// Setup UI event handling
	gVirtualX->SetKeyAutoRepeat(kFALSE);
	gVirtualX->GrabKey(frame_.GetId(), kAnyKey, kAnyModifier, kTRUE);
	gl_widget_->SetEventHandler(&event_handler_);
	camera_motion_controller_.object = &viewer_->camera();
	
	// Initialize renderer
	init_();
	
	// Start timer
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


float viewer_window::speed_(movement_direction dir) const {
	switch(dir) {
		case positive: return movement_speed;
		case negative: return -movement_speed;
		default: return 0;
	}
}


void viewer_window::update_movement_velocity_(bool slow) {
	Eigen::Vector3f cam_vel, obj_vel;
	for(std::ptrdiff_t i = 0; i < 3; ++i) {
		cam_vel[i] = speed_(camera_movement_[i]);
		obj_vel[i] = speed_(object_movement_[i]);
	}
	if(slow) {
		cam_vel /= slow_factor_;
		obj_vel /= slow_factor_;
	}
	
	camera_motion_controller_.target_velocity = cam_vel;
	object_motion_controller_.target_velocity = obj_vel;
}


Bool_t viewer_window::HandleTimer(TTimer*) {
	camera_motion_controller_.tick();
	object_motion_controller_.tick();
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


void viewer_window::select_object(pcf::space_object& obj) {
	object_motion_controller_.object = &obj;
}
