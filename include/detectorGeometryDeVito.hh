#ifndef DETECTRGEOMETRYDEVITO_HH
#define DETECTRGEOMETRYDEVITO_HH

#include "detectorGeometryDefault.hh"

class DetectorGeometryDeVito : public DetectorGeometryDefault
{
public:
    DetectorGeometryDeVito(std::string detector);
    ~DetectorGeometryDeVito();
    void Build(G4LogicalVolume* logicWorld) override;

private:
    std::string fDetector;
};


#endif