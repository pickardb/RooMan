// Copyright (C) 1991-2015 Altera Corporation. All rights reserved.
// Your use of Altera Corporation's design tools, logic functions 
// and other software and tools, and its AMPP partner logic 
// functions, and any output files from any of the foregoing 
// (including device programming or simulation files), and any 
// associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License 
// Subscription Agreement, the Altera Quartus II License Agreement,
// the Altera MegaCore Function License Agreement, or other 
// applicable license agreement, including, without limitation, 
// that your use is for the sole purpose of programming logic 
// devices manufactured by Altera and sold by Altera or its 
// authorized distributors.  Please refer to the applicable 
// agreement for further details.

// PROGRAM		"Quartus II 64-Bit"
// VERSION		"Version 15.0.0 Build 145 04/22/2015 SJ Web Edition"
// CREATED		"Thu Jan 25 13:59:31 2018"

module OnChipM68xxIO(
	Reset_L,
	Clock_50Mhz,
	RS232_RxData,
	WE_L,
	GPS_RxData,
	BlueTooth_RxData,
	IOSelect_H,
	ByteSelect_L,
	TouchScreen_RxData,
	Address,
	DataIn,
	RS232_TxData,
	ACIA_IRQ,
	GPS_TxData,
	BlueTooth_TxData,
	TouchScreen_TxData,
	DataOut
);


input wire	Reset_L;
input wire	Clock_50Mhz;
input wire	RS232_RxData;
input wire	WE_L;
input wire	GPS_RxData;
input wire	BlueTooth_RxData;
input wire	IOSelect_H;
input wire	ByteSelect_L;
input wire	TouchScreen_RxData;
input wire	[15:0] Address;
input wire	[7:0] DataIn;
output wire	RS232_TxData;
output wire	ACIA_IRQ;
output wire	GPS_TxData;
output wire	BlueTooth_TxData;
output wire	TouchScreen_TxData;
output wire	[7:0] DataOut;

wire	SYNTHESIZED_WIRE_0;
wire	SYNTHESIZED_WIRE_32;
wire	SYNTHESIZED_WIRE_2;
wire	SYNTHESIZED_WIRE_33;
wire	SYNTHESIZED_WIRE_34;
wire	SYNTHESIZED_WIRE_7;
wire	SYNTHESIZED_WIRE_8;
wire	SYNTHESIZED_WIRE_10;
wire	SYNTHESIZED_WIRE_35;
wire	SYNTHESIZED_WIRE_36;
wire	SYNTHESIZED_WIRE_15;
wire	SYNTHESIZED_WIRE_17;
wire	SYNTHESIZED_WIRE_37;
wire	SYNTHESIZED_WIRE_38;
wire	SYNTHESIZED_WIRE_22;
wire	SYNTHESIZED_WIRE_23;
wire	SYNTHESIZED_WIRE_24;
wire	SYNTHESIZED_WIRE_25;
wire	SYNTHESIZED_WIRE_27;
wire	SYNTHESIZED_WIRE_39;
wire	SYNTHESIZED_WIRE_40;

assign	SYNTHESIZED_WIRE_34 = 0;
assign	SYNTHESIZED_WIRE_36 = 0;
assign	SYNTHESIZED_WIRE_38 = 0;
assign	SYNTHESIZED_WIRE_40 = 0;




M68xxIODecoder	b2v_inst(
	.IOSelect_H(IOSelect_H),
	.ByteSelect_L(ByteSelect_L),
	.WE_L(WE_L),
	.Address(Address),
	.RS232_Port_Enable(SYNTHESIZED_WIRE_10),
	.RS232_Baud_Enable(SYNTHESIZED_WIRE_0),
	.GPS_Port_Enable(SYNTHESIZED_WIRE_17),
	.GPS_Baud_Enable(SYNTHESIZED_WIRE_15),
	.Bluetooth_Port_Enable(SYNTHESIZED_WIRE_27),
	.Bluetooth_Baud_Enable(SYNTHESIZED_WIRE_7),
	.TouchScreen_Port_Enable(SYNTHESIZED_WIRE_2),
	.TouchScreen_Baud_Enable(SYNTHESIZED_WIRE_8));


ACIA_BaudRate_Generator	b2v_inst1(
	.Clk_50Mhz(Clock_50Mhz),
	.Enable_H(SYNTHESIZED_WIRE_0),
	.Clk(Clock_50Mhz),
	.Reset_L(Reset_L),
	.DataIn(DataIn[2:0]),
	.ACIA_Clock(SYNTHESIZED_WIRE_35));


