#!/bin/bash
for i in {1..8}
do
   curl -X PUT -H 'Content-Type: application/json' -d '{"color":[255,0,255]}' http://192.168.31.122/lightstrip/color
   sleep 0.05
done

