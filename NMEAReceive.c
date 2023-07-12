#include "NMEALib.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/*
    Creates a valid NMEA packet from parameters
*/

NMEAClause_t CreateNMEAClauseFromParam(char *talkerId, char *format, char *data)
{
    NMEAClause_t clause;
    clause.signatureDelimiter = '$';
    memcpy(clause.talkerId, talkerId, 2);
    memcpy(clause.sentenceFormatter, format, 3);
    clause.dataFields = data;
    clause.checksumDelimiter = '*';
    clause.checksum = CalculateNMEAChecksum(&clause);
    clause.terminator[0] = '\r';
    clause.terminator[1] = '\n';
    return clause;
}

/*
    Turns string into NMEA Clause, expects string with format
    $TTFFF,DATA*HH
*/

NMEAClause_t CreateNMEAClauseFromString(char *string)
{
    NMEAClause_t clause;
    clause.signatureDelimiter = string[0];
    memcpy(clause.talkerId, string + 1, 2);
    memcpy(clause.sentenceFormatter, string + 3, 3);

    int length = strlen(string);

    clause.dataFields = malloc(length - 9 + 1);
    memcpy(clause.dataFields, string + 6, length - 9);
    clause.dataFields[length - 9] = 0;
    clause.checksumDelimiter = string[length - 3];
    clause.checksum = CalculateNMEAChecksum(&clause);
    clause.terminator[0] = '\r';
    clause.terminator[1] = '\n';
    return clause;
}