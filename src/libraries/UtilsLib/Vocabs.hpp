// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_VOCABS_HPP__
#define __RD_VOCABS_HPP__

#include <yarp/os/Vocab.h>

constexpr auto VOCAB_RD_FAIL = yarp::os::createVocab32('f','a','i','l');
constexpr auto VOCAB_RD_GAME_OVER = yarp::os::createVocab32('o','v','e','r');
constexpr auto VOCAB_RD_HELP = yarp::os::createVocab32('h','e','l','p');
constexpr auto VOCAB_RD_HIT = yarp::os::createVocab32('h','i','t');
constexpr auto VOCAB_RD_LOGIN = yarp::os::createVocab32('l','o','g');
constexpr auto VOCAB_RD_LOGOUT = yarp::os::createVocab32('l','o','g','o');
constexpr auto VOCAB_RD_OK = yarp::os::createVocab32('o','k');
constexpr auto VOCAB_RD_PLAYERS = yarp::os::createVocab32('p','l','y','s');
constexpr auto VOCAB_RD_RESPAWN = yarp::os::createVocab32('r','p','w','n');
constexpr auto VOCAB_RD_KEEPALIVE = yarp::os::createVocab32('k','e','e','p');

#endif  // __RD_VOCABS_HPP__
