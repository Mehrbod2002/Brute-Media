# Brute-Media
### info
Brute Media execute file to iterate list of brute by using tor DNS
### Execute
*Following library will install :*
- libcurl3
- curl
- libssl-dev
- openssl
- libc-dev
- gcc && tor
``` bash
sudo apt-get remove libcurl4-openssl-dev && apt-get install libcurl3 libssl-dev openssl tor libcurl-dev curl gcc libc-dev
```
*Make execution file with libraries:*
``` bash
gcc brute.c -lssl -lcrypto -lcurl -o EXE.name && sudo ./EXE.name
```
### Upgradeable options :
- [x] Bypass blocked ip's
- [x] Make txt file to brute 
- [x] Iterate txt file to brute
