#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 {root folder path}";
  exit 1;
fi

# silenced the following categories:
#   legal/copyright   No copyright needed
#   readability/todo  No user names next to todo comments needed
#   build/c++11       Google banned some headers because they have their own implementation 

python "$1"/tools/cpplint/cpplint.py --extensions=cpp,hpp --filter=-legal/copyright,-readability/todo,-build/c++11,-build/include_order,-whitespace/braces $(find "$1"/include "$1"/src -name *.cpp -o -name *.hpp) | grep -v 'Done processing'
exit ${PIPESTATUS[0]}
