#include <iostream>

#include "Field.h"
#include "SDLField.h"

int main(int argc, char* argv[])
{
	Field* field = nullptr;
	SDLField* sdlField = nullptr;
	try
	{
		field = new Field();
		sdlField = new SDLField(field);
	}
	catch (std::string& error)
	{
		std::cout << error << std::endl;
		return -1;
	}

	while (sdlField->IsRunning())
	{
		sdlField->Update();
	}

	delete sdlField;
	delete field;
	
	return 0;
}