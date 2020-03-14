@echo off

set port="27986"


cd ../../../
quake3 +set dedicated 2 +set fs_game efreeze +set net_port %port% +exec efreeze/cfgs/efreeze-unpure-tdm.cfg

