#include <iostream>

#include "Field.h"
#include "SDLField.h"

int main(int argc, char* argv[])
{
	Field f;
	SDLField field(&f);

	while (field.IsRunning())
	{
		field.Update();
	}

	return 0;
}