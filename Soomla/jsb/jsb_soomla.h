//
// Created by Fedor Shubin on 1/22/14.
//


#ifndef __JSB_AUTO_H_
#define __JSB_AUTO_H_

#ifdef COCOS2D_JAVASCRIPT

#include "jsapi.h"
#include "jsfriendapi.h"
#include "ScriptingCore.h"
#include "JSBinding.h"

void register_jsb_soomla(JSContext *cx, JSObject *obj);

#endif // COCOS2D_JAVASCRIPT

#endif //__JSB_AUTO_H_
