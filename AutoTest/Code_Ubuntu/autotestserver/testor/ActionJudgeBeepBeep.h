#ifndef ACTIONJUDGEBEEPBEEP_H
#define ACTIONJUDGEBEEPBEEP_H

#include "Action.h"
#include <GetBeepLen.h>

class ActionJudgeBeepBeep : public Action, public GetBeepLen
{
private:
    const std::string _filePath;
    BeepInfo* t1;
    BeepInfo* t2;

public:
    ActionJudgeBeepBeep(int timeout, std::string filePath,  int type1,int type2)
        : Action(timeout)
        ,_filePath(filePath)
        , t1(new BeepInfo(type1))
        , t2(new BeepInfo(type2))
    {

    }

    virtual ~ActionJudgeBeepBeep()
    {
        if (this->t1 != NULL) {
            delete this->t1;
            this->t1 = NULL;
        }
        if (this->t2 != NULL) {
            delete this->t2;
            this->t2 = NULL;
        }
    }

    virtual bool run(std::string casename, int seq) override;
};

#endif /* ACTIONJUDGEBEEPBEEP_H */
