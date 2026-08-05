#ifndef DETECTRGEOMETRYDEVITO_HH
#define DETECTRGEOMETRYDEVITO_hH

#include "detectorGeometryDefault.hh"

class DetectorGeometryDeVITOCircle : public DetectorGeometryDefault
{
public:
    DetectorGeometryDeVITOCircle(std::string detector);
    ~DetectorGeometryDeVITOCircle();
    void Build(G4LogicalVolume* logicWorld) override;

private:
    std::string fDetector;
};


#endif