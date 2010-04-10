#!/bin/sh

mkThumb() {
  i=$1
  NAME=thumb_250x250_$i
  if [ -r $NAME ]; then
    echo "$NAME exists, skipping."
  else
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
    echo "$i -> $NAME"
    convert -resize $GEOMETRY $i $NAME
  fi
}

if [ $# = 0 ]; then
  while read a; do
    mkThumb "$a"
  done
else
  for i in "$@"; do
    mkThumb "$i"
  done
fi
