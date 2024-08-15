#if defined(_WIN32)
#define QUEST_API __declspec(dllexport)
#elif defined(__EMSCRIPTEN__)
#include <emscripten.h>
#define QUEST_API EMSCRIPTEN_KEEPALIVE __attribute__((visibility("default")))
#else
#define QUEST_API
#endif
