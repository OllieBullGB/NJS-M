#ifndef SNRPINGAPP_SNRReportElement_H_
#define SNRPINGAPP_SNRReportElement_H_

#include <omnetpp.h>
#include "inet/common/INETDefs.h"
#include "inet/common/INETUtils.h"
#include "inet/mobility/contract/IMobility.h"

using namespace inet;

class SNRReportElement
{
    public:
        SNRReportElement();
        virtual ~SNRReportElement();

        SNRReportElement(const Coord& position, double rsuTransmissionStrength, double jammerTransmissionStrength, double snr, simtime_t timestamp, const char* node): position(position), rsuTransmissionStrength(rsuTransmissionStrength), jammerTransmissionStrength(jammerTransmissionStrength), snr(snr), timestamp(timestamp), node(node) {};
        Coord getPosition() const { return position; }
        double getRsuTransmissionStrength() const { return rsuTransmissionStrength; };
        double getJammerTransmissionStrength() const { return jammerTransmissionStrength; };
        double getSNR() const { return snr; }
        simtime_t getTimestamp() const { return timestamp; }
        const char* getNode() const { return node; }

        std::string toString() const
        {
            std::stringstream ss;
            ss << "Node: " << node
               << "Position: " << position.str()
               << ", SNR Level: " << snr
               << ", Timestamp: " << timestamp;
            return ss.str();
        }

    protected:
        Coord position;
        double rsuTransmissionStrength;
        double jammerTransmissionStrength;
        double snr;
        simtime_t timestamp;
        const char* node;

};

#endif /* SNRPINGAPP_SNRReportElement_H */
