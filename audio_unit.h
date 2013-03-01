#include <AudioUnit/AudioComponent.h>
#include <AudioUnit/AudioUnit.h>
#include <CoreFoundation/CoreFoundation.h>

int GetAudioUnit(AudioComponentInstance *ci, void *rendererFunction, void *rendererData);
int startPlay(AudioUnit *c);
int stopPlay(AudioUnit *c);
int closeUnit(AudioUnit *c);
