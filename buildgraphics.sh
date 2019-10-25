#!/bin/sh

for f in $(ls -1 source/graphics/*.png | tr '\n' '\0' | xargs -0 -n 1 basename); do
  (cd source/graphics && grit "$f" -ftc)
done
