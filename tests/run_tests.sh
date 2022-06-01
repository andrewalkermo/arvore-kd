#!/bin/bash

for file in tests/*.sh; do
  if [ $file != "tests/run_tests.sh" ]; then
    echo "$file"
    $file
  fi
done