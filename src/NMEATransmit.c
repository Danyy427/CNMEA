#include "NMEALib.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/*
    Creates a valid NMEA packet from a string - TESTED
*/

char *NMEAClauseToString(NMEAClause_t *clause)
{
    int datalen = strlen(clause->dataFields);
    char *NmeaClause = malloc(9 + datalen + 2 + 1);
    NmeaClause[0] = clause->signatureDelimiter;
    memcpy(NmeaClause + 1, &clause->talkerId, 5);

    for (size_t i = 0; i < datalen; i++)
    {
        NmeaClause[i + 6] = clause->dataFields[i];
    }

    NmeaClause[datalen + 6] = clause->checksumDelimiter;
    char checksumStr[3] = {0};
    sprintf(checksumStr, "%02x", clause->checksum);
    memcpy(NmeaClause + datalen + 7, checksumStr, 2);
    NmeaClause[datalen + 9] = '\r';
    NmeaClause[datalen + 10] = '\n';
    NmeaClause[datalen + 11] = 0;

    return NmeaClause;
}

/*
    Creates a valid NMEA packet from a string and fills it into a buffer - NOT TESTED
*/

void NMEAClauseToStringBuffer(NMEAClause_t *clause, char *buffer)
{
    int datalen = strlen(clause->dataFields);
    buffer[0] = clause->signatureDelimiter;
    memcpy(buffer + 1, &clause->talkerId, 5);

    for (size_t i = 0; i < datalen; i++)
    {
        buffer[i + 6] = clause->dataFields[i];
    }

    buffer[datalen + 6] = clause->checksumDelimiter;
    char checksumStr[3] = {0};
    sprintf(checksumStr, "%02x", clause->checksum);
    memcpy(buffer + datalen + 7, checksumStr, 2);
    buffer[datalen + 9] = '\r';
    buffer[datalen + 10] = '\n';
    buffer[datalen + 11] = 0;
}

/*
    Initializes a transmitter function - NOT TESTED
*/

static void (*NMEATransmitter)(void *, char *, short, int);
static int isInitialized = 0;
void InitTransmitter(void (*transmitter)(void *, char *, short, int))
{
    NMEATransmitter = transmitter;
    isInitialized = 1;
}

/*
    Transmits a clause - NOT TESTED
*/

void TransmitNMEAPacket(void *channel, NMEAClause_t *clause)
{
    if (isInitialized == 0)
        return;

    char *str = NMEAClauseToString(clause);
    for (size_t i = 0; i < strlen(str); i++)
    {
        NMEATransmitter(channel, &str[i], 1, 0xFF);
    }
}