#ifndef _EIGENMATH_H_
#define _EIGENMATH_H_

#include "../core/defs.h"
#include "keypad.h"
#include "mathkey.h"

#include "woopsi/woopsi.h"
#include "woopsi/eventhandler.h"
#include "woopsi/woopsikeyboard.h"
#include "woopsi/multilinetextbox.h"
#include "woopsi/woopsiheaders.h"

#define MAX_HIST 150

using namespace WoopsiUI;

class Eigenmath : public Woopsi, public EventHandler {
public:
	void startup();
	void shutdown();
	bool handleEvent(const EventArgs& e);
	void enter();
	void back();	
	void clearbuf();
	void output_cmd();
	void setitems(ScrollingListBox *list, char ** item);
	
private:
	
	AmigaScreen* outScreen;
	AmigaScreen* inScreen;
	AmigaWindow* window;
	WoopsiKeyboard* _keyboard;
	MultiLineTextBox* _output;
	KeyPad *_keypad;
	WoopsiTimer* _timer;
	ScrollingListBox* listboxMenu;
	ScrollingListBox* listboxItems;
	
	char *history[MAX_HIST];
	int cmd_count;
	int cmd;
};

#endif
