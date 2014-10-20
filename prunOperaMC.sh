#!/bin/bash                                                                           

if [ $# -ne 0 ]
then
  echo "Usage: ./prunOperaMC.sh"
  return
fi

now="mcOperaJob_$(date +"%m_%d_%Y__%H_%M_%S")"
mkdir $now

NAME="runOperaMC.cpp"
g++ $NAME -Werror -Wall -O2 -o "${NAME/%.cpp/}.exe" -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -I/Users/cfmcginn/CPPLibraries/boost_1_56_0/

mv *.exe $now