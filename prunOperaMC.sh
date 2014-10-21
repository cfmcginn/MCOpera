#!/bin/bash                                                                           

if [ $# -ne 0 ]
then
  echo "Usage: ./prunOperaMC.sh"
  return
fi

now="mcOperaJob_$(date +"%m_%d_%Y__%H_%M_%S")"
mkdir $now

NAME="runOperaMC.cpp"
g++ $NAME `root-config --cflags --libs` -Werror -Wall -O2 -o "${NAME/%.cpp/}.exe" -I/usr/X11R6/include -I/Users/cfmcginn/CPPLibraries/boost_1_56_0/ -I/Users/cfmcginn/root/roofit/roofitcore/inc -I/Users/cfmcginn/root/roofit/roofit/inc -L $ROOTSYS/lib -lRooFit -lHtml -lMinuit

mv *.exe $now