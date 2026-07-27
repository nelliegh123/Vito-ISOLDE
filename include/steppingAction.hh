#ifndef STEPPINGAXCTION_HH
#define STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"

class eventAction;

class steppingAction : public G4UserSteppingAction
{
public:
    steppingAction(eventAction* evtAction);
    ~steppingAction() override;

    void UserSteppingAction(const G4Step* step) override;

private:
    eventAction* fEventAction;
};

#endif 