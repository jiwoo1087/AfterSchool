#pragma once
#ifndef MP3_H_INCLUDED
#define MP3_H_INCLUDED

#include <SFML/Audio.hpp>
#include "mpg123.h"
typedef int ssize_t;           //ssize_t 라는 새로운 자료형을 선언했는데, 이는 long과 같은 자료형이다.

namespace sfe
{
    class Mp3 : public sf::SoundStream
    {
    public:
        Mp3();
        ~Mp3();

        bool openFromFile(const std::string& filename);
        bool openFromMemory(void* data, size_t sizeInBytes);

        unsigned getDuration();

    protected:
        bool onGetData(Chunk& data);
        void onSeek(sf::Time timeOffset);

    private:
        mpg123_handle* myHandle;
        size_t              myBufferSize;
        unsigned char* myBuffer;
        sf::Mutex           myMutex;
        long                mySamplingRate;
    };

} // namespace sfe

#endif // MP3_H_INCLUDED
