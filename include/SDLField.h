#ifndef SDL_FIELD_H
#define SDL_FIELD_H

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "Field.h"
#include <vector>

struct SDLNode
{
	SDL_FRect rect;
	const Node* node;
};

class SDLField
{
public:
	SDLField(Field* field);
	~SDLField();

	bool IsRunning() const;

	void Update();
	void Reset();

private:
	void ClearData();

	void HandleInput(const SDL_Event& event);
	void RenderNodes();

private:
	Field* m_field = nullptr;

	std::vector<SDLNode*> m_nodes;

	// SDL
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	static constexpr const char* WINDOW_NAME = "Visual A-Star";
	static constexpr int WINDOW_WIDTH = 1280;
	static constexpr int WINDOW_HEIGHT = 800;
	static constexpr float NODE_PADDING = 3.0f;

	bool m_isRunning = true;

	// delta time
	int lastTime = 0;
	static constexpr float STEP_DELAY = .0051f;	// step time
	float timeUntilStep = STEP_DELAY;
};

#endif