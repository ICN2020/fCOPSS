fCOPSS
===
COPSS is Content-Oriented Pub/Sub System.  
So, whait is 'f' ?  
It's flexible, fundamental, feasible or somthing as you thought.

## Build

### Prerequisites
* Ubuntu 16.04
* ndn-cxx 0.6.2 or later : <https://github.com/named-data/ndn-cxx>

### Compile and Install
```
  cd src
  make 
  sudo make install
```

## Usage Example
Assume that all programs are running on the same host.

####  1. Run router program
```
     router
```
 
####  2. Run RP program
```
     rendezvous
```
 
####  3. Create FIB entry on router for RP 
```
     rtctrl fibadd -n / -t tcp -r 127.0.0.1:9878 -c 0
```
 
####  4. Run subscriber
```
     subscribe -c '/1(/2/3/0)/sports/tennis' -o output.txt
```
 
####  5. Run publihser
```
     publish -c '/1/2/sports/tennis' -i input.txt
```

  subscriber will receive a content of input.txt and write it to output.txt .
 
```
     publish -c '/1/3/sports/tennis' -i input.txt
```

  subscriber will not receive any content.


## License

This software is released under the MIT License, see LICENSE.txt.

## Contributors

- Atsushi Tagami (KDDI Research, Inc.)
- Yuki Koizumi (Osaka University)
- Toru Hasegawa (Osaka University)
