#include "CodePainPCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include "SDL.h"
#include "CodePain.h"

int main(int, char*[]) {
	cp::CodePain engine;
	engine.Run();
    return 0;
}