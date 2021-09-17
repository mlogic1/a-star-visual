#ifndef SDL_FIELD_H
#define SDL_FIELD_H

#include "Field.h"
#include "Node.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>

struct SDLNode
{
	SDL_FRect rect;
	const Node* node;
};

class SDLField
{
public:
	SDLField(Field* f);
	~SDLField();

	bool IsRunning() const;

	void Reset();
	void Update();

private:
	void HandleInput(const SDL_Event& event);
	void RenderNodes();

private:
	Field* m_field = nullptr;

	// Field
	int m_fieldRows;
	int m_fieldColumns;

	float m_nodeWidth;
	float m_nodeHeight;

	std::vector<SDLNode*> m_nodes;


	// SDL
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	static constexpr const char* WINDOW_NAME = "Visual A-STAR";
	static constexpr int WINDOW_WIDTH = 1280;
	static constexpr int WINDOW_HEIGHT = 800;

	bool m_isRunning = true;

	// delta
	int lastTime = 0;
	static constexpr float STEP_DELAY = 0.010f;
	float timeUntilStep = STEP_DELAY;
};

#endif