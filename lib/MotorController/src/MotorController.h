#ifndef MOTORCONTROLLER_H_
#define MOTORCONTROLLER_H_

#include <Module.h>

class MotorController : public Module {
    public:
	    const char *getName();
        virtual void setup();
        virtual void everySecond();

    private:

};


#endif