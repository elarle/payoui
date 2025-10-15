#ifndef PAYOUI
#define PAYOUI

#include <stdio.h>
#include "definitions.h"

#if __linux__

#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

#endif

namespace PUI {

	struct Terminal{

		size_t cols = 0;
		size_t rows = 0;

		void init(){

			struct winsize size;
			ioctl(1, TIOCGWINSZ, &size);
			cols = size.ws_col;
			rows = size.ws_row;	
			
			this->startRawMode();
			setbuf(stdout, nullptr);

		}

		void clear(){
			write(STDOUT_FILENO, "\033[2J\033[H", 7);
		}

		void fastWrite(const char * data, size_t length){
			write(STDOUT_FILENO, data, length);
		}

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

		void startRawMode(){

			//Only linux
			termios t;
			tcgetattr(STDIN_FILENO, &t);
			t.c_cflag &= ~(ICANON | ECHO);
			tcsetattr(STDIN_FILENO, TCSANOW, &t);

		}

		void stopRawMode(){

			//Only linux
			termios t;
			tcgetattr(STDIN_FILENO, &t);
			t.c_cflag |= ~(ICANON | ECHO);
			tcsetattr(STDIN_FILENO, TCSANOW, &t);

		}

		void close(){

			this->stopRawMode();

		}
	};
}

#endif

