#ifndef ACTIONINITIALIZATION_HH
#define ACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

class MyActionInitialization : public G4VUserActionInitialization {
public:
    MyActionInitialization();
    virtual ~MyActionInitialization() override = default;

    virtual void BuildForMaster() const override;
    
    virtual void Build() const override;
};

#endif