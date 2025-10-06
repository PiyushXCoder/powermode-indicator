#!/bin/env fish

cd (git rev-parse --show-toplevel 2>/dev/null)
if test -f .env.fish
  echo "Loading environment variables from .env.fish"
  source .env.fish
end

if not test -d build
  echo "Creating build directory..."
  mkdir build
end

cmake -S . -B build/ --preset=default && \
  cmake --build build/ && \
  ./build/powermode-indicator
