#!/usr/bin/env bash
cd "$(dirname "$0")" || exit 1
BINARY="./bin/DroneWarfare"

if [[ ! -f "$BINARY" ]]; then
  echo "Error: binary '$BINARY' not found."
  exit 1
fi

chmod +x "$BINARY"
echo "Starting $BINARY..."
"$BINARY" "$@"
