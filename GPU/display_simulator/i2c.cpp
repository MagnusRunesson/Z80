//
//  i2c.cpp
//  sh1106_simulator
//
//  Created by Magnus Runesson on 2018-08-23.
//  Copyright © 2018 Magnus Runesson. All rights reserved.
//

#include <stdio.h>
#include "i2c.h"
#include "../display_test_i2c/sh1106_shared.h"

#define INVALID_ADDRESS (-1)

class CI2C
{
public:
	void init()
	{
		m_currentAddress = INVALID_ADDRESS;
	}
	
	uint8 start( uint8 _address )
	{
		if( m_currentAddress != INVALID_ADDRESS )
		{
			printf("ERROR: Calling i2c_start before having called i2c_stop before\n");
		}
		m_currentAddress = _address;
		m_state = CI2C::State::WaitingForControl;
		
		return 0;
	}
	
	void stop()
	{
		m_currentAddress = INVALID_ADDRESS;
	}

	uint8 send( uint8 _data )
	{
		if( m_state == State::WaitingForControl )
		{
			ParseControl( _data );
		}
		else if( m_state == State::FetchingCommands )
		{
			ParseCommand( _data );
		} else if( m_state == State::FetchingCommandParameter )
		{
			ParseDoubleByteCommandParameter( _data );
		} else if( m_state == State::FetchingData )
		{
			WriteData( _data );
		}
		
		return 0;
	}
	
private:
	class State
	{
	public:
		enum e
		{
			WaitingForControl,
			FetchingCommands,
			FetchingCommandParameter,
			FetchingData,
			ReadModifyWrite,
		};
	};

	class Commands
	{
	public:
		enum e
		{
			ColumnAddressLow,		// 1.
			ColumnAddressHigh,		// 2.
			SetPumpVoltage,			// 3.
			DisplayStartLine,		// 4.
			ContrastControl,		// 5.
			SetSegmentRemap,		// 6.
			DisplayOnOff,			// 7.
			NormalReverse,			// 8.
			MultiplexRatio,			// 9.
			DCDCControlMode,		// 10.
			OLEDOnOff,				// 11.
			PageAddress,			// 12.
			COMScanDirection,		// 13.
			DisplayOffset,			// 14.
			ClockFrequency,			// 15.
			ChargePeriods,			// 16.
			ComConfiguration,		// 17.
			VCOMSelect,				// 18.
			ReadModifyWriteStart,	// 19.
			ReadModifyWriteEnd,		// 20.
			NOP,					// 21.
		};
	};
	
	int m_currentAddress;

	State::e	m_state;
	Commands::e m_command;
	
	int m_inputA;
	int m_inputB;		// For multibyte commands

	//
	int m_pageAddress;
	
	int m_pageAddressOffset;
	
	void ParseControl( uint8 _data )
	{
		if( _data == CONTROL_SENDCOMMAND )
		{
			m_state = State::FetchingCommands;
		}
		else if( _data == CONTROL_SENDDATA ) // Send data
		{
			m_state = State::FetchingData;
		}
	}
	
	void ParseCommand( uint8 _data )
	{
		if((_data & 0xf0 ) == 0x00 )
		{
			// 1. Set column address high bits
			m_command = Commands::ColumnAddressLow;
			printf("Setting column address low: %i\n", _data & 0x0f);
		}
		if((_data & 0xf0 ) == 0x10 )
		{
			// 2. Set column address high bits
			m_command = Commands::ColumnAddressHigh;
			printf("Setting column address high: %i\n", _data & 0x0f);
		}
		if((_data & 0xfc) == 0x30 )
		{
			// 3. Pump voltage
			m_command = Commands::SetPumpVoltage;
			printf("Setting pump voltage: %i\n", _data & 0x02 );
		}
		if((_data & 0xC0) == 0x40 )
		{
			// 4. Set display start line
			m_command = Commands::DisplayStartLine;
			printf("Setting display start line: %i\n", _data & 0x3f );
		}
		if( _data == 0x81 )
		{
			// 5. Contrast control
			m_command = Commands::ContrastControl;
			m_state = State::FetchingCommandParameter;
		}
		if((_data & 0xFE) == 0xA0 )
		{
			// 6. SetSegmentRemap
			m_command = Commands::SetSegmentRemap;
			printf("Set segment remap: %i\n", _data & 0x01 );
		}
		if((_data & 0xFE) == 0xA4 )
		{
			// 7. Set display on/off
			m_command = Commands::DisplayOnOff;
			printf("Set display on/off: %i\n", _data & 0x01 );
		}
		if((_data & 0xFE) == 0xA6 )
		{
			// 8. Set display normal / reverse
			m_command = Commands::NormalReverse;
			printf("Setting display normal/reverse to: %i\n", _data & 0x01 );
		}
		if( _data == 0xA8 )
		{
			// 9. Multiplex
			m_command = Commands::MultiplexRatio;
			m_state = State::FetchingCommandParameter;
		}
		if( _data == 0xAD )
		{
			// 10. DC-DC control mode
			m_command = Commands::DCDCControlMode;
			m_state = State::FetchingCommandParameter;
		}
		if((_data & 0xFE) == 0xAE )
		{
			// 11. OLED on / off
			m_command = Commands::OLEDOnOff;
			printf("Set OLED on/off: %i\n", _data & 0x01);
		}
		if((_data & 0xf0) == 0xb0 )
		{
			// 12. Page address
			m_command = Commands::PageAddress;
			m_pageAddress = _data & 0x0f;
			m_pageAddressOffset = 0;
			printf("Page address=%i\n", m_pageAddress );
		}
		if((_data & 0xF0) == 0xC0 )
		{
			// 13. Common output Scan Direction
			m_command = Commands::COMScanDirection;
			printf("Common scan direction: %i\n", _data & 0x08);
		}
		if( _data == 0xD3 )
		{
			// 14. Display offset
			m_command = Commands::DisplayOffset;
			m_state = State::FetchingCommandParameter;
		}
		if( _data == 0xD5 )
		{
			// 15. Clock frequency
			m_command = Commands::ClockFrequency;
			m_state = State::FetchingCommandParameter;
		}
		if( _data == 0xD9 )
		{
			// 16. Charge periods
			m_command = Commands::ChargePeriods;
			m_state = State::FetchingCommandParameter;
		}
		if( _data == 0xDA )
		{
			// 17. Com configuration (common pads hardware configuration)
			m_command = Commands::ComConfiguration;
			m_state = State::FetchingCommandParameter;
		}
		if( _data == 0xDB )
		{
			// 18. VCSOM select
			m_command = Commands::VCOMSelect;
			m_state = State::FetchingCommandParameter;
		}
		if( _data == 0xE0 )
		{
			m_command = Commands::ReadModifyWriteStart;
			m_state = State::ReadModifyWrite;
		}
		if( _data == 0xEE )
		{
			m_command = Commands::ReadModifyWriteStart;
			m_state = State::ReadModifyWrite;
		}
		if( _data == 0xE3 )
		{
			m_command = Commands::NOP;
		}
	}

	void ParseDoubleByteCommandParameter( uint8 _data )
	{
		m_state = State::FetchingCommands;
	}
	
	void WriteData( uint8 _data )
	{
		
	}
};

CI2C i2c;

void i2c_init()
{
	i2c.init();
}

uint8 i2c_start( uint8 _address )
{
	return i2c.start( _address );
}

void i2c_stop(void)
{
	i2c.stop();
}

uint8 i2c_send( uint8 _data )
{
	return i2c.send( _data );
}
