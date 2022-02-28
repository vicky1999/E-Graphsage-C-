#ifndef DATAMODEL_HPP
#define DATAMODEL_HPP

#include<string.h>
using namespace std;

class DataModel {
    public:
        string flowID;
        string sourceIP;
        string sourcePort;
        string destinationIP;
        string destinationPort;
        int protocol;
        string timestamp;
        int flowDuration;
        int totalFwdPackets;
        int totalBackwardPackets;
        float totalLengthOfFwdPackets;
        float totalLengthOfBwdPackets;
        float fwdPacketLengthMax;
        float fwdPacketLengthMin;
        float fwdPacketLengthMean;
        float fwdPacketLengthStd;
        float bwdPacketLengthMax;
        float bwdPacketLengthMin;
        float bwdPacketLengthMean;
        float bwdPacketLengthStd;
        float flowBytes;
        float flowPackets;
        float flowIATMean;
        float flowIATStd;
        float flowIATMax;
        float flowIATMin;
        float fwdIATTotal;
        float fwdIATMean;
        float fwdIATStd;
        float fwdIATMax;
        float fwdIATMin;
        float bwdIATTotal;
        float bwdIATMean;
        float bwdIATStd;
        float bwdIATMax;
        float bwdIATMin;
        int fwdPSHFlags;
        int bwdPSHFlags;
        int fwdURGFlags;
        int bwdURGFlags;
        int fwdHeaderLength;
        int bwdHeaderLength;
        float fwdPackets;
        float bwdPackets;
        float minPacketLength;
        float maxPacketLength;
        float packetLengthMean;
        float packetLengthStd;
        float packetLengthVariance;
        int FINFlagCount;
        int SYNFlagCount;
        int RSTFlagCount;
        int PSHFlagCount;
        int ACKFlagCount;
        int URGFlagCount;
        int CWEFlagCount;
        int ECEFlagCount;
        float DownUpRatio;
        float averagePacketSize;
        float avgFwdSegmentSize;
        float avgBwdSegmentSize;
        int fwdHeaderLengths;
        int fwdAvgBytesBulk;
        int fwdAvgPacketsBulk;
        int fwdAvgBulkRate;
        int bwdAvgBytesBulk;
        int bwdAvgPacketsBulk;
        int bwdAvgBulkRate;
        int subflowFwdPackets;
        int subflowFwdBytes;
        int subflowBwdPackets;
        int subflowBwdBytes;
        int init_Win_bytes_forward;
        int init_Win_bytes_backward;
        int act_data_pkt_fwd;
        int min_seg_size_forward;
        float activeMean;
        float activeStd;
        float activeMax;
        float activeMin;
        float idleMean;
        float idleStd;
        float idleMax;
        float idleMin;
        int label;

        void createDataModel(DataModel *dm, vector<string> row);
};

#endif