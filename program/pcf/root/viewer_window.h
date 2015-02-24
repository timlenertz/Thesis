#ifndef PCFUI_VIEWER_WINDOW_H_
#define PCFUI_VIEWER_WINDOW_H_

#include "../pcf_viewer/pcf_viewer.h"
#include "RQ_OBJECT.h"
#include <KeySymbols.h>
#include <GuiTypes.h>
#include <string>


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
	
	Int_t drag_position_x_, drag_position_y_;
	enum { stop, positive, negative } movement_directions_[3];

	void update_movement_velocity_();
	Int_t gl_width_() const;
	Int_t gl_height_() const;

	void init_();
	void draw_();

	Bool_t HandleTimer(TTimer*);
	
	viewer_window(const viewer_window&) = delete;
	viewer_window& operator=(const viewer_window&) = delete;

public:
	EKeySym key_forwards = kKey_Up;
	EKeySym key_backwards = kKey_Down;
	EKeySym key_left = kKey_Left;
	EKeySym key_right = kKey_Right;
	EKeySym key_up = kKey_a;
	EKeySym key_down = kKey_q;
	
	EMouseButton mouse_button_scroll_up = kButton4;
	EMouseButton mouse_button_scroll_down = kButton5;
	
	float movement_speed = 0.1;

	explicit viewer_window(const std::string& title = "Viewer");
	~viewer_window();
	
	/**
	Access to scene.
	The operator -> call gets forwarded to viewer::operator->.
	*/
	pcf::viewer& operator->() { return viewer_; }
};

}



#endif