#ifndef _NMEAMACRO_H_
#define _NMEAMACRO_H_

#define IS_COMMAND(clause, command) if (memcmp(clause->talkerId, command, 2) == 0 && memcmp(clause->sentenceFormatter, command + 2, 3) == 0)

#endif