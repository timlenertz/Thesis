#ifndef PCFUI_VIEWER_WINDOW_H_
#define PCFUI_VIEWER_WINDOW_H_

#include "RQ_OBJECT.h"
#include <KeySymbols.h>
#include <GuiTypes.h>
#include <string>
#include <array>


namespace pcfui {

class viewer_window : public TObject {
private:
	class event_handler : public TGEventHandler {
	private:
		viewer_window& win_;
	public:
		event_handler(viewer_window&, TGWindow*, TObject*);
		Bool_t HandleButton(Event_t*) override;
		Bool_t HandleKey(Event_t*) override;
		Bool_t HandleMotion(Event_t*) override;
		Bool_t HandleConfigureNotify(Event_t*) override;
	};

	TGMainFrame frame_;
	TGLWidget* gl_widget_;
	event_handler event_handler_;
	TTimer timer_;
	
	pcf::viewer viewer_;
	pcf::motion_controller camera_motion_controller_;
	pcf::motion_controller object_motion_controller_;
	
	enum movement_direction { stop, positive, negative };
	using movement_directions = std::array<movement_direction, 3>;
	Int_t drag_position_x_, drag_position_y_;
	movement_directions camera_movement_, object_movement_;

	float speed_(movement_direction) const;
	void update_movement_velocity_(bool slow);
	Int_t gl_width_() const;
	Int_t gl_height_() const;

	void init_();
	void draw_();

	Bool_t HandleTimer(TTimer*);
	
	viewer_window(const viewer_window&) = delete;
	viewer_window& operator=(const viewer_window&) = delete;

public:
	EKeySym key_camera_forwards = kKey_Up;
	EKeySym key_camera_backwards = kKey_Down;
	EKeySym key_camera_left = kKey_Left;
	EKeySym key_camera_right = kKey_Right;
	EKeySym key_camera_up = kKey_p;
	EKeySym key_camera_down = kKey_m;
	
	EKeySym key_object_forwards = kKey_z;
	EKeySym key_object_backwards = kKey_s;
	EKeySym key_object_left = kKey_q;
	EKeySym key_object_right = kKey_d;
	EKeySym key_object_up = kKey_r;
	EKeySym key_object_down = kKey_f;
	
	EMouseButton mouse_button_scroll_up = kButton4;
	EMouseButton mouse_button_scroll_down = kButton5;
	
	Mask_t key_mask_for_object = kKeyShiftMask;
	Mask_t key_mask_for_slow = kKeyMod1Mask;
	
	float movement_speed = 0.003;

	explicit viewer_window(const std::string& title = "Viewer");
	~viewer_window();
	
	/**
	Access to scene.
	The operator -> call gets forwarded to viewer::operator->.
	*/
	pcf::viewer& operator->() { return viewer_; }
	
	void select_object(pcf::space_object& obj);
};

}



#endif