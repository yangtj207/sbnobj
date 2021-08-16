#include <cassert>
#include <string>
#include <map>
#include <vector>
#include "TMatrixD.h"
#include "CLHEP/Random/RandGaussQ.h"
#include "EventWeightParameterSet.h"

namespace sbn {
  namespace evwgh {

void EventWeightParameterSet::Configure(std::string name, ReweightType rwtype, size_t nuni) {
  fName = name;
  fRWType = rwtype;
  fNuniverses = nuni;

  if (fRWType == kMultisim) {
    fNuniverses = nuni;
  }
  else if (fRWType == kPMNSigma) {
    fNuniverses = 2;
  }
  else if (fRWType == kFixed) {
    fNuniverses = 1;
  }
  else {
    std::cerr << "EventWeightParameterSet: Unknown reweight type " << fRWType << std::endl;
    assert(false);
  }
}


void EventWeightParameterSet::Configure(std::string name, std::string rwtype_string, size_t nuni) {
  if (rwtype_string == "multisim") Configure(name, kMultisim, nuni);
  else if (rwtype_string == "pmNsigma") Configure(name, kPMNSigma);
  else if (rwtype_string == "fixed") Configure(name, kFixed);
  else {
    std::cerr << "EventWeightParameterSet: Unknown reweight type " << rwtype_string << std::endl;
    assert(false);
  }
}


void EventWeightParameterSet::AddParameter(
    std::string name, float width1, float mean, float width2, size_t covIndex) {
  EventWeightParameter p(name, mean, width1, width2, covIndex);
  fParameterMap.insert({ p, std::vector<float>() });
}


void EventWeightParameterSet::Sample(CLHEP::HepRandomEngine& engine) {
  if (fRWType == kDefault) {
    std::cerr << "EventWeightParameterSet: Must be configured before sampling." << std::endl;
    assert(false);
  }

  // No covariance matrix, uncorrelated sampling
  if (!fCovarianceMatrix) {
    for (auto& it : fParameterMap) {
      const EventWeightParameter& p = it.first;

      if (fRWType == kMultisim) {
        for (size_t i=0; i<fNuniverses; i++) {
          float r = p.fWidth1*CLHEP::RandGaussQ::shoot(&engine, p.fMean, 1);
          it.second.push_back(r);
        }
      }

      else if (fRWType == kPMNSigma) {
        it.second.push_back(p.fMean + p.fWidth1);
        it.second.push_back(p.fMean + p.fWidth2);
      }

      else if (fRWType == kFixed) {
        it.second.push_back(p.fMean + p.fWidth1);
      }

      else {
        std::cerr << "EventWeightParameterSet: Unknown reweight type " << fRWType << std::endl;
        assert(false);
      }
    }
  }

  // Multivariate Gaussian sampling
  else {
    std::cerr << "EventWeightParameterSet: Multivariate Gaussian sampling is not implemented." << std::endl;
    assert(false);
  }
}

  }  // namespace evwgh
}  // namespace sbn

