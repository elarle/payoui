#include "lib/payoui.hpp"

int main() {

	PUI::Terminal term;
	term.init();
	
	//El clear no funciona bien
	term.clear();

	term.hideCursor();

	term.moveCursor(term.cols/2 - 32, term.rows/2);
	term.fastTextWrite("Esto se ha escrito mientras el otro hilo está funcionando");
	term.moveCursor(term.cols/2 - 32, term.rows/2 + 1);
	term.fastTextWrite("Pulsa q para salir");
	
	//Para que quede la pantalla entera limpia
	term.moveCursor(term.cols, term.rows);

	//Muy importante cerrar antes de acabar el programa;
	term.close();

	return 0;
}
