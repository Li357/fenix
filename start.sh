#! /bin/sh

(sleep 10 && sudo ifconfig bridge0 addm tap0) & sudo sh /Applications/Renode.app/Contents/MacOS/macos_run.command renode-config.resc
