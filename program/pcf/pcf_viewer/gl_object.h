#ifndef PCFVW_GL_OBJECT_H_
#define PCFVW_GL_OBJECT_H_

namespace pcf {

/**
Abstract base class for object tied to OpenGL context.
Object can be created without current OpenGL context. At first call to draw, gets initialized and tied to current OpenGL context. Subsequent draw calls must be while in that same context. As a consequence initialize and draw may not be called from different threads.
*/
class gl_object {
private:
	bool initialized_ = false;

protected:
	bool should_draw_ = true;
	virtual void gl_initialize_() { }
	virtual void gl_initialize_done_() { }
	virtual void gl_uninitialize_() { }
	virtual void gl_draw_() = 0;

public:
	virtual ~gl_object() { }

	bool initialized() const { return initialized_; }
	
	void initialize();
	void uninitialize();
	void draw();
};

}

#endif