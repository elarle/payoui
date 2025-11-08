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


	template <typename CTX>
	struct Terminal{

		TerminalStatus status;
		CTX * context = nullptr;

		std::thread * listener_thread = nullptr;

		void (*onKeyPress)(Terminal *, char) = nullptr;

		size_t cols = 0;
		size_t rows = 0;

		int cursor_x = 0;
		int cursor_y = 0;

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

		void writeBytes(const void * data, size_t length){
			write(STDOUT_FILENO, data, length);
		}

		/**
		 * Esto debe ser llamado solo con texto estático
		 */
		void writeText(const char * data){
			size_t length = strlen(data);
			write(STDOUT_FILENO, data, length);
		}

		//PRE={00 <= red <= 255;0 <= green <= 255; <= blue <= 255;}
		//POST={Sets the text foreground to the specified color}
		void setForegroundRGB(uint8_t red, uint8_t green, uint8_t blue){
			char text_buffer[32];
			size_t buffer_length = snprintf(
				text_buffer, 
				sizeof(text_buffer), 
				"\033[38;2;%u;%u;%um",
				red, 
				green,
				blue
			);
			this->writeBytes(text_buffer, buffer_length);
		}

		//PRE={00 <= red <= 255;0 <= green <= 255; <= blue <= 255;}
		//POST={Sets the text background to the specified color}
		void setBackgroundRGB(uint8_t red, uint8_t green, uint8_t blue){
			char text_buffer[32];
			size_t buffer_length = snprintf(
				text_buffer, 
				sizeof(text_buffer), 
				"\033[48;2;%u;%u;%um",
				red, 
				green,
				blue
			);
			this->writeBytes(text_buffer, buffer_length);
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
			
			cursor_x = x;
			cursor_y = y;
		}

		void showCursor(){
			this->writeText(SHOW_CURSOR);
		}
		
		void hideCursor(){
			this->writeText(HIDE_CURSOR);
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

		void init(CTX * context_p){

			this->context = context_p;

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
				
				if(terminal->onKeyPress != nullptr){
					terminal->onKeyPress(terminal, input_char);
				}

				//Solución temporal
				if(input_char == 'q')
					terminal->status = EXITING;
			}
		}

	};
}

#endif

