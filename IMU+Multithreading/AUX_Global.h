// Execute the keypad command by changing global variables
void ExecuteKeypadCommand(){
	char c;
	c = getchar();
	if (c == 'P' || c =='p')
		{	flag_printf = true;}
	else if (c == 'S' || c =='s')
		{flag_printf = false;}
}

// #endif // AUX_GLOBAL_H
