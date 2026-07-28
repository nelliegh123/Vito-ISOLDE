#ifndef DETECTRGEOMETRYDEVITO_HH
#define DETECTRGEOMETRYDEVITO_hH

// #include "G4LogicalVolume.hh"
#include "detectorGeometryDefault.hh"


class DetectorGeometryDeVITO : public DetectorGeometryDefault
{
public:
    void Build(G4LogicalVolume* logicWorld) override;
};


#endif