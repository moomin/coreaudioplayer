#include <AudioUnit/AudioComponent.h>
#include <AudioUnit/AudioUnit.h>
#include <CoreFoundation/CoreFoundation.h>

int GetAudioUnit(AudioComponentInstance *ci);
int startPlay(AudioUnit *c);
int stopPlay(AudioUnit *c);
int closeUnit(AudioUnit *c);

double renderPhase;
