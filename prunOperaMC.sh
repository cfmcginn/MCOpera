#!/bin/bash                                                                           

if [ $# -ne 0 ]
then
  echo "Usage: ./prunOperaMC.sh"
  return
fi

now="mcOperaJob_$(date +"%m_%d_%Y__%H_%M_%S")"
mkdir $now

NAME="runOperaMC.cpp"
g++ $NAME `root-config --cflags --libs` -Werror -Wall -O2 -o "${NAME/%.cpp/}.exe" -I/opt/X11/include/ -I/Users/ChrisMcGinn/CPPLibraries/boost_1_56_0/

mv *.exe $now