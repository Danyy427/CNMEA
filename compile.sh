rm -rf ./bin
mkdir ./bin
gcc src/NMEAReceive.c -I./inc -c -o bin/NMEAReceive.o
gcc src/NMEAReceive.c -I./inc -c -o bin/NMEATransmit.o
gcc src/NMEAReceive.c -I./inc -c -o bin/NMEAUtils.o
ar rcs bin/libCNMEA.a bin/NMEAReceive.o bin/NMEATransmit.o bin/NMEAUtils.o