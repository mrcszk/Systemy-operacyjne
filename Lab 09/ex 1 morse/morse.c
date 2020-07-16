#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

int get_term_char() {
  struct termios previous_term_conf, new_term_conf;

  tcgetattr(STDIN_FILENO, &previous_term_conf);
  new_term_conf = previous_term_conf;

  new_term_conf.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_term_conf);

  int ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &previous_term_conf);
  return ch;
}
 
int main(void) {
  char *kod[]={".- ","-... ","-.-. ","-.. ",". ","..-. ","--. ",".... ",".. ",".--- ",
	        "-.- ",".-.. ","-- ","-. ","--- ",".--. ","--.- ",".-. ","... ","- ",
	        "..- ","...- ",".-- ","-..- ","-.-- ","--.. "};
  char bufor[6] = "";
  char znak;
 
    while (1) {
        znak = get_term_char();  
        while(read(0,&znak,1) && znak != '\n') {
        strcpy(bufor,kod[znak-'a']);
        write(1,bufor, 6);
        }
    }    
     
  return 0;
}
