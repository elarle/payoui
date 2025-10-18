#include "lib/payoui.hpp"

#define FRAME_CHARS 

struct FrameDecorations{
	const char * TOP_LEFT = "╔";
	const char * TOP_RIGHT = "╗";
	const char * BOTTOM_LEFT = "╚";
	const char * BOTTOM_RIGHT = "╝";
	const char * VERTICAL = "║";
	const char * HORIZONTAL = "═";
};

void drawFrame(
	PUI::Terminal * terminal, 
	size_t x, 
	size_t y, 
	size_t width, 
	size_t height, 
	FrameDecorations deco = FrameDecorations{}
) {
	
	terminal->moveCursor(x, y);

	terminal->writeText(deco.TOP_LEFT);

	//Top line
	for(size_t i = 1; i < width; i++){
		terminal->writeText(deco.HORIZONTAL);
	}

	terminal->writeText(deco.TOP_RIGHT);
	terminal->moveCursor(x, y + height - 1);
	terminal->writeText(deco.BOTTOM_LEFT);

	//Bottom line
	for(size_t i = 0; i < width - 1; i++){
		terminal->writeText(deco.HORIZONTAL);
	}

	terminal->writeText(deco.BOTTOM_RIGHT);

	terminal->moveCursor(x, y);

	for(size_t i = 1; i < height-1; i++){
		//Left line
		terminal->moveCursor(x, y+i);
		terminal->writeText(deco.VERTICAL);

		//Right line
		terminal->moveCursor(x+width, y+i);
		terminal->writeText(deco.VERTICAL);
	}
}

//Showcase function
void drawTextFramed(PUI::Terminal * terminal, const char * text){
	size_t length = strlen(text);
	drawFrame(terminal, terminal->cols/2 - length/2, terminal->rows/2 - 1, length + 1, 3);
	terminal->moveCursor(terminal->cols/2 - length/2 + 1, terminal->rows/2);

	for(size_t i = 0; i < strlen(text); i++){
		terminal->setForegroundRGB(255-i*4, 255, 255-i*4);
		terminal->setBackgroundRGB(i*4, 0, 255-i*4);

		//If it's an utf-8 char it needs at least 2 bytes to print correclty.
		//So +1 if its > 127!!!!
		uint8_t utf8_fix = !!(text[i] & 0b10000000);
		terminal->writeBytes(&text[i], utf8_fix + 1);
		i+=utf8_fix;
	}

	terminal->writeText(BGNORMAL);
	terminal->writeText(NORMAL);
}

int main() {

	PUI::Terminal term;
	term.init();
	
	//El clear no funciona bien
	term.clear();

	term.hideCursor();

	term.writeText(BWHITE);
	drawTextFramed(&term, "Esto se ha escrito mientras el otro hilo está funcionando");
	
	term.moveCursor(term.cols/2 - 32, term.rows/2 + 2);
	term.writeText("Pulsa q para salir");

	//Para que quede la pantalla entera limpia
	term.moveCursor(term.cols, term.rows);

	//Muy importante cerrar antes de acabar el programa;
	term.close();

	return 0;
}
