#include <Primech.h>

class Sandbox : public PriMech::Application
{
public:
	Sandbox();
	~Sandbox();

private:

};

Sandbox::Sandbox()
{

}

Sandbox::~Sandbox()
{

}

PriMech::Application* PriMech::CreateApplication() {
	return new Sandbox();
}