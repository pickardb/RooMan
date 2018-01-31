-- Copyright (C) 1991-2015 Altera Corporation. All rights reserved.
-- Your use of Altera Corporation's design tools, logic functions 
-- and other software and tools, and its AMPP partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Altera Program License 
-- Subscription Agreement, the Altera Quartus II License Agreement,
-- the Altera MegaCore Function License Agreement, or other 
-- applicable license agreement, including, without limitation, 
-- that your use is for the sole purpose of programming logic 
-- devices manufactured by Altera and sold by Altera or its 
-- authorized distributors.  Please refer to the applicable 
-- agreement for further details.

-- PROGRAM		"Quartus II 64-Bit"
-- VERSION		"Version 15.0.0 Build 145 04/22/2015 SJ Web Edition"
-- CREATED		"Thu Jan 25 13:55:57 2018"

LIBRARY ieee;
USE ieee.std_logic_1164.all; 

LIBRARY work;

ENTITY OnChipM68xxIO IS 
	PORT
	(
		Reset_L :  IN  STD_LOGIC;
		Clock_50Mhz :  IN  STD_LOGIC;
		RS232_RxData :  IN  STD_LOGIC;
		WE_L :  IN  STD_LOGIC;
		GPS_RxData :  IN  STD_LOGIC;
		BlueTooth_RxData :  IN  STD_LOGIC;
		IOSelect_H :  IN  STD_LOGIC;
		ByteSelect_L :  IN  STD_LOGIC;
		TouchScreen_RxData :  IN  STD_LOGIC;
		Address :  IN  STD_LOGIC_VECTOR(15 DOWNTO 0);
		DataIn :  IN  STD_LOGIC_VECTOR(7 DOWNTO 0);
		RS232_TxData :  OUT  STD_LOGIC;
		ACIA_IRQ :  OUT  STD_LOGIC;
		GPS_TxData :  OUT  STD_LOGIC;
		BlueTooth_TxData :  OUT  STD_LOGIC;
		TouchScreen_TxData :  OUT  STD_LOGIC;
		DataOut :  OUT  STD_LOGIC_VECTOR(7 DOWNTO 0)
	);
END OnChipM68xxIO;

ARCHITECTURE bdf_type OF OnChipM68xxIO IS 

COMPONENT m68xxiodecoder
	PORT(IOSelect_H : IN STD_LOGIC;
		 ByteSelect_L : IN STD_LOGIC;
		 WE_L : IN STD_LOGIC;
		 Address : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
		 RS232_Port_Enable : OUT STD_LOGIC;
		 RS232_Baud_Enable : OUT STD_LOGIC;
		 GPS_Port_Enable : OUT STD_LOGIC;
		 GPS_Baud_Enable : OUT STD_LOGIC;
		 Bluetooth_Port_Enable : OUT STD_LOGIC;
		 Bluetooth_Baud_Enable : OUT STD_LOGIC;
		 TouchScreen_Port_Enable : OUT STD_LOGIC;
		 TouchScreen_Baud_Enable : OUT STD_LOGIC
	);
END COMPONENT;

COMPONENT acia_baudrate_generator
	PORT(Clk_50Mhz : IN STD_LOGIC;
		 Enable_H : IN STD_LOGIC;
		 Clk : IN STD_LOGIC;
		 Reset_L : IN STD_LOGIC;
		 DataIn : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
		 ACIA_Clock : OUT STD_LOGIC
	);
END COMPONENT;

COMPONENT acia_6850
	PORT(Clk : IN STD_LOGIC;
		 Reset_H : IN STD_LOGIC;
		 CS_H : IN STD_LOGIC;
		 Write_L : IN STD_LOGIC;
		 RS : IN STD_LOGIC;
		 RxClock : IN STD_LOGIC;
		 TxClock : IN STD_LOGIC;
		 RxData : IN STD_LOGIC;
		 DCD_L : IN STD_LOGIC;
		 CTS_L : IN STD_LOGIC;
		 DataIn : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
		 IRQ_L : OUT STD_LOGIC;
		 TxData : OUT STD_LOGIC;
		 RTS_L : OUT STD_LOGIC;
		 DataOut : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
	);
END COMPONENT;

SIGNAL	SYNTHESIZED_WIRE_0 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_32 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_2 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_33 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_34 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_7 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_8 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_10 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_35 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_36 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_15 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_17 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_37 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_38 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_22 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_23 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_24 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_25 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_27 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_39 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_40 :  STD_LOGIC;


BEGIN 
SYNTHESIZED_WIRE_34 <= '0';
SYNTHESIZED_WIRE_36 <= '0';
SYNTHESIZED_WIRE_38 <= '0';
SYNTHESIZED_WIRE_40 <= '0';



b2v_inst : m68xxiodecoder
PORT MAP(IOSelect_H => IOSelect_H,
		 ByteSelect_L => ByteSelect_L,
		 WE_L => WE_L,
		 Address => Address,
		 RS232_Port_Enable => SYNTHESIZED_WIRE_10,
		 RS232_Baud_Enable => SYNTHESIZED_WIRE_0,
		 GPS_Port_Enable => SYNTHESIZED_WIRE_17,
		 GPS_Baud_Enable => SYNTHESIZED_WIRE_15,
		 Bluetooth_Port_Enable => SYNTHESIZED_WIRE_27,
		 Bluetooth_Baud_Enable => SYNTHESIZED_WIRE_7,
		 TouchScreen_Port_Enable => SYNTHESIZED_WIRE_2,
		 TouchScreen_Baud_Enable => SYNTHESIZED_WIRE_8);


