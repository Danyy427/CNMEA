#ifndef _NMEALIB_H_
#define _NMEALIB_H_

#ifdef __cplusplus
extern “C”
{
#endif

    typedef char checksum_t;

    typedef struct NMEAClause
    {
        char signatureDelimiter;
        char talkerId[2];
        char sentenceFormatter[3];
        char *dataFields;
        char checksumDelimiter;
        checksum_t checksum;
        char terminator[2];
    } NMEAClause_t;

    typedef struct NMEAPacketFlag
    {
        char packetValid : 1;
        char packetReceived : 1;
        char packetTransmitted : 1;
        char packetSuccess : 1;
        char packetIncorrectData : 1;
        char packetIncorrectFormat : 1;
        char packetIncorrectChecksum : 1;
        char packetCorrupt : 1;
    } __attribute__((packed)) NMEAPacketFlag_t;

    typedef struct DataField
    {
        char *field;
        int length;
    } __attribute__((packed)) DataField_t;

    checksum_t CalculateNMEAChecksum(NMEAClause_t * clause);

    checksum_t CalculateNMEAChecksumFromString(char *clause);

    NMEAPacketFlag_t ValidatePacket(NMEAClause_t * clause);

    DataField_t *GetNMEADataPtr(NMEAClause_t * clause, int *resultCount);

    DataField_t *GetNMEADataArray(NMEAClause_t * clause, int *resultCount);

    void DisposeDataPtr(DataField_t * fields);

    void DisposeDataArray(DataField_t * fields, int len);

    NMEAClause_t CreateNMEAClauseFromParam(char *talkerId, char *format, char *data);

    NMEAClause_t CreateNMEAClauseFromString(char *string);

    char *NMEAClauseToString(NMEAClause_t * clause);

    void InitTransmitter(void (*transmitter)(void *, char *, short, int));

    void TransmitNMEAPacket(void *channel, NMEAClause_t *clause);

#ifdef __cplusplus
}
#endif

#endif