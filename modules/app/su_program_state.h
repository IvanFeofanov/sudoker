#ifndef SU_PROGRAM_STATE_H
#define SU_PROGRAM_STATE_H

class SuProgramState
{
public:
    SuProgramState();

    //mode
    bool isSolvingMode;
    bool isCapturingMode;

    //source
    int source;

    enum{
        SOURCE_IMAGE_FROM_FILE = 0,
        SOURCE_VIDEO_MODE      = 1
    };

    //browsing state
    bool isBrowsingState;

    void setDefault();
};

#endif // SU_PROGRAM_STATE_H
