#include "Game.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

int main() {
    // Heap lifetime matters: Emscripten callbacks outlive main().
    Game* game = new Game();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(
        [](void* context) { static_cast<Game*>(context)->Tick(); },
        game, 0, true);
#else
    while (!WindowShouldClose()) game->Tick();
    delete game;
#endif
    return 0;
}
