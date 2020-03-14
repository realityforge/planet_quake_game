#!/bin/sh

PORT="27984"


cd ../../../
quake3 +set dedicated 2 +set fs_game efreeze +set net_port $PORT +exec efreeze/cfgs/efreeze-pure-tdm.cfg