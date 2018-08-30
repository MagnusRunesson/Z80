//
//  Serial.h
//  sh1106_simulator
//
//  Created by Magnus Runesson on 2018-08-23.
//  Copyright © 2018 Magnus Runesson. All rights reserved.
//

#ifndef Serial_h
#define Serial_h

class CSerial
{
public:
	void begin( int _speed );
};

extern CSerial Serial;

#endif /* Serial_h */
