#!/usr/bin/env python
#
# Simple utility to transcode (recompress) movies recorded with the
# in-game movie recorder. This requires mencoder to be installed
# correctly with libavcodec.
#
from BZFlag.UI import MovieRecorder
from BZFlag import optik
import sys, re

parser = optik.OptionParser(usage = "usage: %prog [options] snapshot.bzm")
parser.add_option("-o", "--output", metavar="FILE", dest="file",
                  help="Sets the output AVI file")
parser.add_option("-r", "--resolution", metavar="WIDTHxHEIGHT", dest="resolution",
                  help="Sets the output resolution. Default is 512x384.", default="512x384")
parser.add_option("-b", "--bitrate", metavar="KBPS", dest="bitrate",
                  help="Sets the mpeg4 encoder bit rate. Default is 900kbps.", default=900)
(options, args) = parser.parse_args()

try:
    inFile = args[0]
except IndexError:
    parser.error("A recording filename must be specified on the command line.")

if not options.file:
    options.file = re.sub("\..*", "", inFile) + ".avi"

(width, height) = options.resolution.split("x")

t = MovieRecorder.Transcoder(inFile)
t.encode("-o %s -vop scale=%s:%s -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=%s" %
         (options.file, width, height, options.bitrate))
