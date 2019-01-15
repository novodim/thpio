#!/bin/bash


function get_value {
echo $(cat /tmp/status | grep -w "$1:" | awk '{print $2}')
}

sleep 2

dev_name=$(find  /sys/module/thpo/drivers/usb:thpo/ -type l | grep -v "usb:thpo/module")
cat ${dev_name}/status > /tmp/status

dp=$(get_value "mpx-0")
hgp=$(echo "scale=0;($dp*0.81387+79.13)/1"|bc)
echo "pressure: $hgp" >> /tmp/status

web_path="/var/www/thpo"
cd ${web_path}

for i in $(cat ids)
do
        value=$(get_value ${i})
        rrdtool update ${i}.rrd N:${value}
done

> /tmp/rrd_times
echo "1d 86400" >> /tmp/rrd_times
echo "2d 172800" >> /tmp/rrd_times
echo "3d 259200" >> /tmp/rrd_times
echo "1w 604800" >> /tmp/rrd_times
echo "1m 2592000" >> /tmp/rrd_times
echo "3m 8035200" >> /tmp/rrd_times
echo "1year 31536000" >> /tmp/rrd_times

cat /tmp/rrd_times | while read id dt
do

for i in $(cat ids)
do
rrdtool graph - \
--right-axis 1:0 \
--imgformat=PNG \
--start=-${id} \
--end=now \
--title="${i} values" \
--base=1000 \
--height=300 \
--width=700 \
--vertical-label="" \
--font TITLE:10: \
--font AXIS:8: \
--font LEGEND:10: \
--font UNIT:8: \
--slope-mode \
--tabwidth 40 \
DEF:v="${web_path}/${i}.rrd":value:AVERAGE \
LINE1:v#0000FF:""  \
GPRINT:v:LAST:"Last\: %.1lf\n" GPRINT:v:MAX:"Max\: %.1lf\n" GPRINT:v:MIN:"Min\: %.1lf\n" \
> ${web_path}/${i}_${id}.png_tmp

mv ${i}_${id}.png_tmp ${i}_${id}.png

done

done

