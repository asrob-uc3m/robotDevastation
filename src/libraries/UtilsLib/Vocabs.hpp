// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_VOCABS_HPP__
#define __RD_VOCABS_HPP__

#define VOCAB(a,b,c,d) ((((int)(d))<<24)+(((int)(c))<<16)+(((int)(b))<<8)+((int)(a)))

#define VOCAB_RD_FAIL VOCAB('f','a','i','l')
#define VOCAB_RD_GAME_OVER VOCAB('o','v','e','r')
#define VOCAB_RD_HELP VOCAB('h','e','l','p')
#define VOCAB_RD_HIT VOCAB('h','i','t',0)
#define VOCAB_RD_LOGIN VOCAB('l','o','g',0)
#define VOCAB_RD_LOGOUT VOCAB('l','o','g','o')
#define VOCAB_RD_OK VOCAB('o','k',0,0)
#define VOCAB_RD_PLAYERS VOCAB('p','l','y','s')
#define VOCAB_RD_RESPAWN VOCAB('r','p','w','n')
#define VOCAB_RD_KEEPALIVE VOCAB('k','e','e','p')

#endif  // __RD_VOCABS_HPP__
