#/bin/sh

echo "Generating xes from png..."

perl img2xes.pl -x 320 -y 26214 -d 3+2+3 -pixelwidth 8 -memwidth 16 -address 0x000000 -i starcell_full.png -o starcell.xes

echo "Converting to DOS line endings..."

unix2dos --unix2dos --force starcell.xes >&/dev/null

echo "Done."
