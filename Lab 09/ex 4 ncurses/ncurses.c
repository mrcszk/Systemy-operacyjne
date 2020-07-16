#include <stdlib.h>   /* exit() */
#include <unistd.h>   /* sleep() */
#include <curses.h>

int main(void) 
{
SCREEN *ekran;

ekran= newterm(NULL, stdout, stdin);
if (ekran == NULL) {
	fprintf(stderr,"Error: newterm()\n");
	exit(1);
}
/* Ustawienie "ekran" jako biezacego terminalu */
if (set_term(ekran) == NULL) {
/* Jezeli sie nie uda, to i tak trzeba zwolnic pamiec */
	endwin();
	delscreen(ekran);  /* zwolnienie pamieci  */
	fprintf(stderr,"Error: set_term()\n");
	exit(1);
}
printw("Hello! To okno curses stworzono przez newterm()\n");
refresh();
sleep(2);

printw("Koniec pracy...\n");
refresh();
sleep(2);

endwin();
delscreen(ekran);
return 0;
}
