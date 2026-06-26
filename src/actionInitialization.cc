#include "actionInitialization.hh"
#include "runAction.hh"               
#include "eventAction.hh"        
#include "primaryGeneratorAction.hh" 

MyActionInitialization::MyActionInitialization() : G4VUserActionInitialization() {}

void MyActionInitialization::BuildForMaster() const {

    SetUserAction(new runAction()); 
}

void MyActionInitialization::Build() const {

    SetUserAction(new runAction());

    SetUserAction(new eventAction());

    SetUserAction(new MyPrimaryGenerator());
} 


