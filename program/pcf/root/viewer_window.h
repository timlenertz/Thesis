#ifndef PCFUI_VIEWER_WINDOW_H_
#define PCFUI_VIEWER_WINDOW_H_

#include "RQ_OBJECT.h"
#include "../pcf_viewer/pcf_viewer.h"

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
	};

	const static std::size_t initial_width_;
	const static std::size_t initial_height_;
	const static Long_t refresh_rate_;

	TGMainFrame frame_;
	TGLWidget* gl_widget_;
	event_handler event_handler_;
	TTimer timer_;
	pcf::viewer viewer_;
	
	Int_t drag_position_x_, drag_position_y_;
	enum { stop, positive, negative } movement_directions_[3];
	float movement_speed_ = 0.1;

	void init_();
	void draw_();

	Bool_t HandleTimer(TTimer*);
	
	viewer_window(const viewer_window&) = delete;
	viewer_window& operator=(const viewer_window&) = delete;

public:
	viewer_window();
	~viewer_window();
	
	pcf::viewer& operator->() { return viewer_; }
};

}

#endif