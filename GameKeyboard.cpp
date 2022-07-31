// INCLUDES ////////////////////////////////////////////////////////////////////
#include "GameKeyboard.h"
#include <iostream>


// METHODS /////////////////////////////////////////////////////////////////////
GameKeyboard::GameKeyboard()
{
	currentKeyStates = 0;
	lastKeyStates = 0;
	keys = 0;
	inputString = "";
	isTextInputOn = false;
	currentKeyStates = SDL_GetKeyboardState(&keys);
	if (keys != 0)
	{
		lastKeyStates = new uint8_t[keys];
#ifdef _DEBUG
		if (keys == NULL)
			std::cout << "shit" << std::endl;
#endif
		for (int i = 0; i < keys; i++)
		{
			lastKeyStates[i] = 0;
		}
	}
}

GameKeyboard::~GameKeyboard()
{
	if (lastKeyStates != 0)
		delete [] lastKeyStates;
		
}




//bool GameKeyboard::isKeyDown(const char *name)
//{
//#ifdef _DEBUG
//	SDL_Keycode code = SDL_GetScancodeFromName(name);
//	if (code == SDLK_UNKNOWN)
//	{
//		std::cout << "\"" << name << "\" is an unknown key." << std::endl;
//		return false;
//	}
//	else
//	return currentKeyStates[code] != 0;
//#else
//	return currentKeyStates[SDL_GetScancodeFromName(name)] != 0;
//#endif
//}
//
//bool GameKeyboard::isKeyUp(const char *name)
//{
//#ifdef _DEBUG
//	SDL_Keycode code = SDL_GetScancodeFromName(name);
//	if (code == SDLK_UNKNOWN)
//	{
//		std::cout << "\"" << name << "\" is an unknown key." << std::endl;
//		return false;
//	}
//	else 
//		return currentKeyStates[SDL_GetScancodeFromName(name)] == 0;	
//#else
//	return currentKeyStates[SDL_GetScancodeFromName(name)] == 0;
//#endif
//}

void GameKeyboard::EndInputText()
{
	SDL_StopTextInput();
	isTextInputOn = false;
}

void GameKeyboard::StartInputText()
{
	SDL_StartTextInput();
	isTextInputOn = true;
}

void GameKeyboard::ResetInputText()
{
	inputString = "";
}

std::string GameKeyboard::GetInputText()
{
	return inputString;
}

void GameKeyboard::AppendInputText(std::string string)
{
	inputString += string;
}

void GameKeyboard::BackspaceInputText()
{
	if (!inputString.empty())
	{
		inputString.pop_back();
	}
}

void GameKeyboard::ReturnInputText()
{
	lastInput = inputString;
	ResetInputText();
}

void GameKeyboard::ResetLastInputText()
{
	lastInput = "";
}

std::string GameKeyboard::GetLastInputText()
{
	return lastInput;
}

bool GameKeyboard::TextInputIsOn()
{
	return isTextInputOn;
}

bool GameKeyboard::WasPressed(const char *name)
{
	SDL_Keycode code = SDL_GetScancodeFromName(name);
#ifdef _DEBUG
	if (code == SDLK_UNKNOWN)
	{
		std::cout << "\"" << name << "\" is an unknown key." << std::endl;
		return false;
	}
	else
#endif
	{
		uint8_t testc = currentKeyStates[code];
		uint8_t testl = lastKeyStates[code];
		// Save state of the key
		lastKeyStates[code] = currentKeyStates[code];

		if ((testl == 0) && (testc == 1))
		{
			// if last state was up, and current is down, key was just pressed
			return true;
		}
	}
	return false;
}


bool GameKeyboard::WasReleased(const char *name)
{
	SDL_Keycode code = SDL_GetScancodeFromName(name);
#ifdef _DEBUG
	if (code == SDLK_UNKNOWN)
	{
		std::cout << "\"" << name << "\" is an unknown key." << std::endl;
		return false;
	}
	else
#endif
	{
		uint8_t testc = currentKeyStates[code];
		uint8_t testl = lastKeyStates[code];

		// Save state of the key
		lastKeyStates[code] = currentKeyStates[code];

		if ((testl == 1) && (testc == 0))
		{
			// if last state was down, and current is up, key was just released
			return true;
		}
	}
	return false;
}

bool GameKeyboard::isHeld(const char *name)
{
	SDL_Keycode code = SDL_GetScancodeFromName(name);
#ifdef _DEBUG
	if (code == SDLK_UNKNOWN)
	{
		std::cout << "\"" << name << "\" is an unknown key." << std::endl;
		return false;
	}
	else
#endif
	{
		uint8_t testc = currentKeyStates[code];
		uint8_t testl = lastKeyStates[code];

		// Save state of the key
		lastKeyStates[code] = currentKeyStates[code];

		if ((testl == 1) && (testc == 1))
		{
			// if last state was down, and current is down, key is held
			return true;
		}
		else if ((testl == 0) && (testc == 1))
		{
			// Going to assume the key is held if it is just pressed
			// as that is why we are calling the function, to test for held
			return true;
		}
	}
	return false;
}
