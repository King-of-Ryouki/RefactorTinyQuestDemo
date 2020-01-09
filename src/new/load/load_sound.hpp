#ifndef SOUND_HPP
#define SOUND_HPP

#include "..\_pch.hpp"

// sound identifiers
enum EnumSound {

    SOUND_AMBIENT_1 = 0,
    SOUND_AMBIENT_2,

    SOUND_SLASH,
    SOUND_SUPERSLASH,

    SOUND_ARROW,

    SOUND_FIREBALL,
    SOUND_SUPERFIREBALL,
    SOUND_EXPLOSION,

    SOUND_HIT,

    SOUND_TOKEN,
    SOUND_CHARGED,

    SOUND_VICTORY,
    SOUND_GAMEOVER,
    SOUND_FAIL,
    SOUND_DEATH,

    SOUND_ALL_SOUND_ID_SUM
};

class Sound {
public:
    Sound();
    ~Sound();

    void Update();

    void PlaySound(int sound_id);

    void StopAllSound();

    bool Load();

private:
    FMOD::System *System; // 處理FMOD引擎 // handle to FMOD engine

    FMOD::Sound *SoundId[ SOUND_ALL_SOUND_ID_SUM ];

    FMOD::Channel *Channel1;
    FMOD::Channel *Channel2;
};

#endif // SOUND_HPP