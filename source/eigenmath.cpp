
#include <string.h>
#include <malloc.h>
#include "eigenmath.h"
#include "menu.h"
#include "fonts.h"

#define MEMINFO  0
#define MAXCOLOR 7

#define READBUFLEN 250
#define OUTBUFLEN 10000

int line_height = 5;
int left_margin = 20;
int right_margin = 1;
int display_x;
int display_y;
int display_width;
int display_height;
int max_x;
int max_y;
int total_w;
int total_h;
int update_display_request;
extern int esc_flag;

SuperBitmap *draw_context;
AmigaWindow *canvas;
AmigaWindow *script;
MultiLineTextBox* editScript;

Font *font6px;
Font *font10px;

char *readbuf;
char *ptr_readbuf;
char *outbuf;
char *ptr_outbuf;
bool draw_top;
bool script_top;
bool zoom_text;

int inxcolor;
u16 colors[] = {261120, 33023, 40928, 261375, 53908, 37119, 40959, 262112};

	
void meminfo();
void setup_draw_context(void);
void display_range(char * srange);

extern int test_flag;

			
void Eigenmath::startup() {

	Gadget::Rect rect;

	readbuf = ptr_readbuf = new char[READBUFLEN];
	outbuf = ptr_outbuf = new char[OUTBUFLEN]; 
	
	cmd_count = 0;
	cmd = 0;
	test_flag = 0;
	draw_top = true;
	script_top = false;
	zoom_text = true;
	
	font6px = new Font(font6px_Bitmap, 128, 36, 4, 6, 64543);
	font10px = new Font(font10px_Bitmap, 256, 60, 8, 10, 64543);
	
	Woopsi::startup();
	
	outScreen = new AmigaScreen("Eigenmath DS", Gadget::GADGET_PERMEABLE, AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(outScreen);
	outScreen->flipToTopScreen();
	
	script = new AmigaWindow(0, 14, 255, 178, "Script", Gadget::GADGET_DRAGGABLE, /*AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE |*/ AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	script->setEventHandler(this);
	outScreen->addGadget(script);
	script->getClientRect(rect);
	//script->hideDepthButton();
	
	editScript = new MultiLineTextBox(rect.x, rect.y, rect.width, rect.height, "", Gadget::GADGET_DRAGGABLE, 250, font10px);
	editScript->setTextAlignmentHoriz(MultiLineTextBox::TEXT_ALIGNMENT_HORIZ_LEFT);
	editScript->setTextAlignmentVert(MultiLineTextBox::TEXT_ALIGNMENT_VERT_TOP);
	editScript->setBackColour(woopsiRGB(255, 255, 255));
	editScript->showCursor();
	script->addGadget(editScript);
	
	
	canvas = new AmigaWindow(0, 14, 255, 178, "Graphics", Gadget::GADGET_DRAGGABLE, /*AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE |*/ AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	canvas->setEventHandler(this);
	outScreen->addGadget(canvas);
	canvas->getClientRect(rect);
	
	draw_context = new SuperBitmap(rect.x, rect.y, rect.width, rect.height, rect.width, rect.height, 1);
	canvas->addGadget(draw_context);
	draw_context->setBackColour(woopsiRGB(255, 255, 255));

	window = new AmigaWindow(0, 13, 255, 178, "Output", Gadget::GADGET_DRAGGABLE, /*AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE |*/ AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	window->setEventHandler(this);
	outScreen->addGadget(window);
	window->getClientRect(rect);
	
	
	_output = new MultiLineTextBox(rect.x, rect.y, rect.width, rect.height, "", Gadget::GADGET_DRAGGABLE, 250, font10px);
	_output->setTextAlignmentHoriz(MultiLineTextBox::TEXT_ALIGNMENT_HORIZ_LEFT);
	_output->setTextAlignmentVert(MultiLineTextBox::TEXT_ALIGNMENT_VERT_TOP);
	_output->setBackColour(woopsiRGB(255, 255, 255));
	window->addGadget(_output);
	
	inScreen = new AmigaScreen("Controls", Gadget::GADGET_PERMEABLE, AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(inScreen);
	
	AmigaWindow* functions = new AmigaWindow(0, 30, 255, 139, "Functions", Gadget::GADGET_DRAGGABLE, /*AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE |*/ AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	functions->setEventHandler(this);
	inScreen->addGadget(functions);
	
	listboxMenu = new ScrollingListBox(5, 14, 130, 120);
	listboxMenu->setEventHandler(this);
	functions->addGadget(listboxMenu);
	setitems(listboxMenu, items[0]);
	
	listboxMenu->setAllowMultipleSelections(false);
	
	listboxItems = new ScrollingListBox(136, 14, 115, 120);
	listboxItems->setEventHandler(this);
	functions->addGadget(listboxItems);
	setitems(listboxItems, items[1]);
	listboxItems->setAllowMultipleSelections(false);
	
	// Create keyboard
	_keyboard = new WoopsiKeyboard(5, 50, 247, 121, "Keyboard", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	_keyboard->setEventHandler(this);
	inScreen->addGadget(_keyboard);
	
	_keypad = new KeyPad(1, 14, 253, 178, "Keypad", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	_keypad->setEventHandler(this);
	inScreen->addGadget(_keypad);
	inScreen->setFocusedGadget(_keypad);
	
	
	enableDrawing();
	// Draw GUI
	draw();
	setup_draw_context();
	run("");
	_output->appendText(">");
	
}


bool Eigenmath::handleEvent(const EventArgs& e) {
	
		
		
	if (e.gadget != NULL) {
		
		if (e.gadget == window) 
			if (e.type == EVENT_FOCUS)
				script_top = false;
			
		if (e.gadget == script) 
			if (e.type == EVENT_FOCUS)
				script_top = true;
		
		if (e.gadget == _keyboard) {
			if (e.type == EVENT_ACTION) {
				const WoopsiKey* key = _keyboard->getLastKeyClicked();
				char tkey = key->getValue();	
				
				if (key->getKeyType() == WoopsiKey::KEY_RETURN) {
					
					if(script_top)
						editScript->insertTextAtCursor("\n");
					else
						enter();
				}
				else if (key->getKeyType() == WoopsiKey::KEY_BACKSPACE) {
					
					if(script_top)
						editScript->removeText(editScript->getCursorPosition()-1, 1);
					else
						back();	
				} 
				// Append key value to output box if the last key was not a modifier
				else if (key->getValue() != '\0') {		
					
					if(script_top)
						editScript->insertTextAtCursor(tkey);
					else{
						*ptr_readbuf++ = tkey;			
						_output->appendText(tkey);
					}
				} 				
			}
		}
		
		else if (e.gadget == _keypad) {
			if (e.type == EVENT_ACTION) {
				const MathKey* mkey = _keypad->getLastKeyClicked();
				const char *pkey = mkey->getValue();
				
				if (mkey->getKeyType() == MathKey::KEY_RETURN) {
					
					if(script_top)
						editScript->insertTextAtCursor("\n");
					else
						enter();	
				
				}
				else if (mkey->getKeyType() == MathKey::KEY_BACKSPACE) {
					
					if(script_top)
						editScript->removeText(editScript->getCursorPosition()-1, 1);
					else
						back();	
				} 
				// Append key value to output box if the last key was not a modifier
				else if (*mkey->getValue() != '\0') {
					const char *ckey = pkey;
					
					if(script_top)
						editScript->insertTextAtCursor(pkey);
					else{
						
						do *ptr_readbuf++ = *ckey; while(*(++ckey) != 0);						
							
							if(strcmp(readbuf, "float") == 0) {						
								run("float");
								_output->appendText(readbuf);
								output_cmd();
							}
							else if(strcmp(readbuf, "clear") == 0) {
								run("clear");
								output_cmd();
								_output->jump(0,0);
								_output->setText(">");							
							}
							else if(strcmp(readbuf, "tty=") == 0) {
								if (equaln(get_binding(symbol(TTY)), 1)) {
									run("tty=0");
									_output->appendText("tty=0");
								}
								else {
									run("tty=1");
									_output->appendText("tty=1");
								}
								output_cmd();														
							}
							else
							_output->appendText(pkey);
					} 		
				}		
			}
		}
		else if (e.gadget == listboxMenu) 
		{
			if (e.type == EVENT_DOUBLE_CLICK) 
			{
				listboxItems->removeAllOptions();
				setitems(listboxItems, items[listboxMenu->getSelectedIndex()+1]);
				listboxItems->draw();
			}
		}
		else if (e.gadget == listboxItems) 
		{
			if (e.type == EVENT_DOUBLE_CLICK) 
			{
				const char *fpkey = listboxItems->getSelectedOption()->text;	
				const char *fckey = fpkey;
				
				if(script_top){
					editScript->insertTextAtCursor(fpkey);
					editScript->insertTextAtCursor("(");	
				}
				else{
					do *ptr_readbuf++ = *fckey; while(*(++fckey) != 0);
					*ptr_readbuf++ = '(';
					_output->appendText(fpkey);
					_output->appendText("(");
				}
			}
		}
	}
	
	if (e.type == EVENT_KEY_RELEASE){
		switch (e.keyCode){
			case KEY_CODE_A:
				if(!script_top)
					enter();
				else
					editScript->insertTextAtCursor("\n");
				break;
			case KEY_CODE_B:
				if(draw_top){
					window->lowerToBottom();			
					draw_top = false;
				}
				else{
					window->raiseToTop();
					draw_top = true;
				}
				break;	
			case KEY_CODE_Y:
				if(!script_top){
					script->raiseToTop();
					script_top = true;	
				}
				else{
					script->lowerToBottom();
					script_top = false;	
					run((char *)editScript->getText()->getCharArray());
					output_cmd();
				}
				//back();
				break;
			case KEY_CODE_X:
				woopsiApplication->flipScreens(inScreen);		
				break;
			case KEY_CODE_UP:
				if (script_top)
					editScript->moveCursorToPosition(editScript->getCursorPosition() - 30);
				else{
					_output->scroll(0, 5);
					_output->draw();
				}
				break;
			case KEY_CODE_DOWN:
				if (script_top)
					editScript->moveCursorToPosition(editScript->getCursorPosition() + 30);
				else{
					_output->scroll(0, -5);
					_output->draw();
				}	
				break;
			case KEY_CODE_LEFT:
				if (script_top)
					editScript->moveCursorToPosition(editScript->getCursorPosition() - 1);
				else if(cmd != 0){					
					while(ptr_readbuf != readbuf)
						back();
					--cmd;
					for(int i=0; history[cmd][i] !=0; i++)
						*ptr_readbuf++ = history[cmd][i];	
					_output->appendText(readbuf);
				}	
				break;
			case KEY_CODE_RIGHT:	
				if (script_top)
					editScript->moveCursorToPosition(editScript->getCursorPosition() + 1);
				else if(cmd < cmd_count-1){
					while(ptr_readbuf != readbuf)
						back();
					++cmd;
					for(int i=0; history[cmd][i] !=0; i++)
						*ptr_readbuf++ = history[cmd][i];
					_output->appendText(readbuf);
				}
				break;
			case KEY_CODE_START:
				if(zoom_text){
					_output->setFont(font6px);
					zoom_text = false;	
				}
				else{
					_output->setFont(font10px);
					zoom_text = true;	
				}
				_output->appendText("\n>");
				break;
			case KEY_CODE_SELECT:
				_output->appendText(
				"\nEigenmath DS is ported by\n" 
				"Leonel Hernandez\n"
				"to Nintendo DS\n"
				"from the original source code\nwritten by George Weigt.\n\n"
				"To report bugs or suggestions:\nleonelhs@gmail.com\n"
				"http://eigenmatds.sourceforge.net\n>");
				//meminfo();				
				break;
					default:
						break;
		}
	}

	return 0;
}

void Eigenmath::shutdown() {
	
	Woopsi::shutdown();
}

void Eigenmath::enter(){
	
	if(readbuf < ptr_readbuf)
		if(cmd_count < MAX_HIST){
			history[cmd_count++] = strdup(readbuf);
			cmd = cmd_count;
		}
		
	run(readbuf);
	output_cmd();
	
}

void Eigenmath::output_cmd(){
	
	_output->appendText('\n');
	_output->appendText(outbuf);
	clearbuf();	
	_output->appendText("\n>");
}

void Eigenmath::clearbuf(){
	
	while(ptr_readbuf != readbuf)
		*(--ptr_readbuf) = 0;
	while(ptr_outbuf != outbuf)
		*(--ptr_outbuf) = 0;
}

void Eigenmath::back(){
	
	if(readbuf < ptr_readbuf){						
		*(--ptr_readbuf) = 0;
		_output->removeText(_output->getTextLength() - 1);
	}
}

void Eigenmath::setitems(ScrollingListBox *list, char ** item){

	do{
		list->addOption(*item, 0);
	}while(*(item++)[0] != '\0');
}

//functions to display math functions result
extern void eval_print(void);

void eval_display(void){
	
	eval_print();
}


void printstr(char *s){

	while (*s)
		printchar(*s++);
}

void printchar(int c){
	
	*ptr_outbuf++ = c; 	
}

void printchar_nowrap(int c){
	
	printchar(c);
}

//functions to draw math functions
void setup_draw_context(void){
	
	draw_context->clearBitmap();
	display_width = 161;
	display_height = 161;
}

void draw_text(int font, int x, int y, char *s, int len){
		
	draw_context->drawText(x, y, font6px, s);

}

void draw_line(int x1, int y1, int x2, int y2){
	
	draw_context->drawLine(x1, y1, x2, y2, woopsiRGB(0, 0, 0));
}

void draw_point(int x, int dx, int y, int dy){
	
	int left = x + dx - 1;
	int top = y + dy - 1;
	
	if (dx == 0)
		left++;
	if (dy == 0)
		top++;
	
	draw_context->drawPixel(left, top, colors[inxcolor]);
}

void draw_box(int x1, int y1, int x2, int y2) {
	
	draw_context->drawRect(x1, y1, x2, y2, woopsiRGB(0, 0, 0));	
}


void draw_main(void){
	
	
	if (draw_flag) {
		draw_flag = 0; // so "stop" really stops
		stop("draw calls draw");
	}
	
	scan("isnewrange = and(lastxrange == xrange, lastyrange == yrange, lasttrange == trange)");
	eval();
	pop();
		
	if (equaln(get_binding(usr_symbol("isnewrange")), 0)) {
		draw_context->clearBitmap();
		scan("eval(lastxrange = xrange, lastyrange = yrange, lasttrange = trange)");
		eval();
		pop();
	}
	
	draw_flag++;
	
	setup_trange();
	setup_xrange();
	setup_yrange();
	check_for_parametric_draw();
	create_point_set();
	emit_graph();
	draw_display();
	
	draw_context->draw();
	canvas->raiseToTop();
	draw_top = false;
	
	inxcolor == MAXCOLOR ? inxcolor = 0 : inxcolor++; 
	
	draw_flag--;

}



void eval_clear(void){
	
	if (test_flag == 0)
		clear_term();
	
	clear_symbols();
	defn();
	push(symbol(NIL));
	
	draw_context->clearBitmap();
	canvas->lowerToBottom();
}


#ifdef MEMINFO

#define NDS_STACK_END (unsigned int)0x0b000000
extern char *fake_heap_end;

void meminfo(){
	
	unsigned int stackSize = NDS_STACK_END - (unsigned int)fake_heap_end;
	// Next line taken from the newlib patches.. 
	extern char   end asm ("_end");	// Defined by the linker.  
	struct mallinfo m = mallinfo();
	unsigned int heapSize = (unsigned int)fake_heap_end - (unsigned int)&end;

	Debug::printf("Heap:\n");
	Debug::printf(" start          %x\n", (unsigned int)&end);
	Debug::printf(" end            %x\n", (unsigned int)fake_heap_end);
	Debug::printf(" size  (system) %d\n", heapSize);
	Debug::printf(" size  (malloc) %d\n", m.arena);
	Debug::printf(" usage (malloc) %2d%% (%d)\n", (100 * m.uordblks)/m.arena, m.uordblks);
	Debug::printf(" usage (system) %2d%%\n", (100 * m.uordblks)/heapSize);

	Debug::printf("Stack:\n");
	Debug::printf(" start    %x\n", (unsigned int)fake_heap_end);
	Debug::printf(" end      %x\n", (unsigned int)NDS_STACK_END);
	Debug::printf(" current  %x\n", (unsigned int)&stackSize);
	Debug::printf(" usage    %2d%% (%d/%d)\n", \
	    (unsigned int)((100*(NDS_STACK_END-(unsigned int)&stackSize))/(float)stackSize), \
	    NDS_STACK_END-(unsigned int)&stackSize, \
	    stackSize);

}

#endif

/*run("draw(-1)");
	run("draw(-2)");
	run("draw(-3)");
	run("draw(-4)");
	run("draw(-5)");
	run("draw(-6)");
	run("draw(-7)");
	run("draw(-8)");*/
	/*run("draw(1)");
	run("draw(2)");
	run("draw(3)");
	run("draw(4)");
	run("draw(5)");
	run("draw(6)");
	run("draw(7)");
	run("draw(8)");*/
	
	//run("draw()");
	//draw_context->draw();
	
	
	//Debug::printf("%d", woopsiRGB(255, 128, 255));
	//Debug::printf("%d", woopsiRGB(0, 128, 64));
	//Debug::printf("%d", woopsiRGB(255, 128, 0));
	
