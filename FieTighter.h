#ifndef FIETIGHTER_H_INCLUDED
#define FIETIGHTER_H_INCLUDED

#include <Ogre.h>

// Configuration File
#define VIDEO_SECTION    video
#define CONFIG_OPTS_DIR  .FieTighter
#define CONFIG_FILE_NAME config.ini

typedef Ogre::NameValuePairList VideoOptions;

class VideoInitializationException : Ogre::Exception
{
    public:
        VideoInitializationException (const Ogre::String &err) : Ogre::Exception(ERR_INTERNAL_ERROR, err, "Video Initialization") { };
};

// Main Root Class
class FieTighter
{
    public:
        FieTighter();

    protected:
        Ogre::Root *ogre;

        bool getOptions(VideoOptions opts);
};


#endif // FIETIGHTER_H_INCLUDED
