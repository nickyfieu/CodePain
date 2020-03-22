#include "CodePain.h"

#if _DEBUG
#include <vld.h>
#endif

int main(int, char* [])
{
	cp::CodePain engine;
	engine.Run();

	return 0;
}