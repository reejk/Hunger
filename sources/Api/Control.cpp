#include "Control.h"
#include "compatibility.h"

Control Control::_Control;

Controller::~Controller() { }

Control::Control() {
	_mouseX = _mouseY = 0;
	_lastIdleTime = getCurrentTime();
	_events.clear();
	for(int i = 0; i < 256; i++) _keys[i] = STATE_NO_PRESSED;
}

Control::~Control() {
}

Control& Control::instance() {
	return _Control;
}

void Control::addEvent(const Event &event) {
	_events.push_back(event);
}

void Control::addEvent(KeyState state, Keys key, const CallBackFunction& callback) {
	Event event = { state, key };
	event.instance = NULL;
	event.callback.f = callback;
	addEvent(event);
}

void Control::addEvent(KeyState state, Keys key, const Controller *instance, const CallBackMethod& callback) {
	Event event = { state, key };
	event.instance = const_cast<Controller*>(instance);
	event.callback.m = callback;
	addEvent(event);
}

void Control::removeEvent(const Event& event) {
	for(std::vector<Event>::iterator it = _events.begin(); it != _events.end(); it++) {
		Event e = *it;
		if ((e.state == event.state || event.state == STATE_ANY) &&
				(e.key == event.key || event.key == NO_KEY))
			_events.erase(it, it);
	}
}

void Control::removeEvent(Keys key, KeyState state) {
	Event event = { state, key };
	removeEvent(event);
}

bool Control::isKeyPressed(Keys key) {
	if(key == NO_KEY) return true;
	return _keys[key] == STATE_PRESSED;
}

int Control::getMouseX() {
	return _mouseX;
}

int Control::getMouseY() {
	return _mouseY;
}

void Control::idle() {
	if(getCurrentTime() < _lastIdleTime + 50) return;
	_lastIdleTime = getCurrentTime();

	for(std::vector<Event>::const_iterator it = _events.begin(); it != _events.end(); it++) {
		Event e = *it;
		if(e.key == NO_KEY) continue;
		if(((_keys[e.key] == STATE_NO_PRESSED || _keys[e.key] == STATE_PRESSED)
							&& e.state == _keys[e.key]) || e.state == STATE_ANY)
			callCallBack(e.instance, e.callback, _keys[e.key], e.key, _mouseX, _mouseY);
	}
}

void Control::drawHud() const {
	// No hud
}

void Control::keyboardEvent(KeyState state, Keys key) {
	if(_keys[key] == state) return;

	_keys[key] = state;
	state = state == STATE_NO_PRESSED ? STATE_UP : STATE_DOWN;

	for(std::vector<Event>::iterator it = _events.begin(); it != _events.end(); it++) {
		Event e = *it;
		if(e.key == key && (state == e.state || e.state == STATE_ANY))
			callCallBack(e.instance, e.callback, state, key, _mouseX, _mouseY);
	}
}

void Control::mouseEvent(KeyState state, Keys key) {
	if(_keys[key] == state) return;

	_keys[key] = state;
	state = state == STATE_NO_PRESSED ? STATE_UP : STATE_DOWN;

	for(std::vector<Event>::iterator it = _events.begin(); it != _events.end(); it++) {
		Event e = *it;
		if(e.key == key && (state == e.state || e.state == STATE_ANY))
			callCallBack(e.instance, e.callback, state, key, _mouseX, _mouseY);
	}
}

void Control::mouseMoveEvent(int x, int y) {
	_mouseX = x;
	_mouseY = y;

	for(std::vector<Event>::iterator it = _events.begin(); it != _events.end(); it++) {
		Event e = *it;
		if(e.state == STATE_MOUSE_MOVE && isKeyPressed(e.key))
			callCallBack(e.instance, e.callback, STATE_MOUSE_MOVE, e.key, x, y);
	}
}

void Control::callCallBack(const Controller *instance, CallBack callback, KeyState state, Keys key, int x, int y) const {
	Event event = { state, key, const_cast<Controller*>(instance), callback };

	if(event.callback.f) {
		if(!event.instance) event.callback.f(event, x, y);
		else (event.instance->*(event.callback.m))(event, x, y);
	}
}
