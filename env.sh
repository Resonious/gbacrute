#!/bin/sh
exec docker run -it --rm \
  --mount type=bind,source=$PWD,destination=/project \
  --mount type=bind,source=$HOME/.bashrc,destination=/.bashrc \
  --user=1000 \
  -w /project \
  devkitpro/devkitarm $*
