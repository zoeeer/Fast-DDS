Run the example in the console: ./rtps_intra (or rtps_intra.exe on windows).
Type Ctrl-C to stop.

To verify intra-process delievery is used:
- run command `ls /dev/shm` to observe shared memory files: no files appear when running reader and writer
- use wireshark like software to observe network traffic: no user data traffic is captured
