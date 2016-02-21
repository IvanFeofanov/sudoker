#include "su_program_state.h"

SuProgramState::SuProgramState()
{
    setDefault();
}

void SuProgramState::setDefault()
{
    //mode
    isSolvingMode = true;
    isCapturingMode = false;

    //source
    source = SOURCE_VIDEO_MODE;

    //browsing
    isBrowsingState = false;
}
