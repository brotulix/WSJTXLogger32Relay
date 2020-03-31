# WSJTXLogger32Relay

## What?
WSJTXLogger32Relay is (quite the mouthful) a dead-simple application that listens on 0.0.0.0, UDP port 2240, and relays it on to 127.0.0.1, UDP port 2237.

## Why?
Because Logger32 seems to only listen on 127.0.0.1:2237, which is impractical when WSJT-X (or others) are running on another computer than Logger32.

## How?
Make sure to link against `ws2_32.lib`.

## Ok...
Feel free to use the source code, it's just mashed together from tutorials and howto's anyway.

// Jørn Holm Aske, March 2020