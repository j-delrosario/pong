#include "GLCore.h"
#include "GameLayer.h"

using namespace GLCore;

class Pong : public Application
{
public:
	Pong()
	{
		PushLayer(new GameLayer());
	}
};

int main()
{
	std::unique_ptr<Pong> app = std::make_unique<Pong>();
	app->Run();
}