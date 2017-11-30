#include "curve1DBezierParam.h"
#include "scene.h"

unsigned int Curve1DBezierParam::nbFrames() {
    return Scene::get()->nbFrames();
}
