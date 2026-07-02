#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"

class G4Run; 

class runAction : public G4UserRunAction 
{
public:
    runAction();
    virtual ~runAction() override = default;

    virtual void BeginOfRunAction(const G4Run*) override;
    virtual void EndOfRunAction(const G4Run* run) override;

private:
    // G4int fNEvents = 0;
};

#endif