#include "lib/payoui.hpp"

int main() {

	PUI::Terminal term;
	term.init();
	
	//El clear no funciona bien
	term.clear();

	//TODO Implementar funcion para solo colores

	term.moveCursor(term.cols / 2, term.rows / 2);
   	term.fastColorWrite(BBLUE, "PAYO", 4);

	term.moveCursor((term.cols / 2), (term.rows / 2) + 1);
	term.fastColorWrite(BLUE, "MAN", 3);

	term.moveCursor((term.cols / 2), (term.rows / 2) + -1);
	term.fastColorWrite(RED, "SUPER", 5);

	term.fastWrite(NORMAL, 8);
	term.moveCursor(term.cols, term.rows);

	term.close();

	return 0;
}
