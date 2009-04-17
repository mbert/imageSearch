#!/bin/sh

if [ $# = 0 ]; then
  echo "usage $0 <files>" 1>&2
  exit 1
fi

for i in "$@"; do
  SIZE="`identify $i | sed "s/.* \([0-9]\+\)x\([0-9]\+\)+[0-9]\++[0-9]\+ .*/\1 \2/"`"
  if [ -z "$SIZE" ]; then
    echo "Error, cannot make thumbnail from $i"
    exit 1
  fi
  ROWS=`echo $SIZE | cut -d ' ' -f 1`
  COLS=`echo $SIZE | cut -d ' ' -f 2`
  #echo "ROWS: $ROWS"
  #echo "COLS: $COLS"
  GEOMETRY="250x150"
  NAME=thumb_250x250_$i
  echo "$i -> $NAME"
  convert -resize $GEOMETRY $i $NAME
done
