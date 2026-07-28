#ifndef DETECTORGEOMETRYDEFAULT_HH 
#define DETECTORGEOMETRYDEFAULT_HH 

#include "G4LogicalVolume.hh"

class DetectorGeometryDefault
{
public:
    virtual ~DetectorGeometryDefault() = default;
    virtual void Build(G4LogicalVolume* logicWorld);
};

#endif