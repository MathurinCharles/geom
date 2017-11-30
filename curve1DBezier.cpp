#include "curve1DBezier.h"
#include "scene.h"

unsigned int Curve1DBezier::nbFrames() {
    return Scene::get()->nbFrames();
}
