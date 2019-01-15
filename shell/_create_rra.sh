#!/bin/bash


for i in $(cat ids)
do
test -f ${i}.rrd || rrdtool create ${i}.rrd \
-s 300 \
DS:value:GAUGE:600:-100:1000 \
RRA:AVERAGE:0.5:1:288 \
RRA:AVERAGE:0.5:3:672 \
RRA:AVERAGE:0.5:12:744 \
RRA:AVERAGE:0.5:72:1460


done

