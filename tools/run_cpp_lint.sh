#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 {root folder path}";
  exit 1;
fi

python "$1"/tools/cpplint/cpplint.py --extensions=cpp,hpp --filter=-legal/copyright $(find "$1"/include "$1"/src -name *.cpp -o -name *.hpp) | grep -v 'Done processing'
