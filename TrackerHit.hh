#ifndef TRACKERHIT_HH
#define TRACKERHIT_HH

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4Threading.hh"
#include "G4ThreeVector.hh"
#include "G4VHit.hh"

class TrackerHit : public G4VHit
{
    public:
        TrackerHit() = default;
        TrackerHit(const TrackerHit&)

};