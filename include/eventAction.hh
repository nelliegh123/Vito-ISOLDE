#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"
#include <TFile.h>
#include <TTree.h>
#include <chrono>

class G4Event;

class eventAction:public G4UserEventAction 
{
public:
    eventAction();
    virtual ~eventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    std::chrono::steady_clock::time_point GetStartTime() const { return fStartTime; }

private:
    std::chrono::steady_clock::time_point fStartTime;
};

#endif 