#include "NMEALib.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/*
    Expects a valid NMEA Packet as clause or as null-terminated string - TESTED
*/

checksum_t CalculateNMEAChecksum(NMEAClause_t *clause)
{
    char checksum = 0;
    char *clausePtr = (char *)clause;

    for (size_t i = 1; i < 6; i++)
    {
        checksum ^= *(clausePtr + i);
    }

    for (size_t i = 0; i < strlen(clause->dataFields); i++)
    {
        checksum ^= *(clause->dataFields + i);
    }

    return checksum;
}

checksum_t CalculateNMEAChecksumFromString(char *clause)
{
    char checksum = 0;
    for (char *ptr = clause + 1; *ptr != '*'; ptr++)
    {
        checksum ^= *ptr;
    }

    return checksum;
}

/*
    Validates the format and correctness of a NMEA packet, checks for format and checksum - TESTED
*/

NMEAPacketFlag_t ValidatePacket(NMEAClause_t *clause)
{
    char *allowedChars = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890.,*$!";
    NMEAPacketFlag_t results = (const NMEAPacketFlag_t){0};

    if (!(clause->signatureDelimiter == '$' ||
          clause->signatureDelimiter == '!') ||
        clause->checksumDelimiter != '*')
    {
        results.packetCorrupt = 1;
        results.packetIncorrectFormat = 1;
    }

    if (clause->checksum != CalculateNMEAChecksum(clause))
    {
        results.packetCorrupt = 1;
        results.packetIncorrectChecksum = 1;
    }

    int datalen = strlen(clause->dataFields);

    if (datalen == 0)
    {
        results.packetIncorrectData = 1;
    }

    for (size_t i = 0; i < datalen; i++)
    {
        results.packetIncorrectData = 1;
        for (size_t j = 0; j < 67; j++)
        {
            if (clause->dataFields[i] == allowedChars[j])
            {
                results.packetIncorrectData = 0;
                break;
            }
        }
    }

    if (clause->terminator[0] != '\r' || clause->terminator[1] != '\n')
    {
        results.packetCorrupt = 1;
        results.packetIncorrectFormat = 1;
    }

    results.packetValid = 1;
    results.packetSuccess = 1;

    return results;
}

/*
    Separates data into datafield array - NOT TESTED
    The `field` field of Datafield points to the start of the data field in the buffer
    The `length` field is data's length
*/

void GetNMEADataBuffer(NMEAClause_t *clause, int *resultCount, DataField_t *buffer)
{
    int start = 0, count = 0;

    for (size_t i = 0; i < strlen(clause->dataFields); i++)
    {
        if (clause->dataFields[i] == ',')
            count++;
    }
    *resultCount = count;
    int fieldsIte = 0;

    for (size_t i = 0; i <= strlen(clause->dataFields); i++) // the <= doesn't look very sane
    {
        if (clause->dataFields[i] == ',' || clause->dataFields[i] == 0)
        {
            if (i != 0)
            {
                buffer[fieldsIte].field = &clause->dataFields[start + 1];
                buffer[fieldsIte].length = i - start - 1;

                fieldsIte++;
            }

            start = i;
        }
    }
}

/*
    Separates data into datafield array - TESTED
    The `field` field of Datafield points to the start of the data field in the NMEA Clause
    The `length` field is data's length
*/

DataField_t *GetNMEADataPtr(NMEAClause_t *clause, int *resultCount)
{
    int start = 0, count = 0;

    for (size_t i = 0; i < strlen(clause->dataFields); i++)
    {
        if (clause->dataFields[i] == ',')
            count++;
    }

    DataField_t *fields = malloc(sizeof(DataField_t) * count);
    *resultCount = count;
    int fieldsIte = 0;

    for (size_t i = 0; i <= strlen(clause->dataFields); i++) // the <= doesn't look very sane
    {
        if (clause->dataFields[i] == ',' || clause->dataFields[i] == 0)
        {
            if (i != 0)
            {
                fields[fieldsIte].field = &clause->dataFields[start + 1];
                fields[fieldsIte].length = i - start - 1;

                fieldsIte++;
            }

            start = i;
        }
    }

    return fields;
}

/*
    Separates data into datafield array - TESTED
    The `field` field of Datafield points to the start of the data field in a newly allocated location
    The `length` field is data's length
*/

DataField_t *GetNMEADataArray(NMEAClause_t *clause, int *resultCount)
{
    int start = 0, count = 0;

    for (size_t i = 0; i < strlen(clause->dataFields); i++)
    {
        if (clause->dataFields[i] == ',')
            count++;
    }

    DataField_t *fields = malloc(sizeof(DataField_t) * count);
    *resultCount = count;
    int fieldsIte = 0;

    for (size_t i = 0; i <= strlen(clause->dataFields); i++) // the <= doesn't look very sane
    {
        if (clause->dataFields[i] == ',' || clause->dataFields[i] == 0)
        {
            if (i != 0)
            {
                fields[fieldsIte].length = i - start - 1;

                fields[fieldsIte].field = malloc(i - start);
                memcpy(fields[fieldsIte].field, clause->dataFields + start + 1, fields[fieldsIte].length);

                fields[fieldsIte].field[fields[fieldsIte].length] = 0;

                fieldsIte++;
            }

            /* ,5,6,7,8,9 */

            start = i;
        }
    }

    return fields;
}

/*
    Adds a new data to the NMEAClause, the ',' is added automatically. - NOT TESTED
    BEWARE USES REALLOC
*/

void AddDataToNMEAClause(NMEAClause_t *clause, char *string)
{
    char *data = clause->dataFields;
    int len = strlen(data);
    int len1 = strlen(string);
    data = realloc(data, len + len1 + 2);
    strcat(data, ",");
    strcat(data, string);
}

void DisposeDataPtr(DataField_t *fields)
{
    free(fields);
}

void DisposeDataArray(DataField_t *fields, int len)
{
    for (size_t i = 0; i < len; i++)
    {
        free(fields[i].field);
    }

    free(fields);
}