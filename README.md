# Tether
Tether is a specific formed signals and slots system based on Boost.Signals2.
Basic elements are TetherSig, Tether, Hooker. Tether keeps a value and only a connected TetherSig can update the value. Hooker hooks value. 
It looks like:

sig1, [sig2, sig3...] → tether ⇄ hooker

Sample codes are here. https://github.com/fictheader/tether/blob/master/TetherTest/main.cpp

## Author
kozmof
 
## License
Boost Software Licence https://www.boost.org/LICENSE_1_0.txt
