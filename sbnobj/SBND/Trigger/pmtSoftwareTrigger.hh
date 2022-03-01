#ifndef pmtSoftwareTrigger_h
#define pmtSoftwareTrigger_h

#include <cstddef>

namespace sbnd::trigger {

  struct pmtSoftwareTrigger{

    size_t nAboveThreshold;
    int triggerTimestamp; // relative to beam window start, in ns
    double totalPE;
    pmtSoftwareTrigger() {}

  };

  struct pmtInfo{ 
    double baseline;
    double baselineSigma; 
    int PE; 
    pmtInfo() {}
  };

  struct pmtPulse{
    double peak;
    double area;
    double t_start;
    double t_peak;
    double t_end;
    pmtPulse() {}
  };
} // namespace sbnd::trigger

#endif