b2v_inst1 : acia_baudrate_generator
PORT MAP(Clk_50Mhz => Clock_50Mhz,
		 Enable_H => SYNTHESIZED_WIRE_0,
		 Clk => Clock_50Mhz,
		 Reset_L => Reset_L,
		 DataIn => DataIn(2 DOWNTO 0),
		 ACIA_Clock => SYNTHESIZED_WIRE_35);


b2v_inst10 : acia_6850
PORT MAP(Clk => Clock_50Mhz,
		 Reset_H => SYNTHESIZED_WIRE_32,
		 CS_H => SYNTHESIZED_WIRE_2,
		 Write_L => WE_L,
		 RS => Address(1),
		 RxClock => SYNTHESIZED_WIRE_33,
		 TxClock => SYNTHESIZED_WIRE_33,
		 RxData => TouchScreen_RxData,
		 DCD_L => SYNTHESIZED_WIRE_34,
		 CTS_L => SYNTHESIZED_WIRE_34,
		 DataIn => DataIn,
		 IRQ_L => SYNTHESIZED_WIRE_23,
		 TxData => TouchScreen_TxData,
		 DataOut => DataOut);


b2v_inst11 : acia_baudrate_generator
PORT MAP(Clk_50Mhz => Clock_50Mhz,
		 Enable_H => SYNTHESIZED_WIRE_7,
		 Clk => Clock_50Mhz,
		 Reset_L => Reset_L,
		 DataIn => DataIn(2 DOWNTO 0),
		 ACIA_Clock => SYNTHESIZED_WIRE_39);


b2v_inst12 : acia_baudrate_generator
PORT MAP(Clk_50Mhz => Clock_50Mhz,
		 Enable_H => SYNTHESIZED_WIRE_8,
		 Clk => Clock_50Mhz,
		 Reset_L => Reset_L,
		 DataIn => DataIn(2 DOWNTO 0),
		 ACIA_Clock => SYNTHESIZED_WIRE_33);





b2v_inst2 : acia_6850
PORT MAP(Clk => Clock_50Mhz,
		 Reset_H => SYNTHESIZED_WIRE_32,
		 CS_H => SYNTHESIZED_WIRE_10,
		 Write_L => WE_L,
		 RS => Address(1),
		 RxClock => SYNTHESIZED_WIRE_35,
		 TxClock => SYNTHESIZED_WIRE_35,
		 RxData => RS232_RxData,
		 DCD_L => SYNTHESIZED_WIRE_36,
		 CTS_L => SYNTHESIZED_WIRE_36,
		 DataIn => DataIn,
		 IRQ_L => SYNTHESIZED_WIRE_22,
		 TxData => RS232_TxData,
		 DataOut => DataOut);


SYNTHESIZED_WIRE_32 <= NOT(Reset_L);




b2v_inst5 : acia_baudrate_generator
PORT MAP(Clk_50Mhz => Clock_50Mhz,
		 Enable_H => SYNTHESIZED_WIRE_15,
		 Clk => Clock_50Mhz,
		 Reset_L => Reset_L,
		 DataIn => DataIn(2 DOWNTO 0),
		 ACIA_Clock => SYNTHESIZED_WIRE_37);


b2v_inst7 : acia_6850
PORT MAP(Clk => Clock_50Mhz,
		 Reset_H => SYNTHESIZED_WIRE_32,
		 CS_H => SYNTHESIZED_WIRE_17,
		 Write_L => WE_L,
		 RS => Address(1),
		 RxClock => SYNTHESIZED_WIRE_37,
		 TxClock => SYNTHESIZED_WIRE_37,
		 RxData => GPS_RxData,
		 DCD_L => SYNTHESIZED_WIRE_38,
		 CTS_L => SYNTHESIZED_WIRE_38,
		 DataIn => DataIn,
		 IRQ_L => SYNTHESIZED_WIRE_24,
		 TxData => GPS_TxData,
		 DataOut => DataOut);


ACIA_IRQ <= NOT(SYNTHESIZED_WIRE_22 AND SYNTHESIZED_WIRE_23 AND SYNTHESIZED_WIRE_24 AND SYNTHESIZED_WIRE_25);


b2v_inst9 : acia_6850
PORT MAP(Clk => Clock_50Mhz,
		 Reset_H => SYNTHESIZED_WIRE_32,
		 CS_H => SYNTHESIZED_WIRE_27,
		 Write_L => WE_L,
		 RS => Address(1),
		 RxClock => SYNTHESIZED_WIRE_39,
		 TxClock => SYNTHESIZED_WIRE_39,
		 RxData => BlueTooth_RxData,
		 DCD_L => SYNTHESIZED_WIRE_40,
		 CTS_L => SYNTHESIZED_WIRE_40,
		 DataIn => DataIn,
		 IRQ_L => SYNTHESIZED_WIRE_25,
		 TxData => BlueTooth_TxData,
		 DataOut => DataOut);


END bdf_type;