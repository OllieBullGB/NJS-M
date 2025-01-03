//
// Generated file, do not edit! Created by nedtool 5.7 from NJSWirelessHost/NJSReport.msg.
//

#ifndef __NJSREPORT_M_H
#define __NJSREPORT_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0507
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>NJSWirelessHost/NJSReport.msg:1</tt> by nedtool.
 * <pre>
 * packet NJSReport
 * {
 *     int someField;
 *     string anotherField;
 * }
 * </pre>
 */
class NJSReport : public ::omnetpp::cPacket
{
  protected:
    int someField;
    ::omnetpp::opp_string anotherField;

  private:
    void copy(const NJSReport& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const NJSReport&);

  public:
    NJSReport(const char *name=nullptr, short kind=0);
    NJSReport(const NJSReport& other);
    virtual ~NJSReport();
    NJSReport& operator=(const NJSReport& other);
    virtual NJSReport *dup() const override {return new NJSReport(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getSomeField() const;
    virtual void setSomeField(int someField);
    virtual const char * getAnotherField() const;
    virtual void setAnotherField(const char * anotherField);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const NJSReport& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, NJSReport& obj) {obj.parsimUnpack(b);}


#endif // ifndef __NJSREPORT_M_H

