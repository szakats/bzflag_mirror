#!/usr/bin/env python
"""
Simple utility to transcode (recompress) movies recorded with the
in-game movie recorder. This requires mencoder to be installed
correctly with libavcodec.

 usage:
  movieconverter.py input.bzm [output.avi [mpeg4rate]]
"""
from BZFlag.UI import MovieRecorder
import sys, re

try:
    inFile = sys.argv[1]
except:
    print __doc__
    sys.exit(0)

try:
    outFile = sys.argv[2]
except IndexError:
    outFile = re.sub("\..*", "", inFile) + ".avi"

try:
    rate = sys.argv[3]
except IndexError:
    rate = 1000

t = MovieRecorder.Transcoder(inFile)
t.encode("-o %s -idx -forceidx -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=%s" % (outFile, rate))