ACIA_6850	b2v_inst10(
	.Clk(Clock_50Mhz),
	.Reset_H(SYNTHESIZED_WIRE_32),
	.CS_H(SYNTHESIZED_WIRE_2),
	.Write_L(WE_L),
	.RS(Address[1]),
	.RxClock(SYNTHESIZED_WIRE_33),
	.TxClock(SYNTHESIZED_WIRE_33),
	.RxData(TouchScreen_RxData),
	.DCD_L(SYNTHESIZED_WIRE_34),
	.CTS_L(SYNTHESIZED_WIRE_34),
	.DataIn(DataIn),
	.IRQ_L(SYNTHESIZED_WIRE_23),
	.TxData(TouchScreen_TxData),
	
	.DataOut(DataOut));


ACIA_BaudRate_Generator	b2v_inst11(
	.Clk_50Mhz(Clock_50Mhz),
	.Enable_H(SYNTHESIZED_WIRE_7),
	.Clk(Clock_50Mhz),
	.Reset_L(Reset_L),
	.DataIn(DataIn[2:0]),
	.ACIA_Clock(SYNTHESIZED_WIRE_39));


ACIA_BaudRate_Generator	b2v_inst12(
	.Clk_50Mhz(Clock_50Mhz),
	.Enable_H(SYNTHESIZED_WIRE_8),
	.Clk(Clock_50Mhz),
	.Reset_L(Reset_L),
	.DataIn(DataIn[2:0]),
	.ACIA_Clock(SYNTHESIZED_WIRE_33));





ACIA_6850	b2v_inst2(
	.Clk(Clock_50Mhz),
	.Reset_H(SYNTHESIZED_WIRE_32),
	.CS_H(SYNTHESIZED_WIRE_10),
	.Write_L(WE_L),
	.RS(Address[1]),
	.RxClock(SYNTHESIZED_WIRE_35),
	.TxClock(SYNTHESIZED_WIRE_35),
	.RxData(RS232_RxData),
	.DCD_L(SYNTHESIZED_WIRE_36),
	.CTS_L(SYNTHESIZED_WIRE_36),
	.DataIn(DataIn),
	.IRQ_L(SYNTHESIZED_WIRE_22),
	.TxData(RS232_TxData),
	
	.DataOut(DataOut));

assign	SYNTHESIZED_WIRE_32 =  ~Reset_L;



ACIA_BaudRate_Generator	b2v_inst5(
	.Clk_50Mhz(Clock_50Mhz),
	.Enable_H(SYNTHESIZED_WIRE_15),
	.Clk(Clock_50Mhz),
	.Reset_L(Reset_L),
	.DataIn(DataIn[2:0]),
	.ACIA_Clock(SYNTHESIZED_WIRE_37));


ACIA_6850	b2v_inst7(
	.Clk(Clock_50Mhz),
	.Reset_H(SYNTHESIZED_WIRE_32),
	.CS_H(SYNTHESIZED_WIRE_17),
	.Write_L(WE_L),
	.RS(Address[1]),
	.RxClock(SYNTHESIZED_WIRE_37),
	.TxClock(SYNTHESIZED_WIRE_37),
	.RxData(GPS_RxData),
	.DCD_L(SYNTHESIZED_WIRE_38),
	.CTS_L(SYNTHESIZED_WIRE_38),
	.DataIn(DataIn),
	.IRQ_L(SYNTHESIZED_WIRE_24),
	.TxData(GPS_TxData),
	
	.DataOut(DataOut));

assign	ACIA_IRQ = ~(SYNTHESIZED_WIRE_22 & SYNTHESIZED_WIRE_23 & SYNTHESIZED_WIRE_24 & SYNTHESIZED_WIRE_25);


ACIA_6850	b2v_inst9(
	.Clk(Clock_50Mhz),
	.Reset_H(SYNTHESIZED_WIRE_32),
	.CS_H(SYNTHESIZED_WIRE_27),
	.Write_L(WE_L),
	.RS(Address[1]),
	.RxClock(SYNTHESIZED_WIRE_39),
	.TxClock(SYNTHESIZED_WIRE_39),
	.RxData(BlueTooth_RxData),
	.DCD_L(SYNTHESIZED_WIRE_40),
	.CTS_L(SYNTHESIZED_WIRE_40),
	.DataIn(DataIn),
	.IRQ_L(SYNTHESIZED_WIRE_25),
	.TxData(BlueTooth_TxData),
	
	.DataOut(DataOut));


endmodule
