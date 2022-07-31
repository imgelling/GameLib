#pragma once
// INCLUDES ////////////////////////////////////////////////////////////////////
#ifdef __linux__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <string>

class GameKeyboard {
public:
	//bool isKeyDown(const char *);
	//bool isKeyUp(const char *);
	GameKeyboard();
	~GameKeyboard();
	std::string GetInputText();
	void ResetInputText();
	void StartInputText();
	void EndInputText();
	void AppendInputText(std::string);
	void BackspaceInputText();
	void ReturnInputText();
	void ResetLastInputText();
	std::string GetLastInputText();
	bool TextInputIsOn();
	bool WasPressed(const char *);
	bool WasReleased(const char *);
	bool isHeld(const char *);
private:
	const uint8_t* currentKeyStates;
	uint8_t* lastKeyStates;

	int keys;
	std::string inputString;
	std::string lastInput;
	bool isTextInputOn;
};
