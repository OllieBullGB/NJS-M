#ifndef NJSREPORT_NJSREPORTELEMENT_H_
#define NJSREPORT_NJSREPORTELEMENT_H_

#include <omnetpp.h>
#include "inet/common/INETDefs.h"
#include "inet/common/INETUtils.h"
#include "inet/mobility/contract/IMobility.h"

#include "../NJSState/NJSState.h"

using namespace inet;

class NJSReportElement
{
    public:
        NJSReportElement();
        virtual ~NJSReportElement();

        NJSReportElement(const Coord& position, int njsState, simtime_t timestamp, const char* node): position(position), njsState(njsState), timestamp(timestamp), node(node) {};
        Coord getPosition() const { return position; }
        NJSState getNJSState() const { return static_cast<NJSState>(njsState); }
        simtime_t getTimestamp() const { return timestamp; }
        const char* getNode() const { return node; }

        std::string toString() const
        {
            std::stringstream ss;
            ss << "Node: " << node
               << "Position: " << position.str()
               << ", NJS State: " << njsState
               << ", Timestamp: " << timestamp;
            return ss.str();
        }

    protected:
        Coord position;
        int njsState;
        simtime_t timestamp;
        const char* node;

};

#endif /* NJSREPORT_NJSREPORTELEMENT_H_ */
