/* bzflag
 * Copyright (c) 1993 - 2002 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named LICENSE that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef BZF_OGG_AUDIO_FILE_H
#define BZF_OGG_AUDIO_FILE_H

#include "AudioFile.h"
#include <vorbis/vorbisfile.h>

class OggAudioFile : public AudioFile {
public:
	OggAudioFile(std::istream*);
	virtual ~OggAudioFile();

	static std::string	getExtension();

	// AudioFile overrides
	virtual bool		read(void* buffer, int numFrames);

protected:
	OggVorbis_File		file;
	vorbis_info*		info;
	bool				open;
	int					stream;
};

size_t	OAFRead(void* ptr, size_t size, size_t nmemb, void* datasource);
int		OAFSeek(void* datasource, ogg_int64_t offset, int whence);
int		OAFClose(void* datasource);
long	OAFTell(void* datasource);

#endif
// ex: shiftwidth=4 tabstop=4
