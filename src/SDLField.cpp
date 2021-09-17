#include "SDLField.h"

#include <string>

SDLField::SDLField(Field* f)
{
	m_field = f;
	
	SDL_Init(SDL_INIT_VIDEO);
	m_window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	m_renderer = SDL_CreateRenderer(m_window, -1, 0);

	if (m_window == nullptr || m_renderer == nullptr)
	{
		throw std::string("Unable to start SDL");
	}

	Reset();
}

SDLField::~SDLField()
{
	for (SDLNode* node : m_nodes)
	{
		delete node;
	}
}

bool SDLField::IsRunning() const
{
	return m_isRunning;
}

void SDLField::Reset()
{
	for (SDLNode* node : m_nodes)
	{
		delete node;
	}
	m_nodes.clear();

	const FieldType& nodeField = m_field->GetField();

	static constexpr float NODE_PADDING = 3;

	float totalPaddingWidth = m_field->GetColumns() * NODE_PADDING + NODE_PADDING;
	float totalPaddingHeight = m_field->GetRows() * NODE_PADDING + NODE_PADDING;
	float fieldWidth = WINDOW_WIDTH - totalPaddingWidth;
	float fieldHeight = WINDOW_HEIGHT - totalPaddingHeight;

	float currentX = NODE_PADDING;
	float currentY = NODE_PADDING;

	m_nodeWidth = (fieldWidth / m_field->GetColumns());
	m_nodeHeight = (fieldHeight / m_field->GetRows());
	
	for (int i = 0; i < nodeField.size(); ++i)
	{
		const std::vector<Node*>& row = nodeField.at(i);
		for (const Node* node : row)
		{
			m_nodes.push_back(new SDLNode{ {currentX, currentY, m_nodeWidth, m_nodeHeight}, node});
			currentX += m_nodeWidth + NODE_PADDING;
		}
		currentY += m_nodeHeight + NODE_PADDING;
		currentX = NODE_PADDING;
	}
}

void SDLField::Update()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	HandleInput(event);

	// stepping delay
	int thisTime = SDL_GetTicks();
	float dt = ((float)thisTime - lastTime) / 1000.0f;
	lastTime = thisTime;

	timeUntilStep -= dt;
	if (timeUntilStep < 0.0f)
	{
		timeUntilStep = STEP_DELAY;
		m_field->Step();
	}

	// rendering
	SDL_RenderClear(m_renderer);
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

	RenderNodes();
	
	
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderPresent(m_renderer);
}

void SDLField::HandleInput(const SDL_Event& event)
{
	switch (event.type)
	{
		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				m_isRunning = false;
			}
			break;
		}

		case SDL_MOUSEBUTTONDOWN:
		{
			break;
		}

		case SDL_QUIT:
		{
			m_isRunning = false;
			break;
		}

		default:
		{
			break;
		}
	}
}

void SDLField::RenderNodes()
{
	const std::vector<Node*>& openNodes = m_field->GetOpenNodes();
	const std::vector<Node*>& closedNodes = m_field->GetClosedNodes();
	const std::vector<const Node*>& pathNodes = m_field->GetPathNodes();

	for (const SDLNode* node : m_nodes)
	{
		if (node->node->IsStart())
		{
			SDL_SetRenderDrawColor(m_renderer, 47, 186, 116, 255);
		}
		else if (node->node->IsTarget())
		{
			SDL_SetRenderDrawColor(m_renderer, 47, 102, 186, 255);
		}
		else if (node->node->IsObstacle())
		{
			SDL_SetRenderDrawColor(m_renderer, 173, 14, 59, 255);
		}
		else if (std::count(pathNodes.cbegin(), pathNodes.cend(), node->node))
		{
			SDL_SetRenderDrawColor(m_renderer, 15, 217, 45, 255);
		}
		else if (std::count(openNodes.begin(), openNodes.end(), node->node))
		{
			SDL_SetRenderDrawColor(m_renderer, 176, 179, 12, 255);
		}
		else if (std::count(closedNodes.begin(), closedNodes.end(), node->node))
		{
			SDL_SetRenderDrawColor(m_renderer, 110, 110, 102, 255);
		}
		else
		{
			// unexplored field
			SDL_SetRenderDrawColor(m_renderer, 240, 240, 230, 255);
		}

		SDL_RenderFillRectF(m_renderer, &node->rect);
	}
}