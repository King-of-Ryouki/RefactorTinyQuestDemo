#include "load_sound.hpp"

Sound::Sound() {

    FMOD::System_Create(&System);          // 創建遊戲引擎的實例 // create an instance of the game engine
    System->init(32, FMOD_INIT_NORMAL, 0); // 使用32個通道初始化遊戲引擎 (可以同時發出的聲音量) // initialise the game engine with 32 channels (amount of simultaneous sound that can be)
}

Sound::~Sound() {

    for (int i = 0; i < SOUND_ALL_SOUND_ID_SUM; ++i) {
        SoundId[ i ]->release();
    }

    System->release();
}

void Sound::Update() { System->update(); }

void Sound::PlaySound(int sound_id) {

    switch (sound_id) {

        case SOUND_AMBIENT_1:
            System->playSound(SoundId[ SOUND_AMBIENT_1 ], nullptr, false, &Channel1);
            Channel1->setVolume(0.25);
            break;

        case SOUND_AMBIENT_2:
            System->playSound(SoundId[ SOUND_AMBIENT_2 ], nullptr, false, &Channel2);
            Channel2->setVolume(0.25);
            break;

        default:
            System->playSound(SoundId[ sound_id ], nullptr, false, 0);
    }
}

void Sound::StopAllSound() {
    Channel1->stop();
    Channel2->stop();
}

bool Sound::Load() {

    System->createStream("Sounds/ambient1.mp3", FMOD_LOOP_NORMAL, 0, &SoundId[ static_cast<int>(SOUND_AMBIENT_1) ]);
    System->createStream("Sounds/ambient2.mp3", FMOD_LOOP_NORMAL, 0, &SoundId[ static_cast<int>(SOUND_AMBIENT_2) ]);

    System->createSound("Sounds/gameover.mp3", FMOD_DEFAULT, 0, &SoundId[ static_cast<int>(SOUND_GAMEOVER) ]);
    System->createSound("Sounds/slash.wav", FMOD_DEFAULT, 0, &SoundId[ static_cast<int>(SOUND_SLASH) ]);
    System->createSound("Sounds/superslash.wav", FMOD_DEFAULT, 0, &SoundId[ static_cast<int>(SOUND_SUPERSLASH) ]);
    System->createSound("Sounds/hit.wav", FMOD_DEFAULT, 0, &SoundId[ static_cast<int>(SOUND_HIT) ]);
    System->createSound("Sounds/explosion.wav", FMOD_DEFAULT, 0, &SoundId[ static_cast<int>(SOUND_EXPLOSION) ]);
    System->createSound("Sounds/fireball.wav", FMOD_DEFAULT, 0, &SoundId[ static_cast<int>(SOUND_FIREBALL) ]);
    System->createSound("Sounds/superfireball.wav", FMOD_DEFAULT, 0, &SoundId[ static_cast<int>(SOUND_SUPERFIREBALL) ]);
    System->createSound("Sounds/arrow.wav", FMOD_DEFAULT, 0, &SoundId[ static_cast<int>(SOUND_ARROW) ]);
    System->createSound("Sounds/death.wav", FMOD_DEFAULT, 0, &SoundId[ static_cast<int>(SOUND_DEATH) ]);
    System->createSound("Sounds/token.wav", FMOD_DEFAULT, 0, &SoundId[ static_cast<int>(SOUND_TOKEN) ]);
    System->createSound("Sounds/fail.wav", FMOD_DEFAULT, 0, &SoundId[ static_cast<int>(SOUND_FAIL) ]);
    System->createSound("Sounds/charged.wav", FMOD_DEFAULT, 0, &SoundId[ static_cast<int>(SOUND_CHARGED) ]);
    System->createSound("Sounds/victory.mp3", FMOD_DEFAULT, 0, &SoundId[ static_cast<int>(SOUND_VICTORY) ]);

    return true;
}