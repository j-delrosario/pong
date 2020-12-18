#include "GLCore.h"
#include "ExampleLayer.h"
#include "BatchEx1Layer.h"
#include "BatchColourLayer.h"
#include "BatchTexLayer.h"
#include "BatchDynamicLayer.h"
#include "RendererExampleLayer.h"

using namespace GLCore;

class Example : public Application
{
public:
	Example()
		: Application("OpenGL Examples")
	{
		//PushLayer(new ExampleLayer());
		//PushLayer(new BatchEx1Layer());
		//PushLayer(new BatchColourLayer());
		//PushLayer(new BatchTexLayer());
		//PushLayer(new BatchDynamicLayer());
		PushLayer(new RendererExampleLayer());
	}
};

int main()
{
	std::unique_ptr<Example> app = std::make_unique<Example>();
	app->Run();

}