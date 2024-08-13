#if defined(_WIN32)
#define QUEST_API __declspec(dllexport)
#elif defined(__wasm__)
#include <emscripten.h>
#define QUEST_API EMSCRIPTEN_KEEPALIVE
#else
#define QUEST_API
#endif
