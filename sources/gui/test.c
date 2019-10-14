#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>


int				ft_getch(void)
{
	struct termios	oldt;
	struct termios	newt;
	char			ch;

	ch = 0;
	tcgetattr(0, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(0, TCSANOW, &newt);
	if ((read(0, &ch, 1)) <= 0)
		ch = (char)0xff;
	tcsetattr(0, TCSANOW, &oldt);
	return ((int)ch);
}

int main()
{
	char	ch;

	while ((ch = ft_getch()) != '\n')
	{
		printf("%d\n", (int)ch);
		printf("\x1B[6n\n");
	}
}
