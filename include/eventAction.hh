#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"
#include <TFile.h>
#include <TTree.h>

class G4Event;

class eventAction:public G4UserEventAction 
{
public:
    eventAction();
    virtual ~eventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
};

#endif 