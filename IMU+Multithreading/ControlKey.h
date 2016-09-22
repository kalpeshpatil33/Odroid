#ifndef ControlKey_H
#define ControlKey_H

bool Estimator_ON;

void ExecuteKeypadCommand(){
	char c;
	c = getch();

	if(c == 'p' || c == 'P')// start/stop printing
	{
		flag_printf = true;// start printing
	}
	else if (c == 's' || c == 'S')
	{
		flag_printf = false;// stop printing
	}
}

#endif