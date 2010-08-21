#! /bin/sh
$EXTRACTRC *.rc >> rc.cpp
$XGETTEXT *.cc *.cpp -o $podir/kcharselect.pot 
