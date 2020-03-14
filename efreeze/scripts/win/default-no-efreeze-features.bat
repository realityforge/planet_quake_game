@echo off

set port="27982"


cd ../../../
quake3 +set dedicated 2 +set fs_game efreeze +set net_port %port% +exec efreeze/cfgs/default-no-efreeze-features.cfg

