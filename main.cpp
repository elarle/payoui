#include "lib/payoui.hpp"

int main() {

	PUI::Terminal term;
	term.init();
	
	//El clear no funciona bien
	term.clear();

	//TODO Implementar funcion para solo colores

	term.moveCursor(term.cols / 2, term.rows / 2);
	term.fastWrite(BBLUE, 8);
	term.fastWrite("PAYO", 4);

	term.moveCursor((term.cols / 2), (term.rows / 2) + 1);
	term.fastWrite(BLUE, 8);
	term.fastWrite("MAN", 3);
	

	term.moveCursor((term.cols / 2), (term.rows / 2) + -1);
	term.fastWrite(RED, 8);
	term.fastWrite("SUPER", 5);

	term.fastWrite(NORMAL, 8);
	term.moveCursor(term.cols, term.rows);

	term.close();

	return 0;
}
