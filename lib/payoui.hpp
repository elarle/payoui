#ifndef PAYOUI
#define PAYOUI

#include <stdio.h>
#include "definitions.h"
#include <string.h>
#include <thread>

#if __linux__

#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

#endif

namespace PUI {

	/**
	 * Representa el estado en el que se encuentra la terminal en todo momento
	 * En cualquier caso se debe usar TerminalStatus para almacenar el estado
	 */
	typedef enum TerminalStatuses{
		STOPPED,
		RUNNING,
		EXITING
	} TerminalStatuses;
	typedef uint8_t TerminalStatus;

	struct Terminal{

		TerminalStatus status;
		std::thread * listener_thread = nullptr;

		size_t cols = 0;
		size_t rows = 0;

		void clear(){
			write(
				STDOUT_FILENO,
		 		CLEAR_ALL,
		 		strlen(CLEAR_ALL)
			);
		}

		/**
		 * =====================
		 * Operaciones con Texto
		 * =====================
		 */

		void fastBytesWrite(const void * data, size_t length){
			write(STDOUT_FILENO, data, length);
		}

		/**
		 * Esto debe ser llamado solo con texto estático
		 */
		void fastTextWrite(const char * data){
			size_t length = strlen(data);
			write(STDOUT_FILENO, data, length);
		}

		void fastColorWrite(
			const char * color,
			const char * data,
			size_t dataLength
		){

			// TODO: Impelementar un struct Color para
			// tener un control más fino sobre el color
			// del texto en la terminal
			write(STDOUT_FILENO, color, COLOR_SIZE);
			write(STDOUT_FILENO, data ,dataLength);

		}

		/**
		 * =========================
		 * Operaciones con el cursor
		 * =========================
		 */

		void moveCursor(int x, int y){

			char text_buffer[32];
			size_t buffer_length = snprintf(
				text_buffer, 
				sizeof(text_buffer), 
				"\033[%d;%dH", 
				y, 
				x
			);

			write(STDOUT_FILENO, text_buffer, buffer_length);

		}

		void showCursor(){
			this->fastTextWrite(SHOW_CURSOR);
		}
		
		void hideCursor(){
			this->fastTextWrite(HIDE_CURSOR);
		}

		/**
		 * ====================
		 * Operaciones internas
		 * ====================
		 */
		void startRawMode(){

			//Only linux
			termios t;
			tcgetattr(STDIN_FILENO, &t);
			t.c_lflag &= ~(ICANON | ECHO);
			tcsetattr(STDIN_FILENO, TCSAFLUSH, &t);

		}

		void stopRawMode(){

			//Only linux
			termios t;
			tcgetattr(STDIN_FILENO, &t);
			t.c_lflag |= ~(ICANON | ECHO);
			tcsetattr(STDIN_FILENO, TCSAFLUSH, &t);

		}

		void init(){

			struct winsize size;
			ioctl(1, TIOCGWINSZ, &size);
			cols = size.ws_col;
			rows = size.ws_row;
			
			this->startRawMode();
			setbuf(stdout, nullptr);

			this->status = RUNNING;

			this->listener_thread = new std::thread(this->listenKeys, this);

		}

		void close(){

			//Importante limpiar todo lo del hilo al acabar;
			if(this->listener_thread != nullptr){
				this->listener_thread->join();

				delete this->listener_thread;
				this->listener_thread = nullptr;
			}

			this->stopRawMode();

		}

		/**
		 * Funcion específicas de la terminal.
		 * No deberían ser usadas por el usuario.
		 */
		static void listenKeys(Terminal * terminal){
			char input_char;
			while(terminal->status == RUNNING){
				read(STDIN_FILENO, &input_char, 1);

				//TODO implementar una funcion que maneje la entrada dependiendo de una configuración.

				//Solución temporal
				if(input_char == 'q')
					terminal->status = EXITING;
			}
		}

	};
}

#endif

