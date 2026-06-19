#include "physicsList.hh"

MyPhysicsList::MyPhysicsList()
{
    RegisterPhysics (new G4EmStandardPhysics()); //Not sure if I need this
    RegisterPhysics (new G4OpticalPhysics()); 
}

MyPhysicsList::~MyPhysicsList()
{}