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
// CREATED		"Thu Jan 25 14:51:25 2018"

module DE1_SOC_NIOS(
	CLOCK_50,
	GPIO_011,
	GPIO_00,
	GPIO_128,
	GPIO_132,
	KEY,
	SW,
	DRAM_CLK,
	DRAM_CKE,
	DRAM_CS_N,
	DRAM_CAS_N,
	DRAM_RAS_N,
	DRAM_WE_N,
	DRAM_UDQM,
	DRAM_LDQM,
	VGA_HS,
	VGA_VS,
	VGA_BLANK_N,
	VGA_SYNC_N,
	VGA_CLK,
	DRAM_ADDR,
	DRAM_BA,
	DRAM_DQ,
	GPIO_05,
	HEX0,
	HEX1,
	HEX2,
	HEX3,
	HEX4,
	HEX5,
	LEDR,
	VGA_B,
	VGA_G,
	VGA_R
);


input wire	CLOCK_50;
input wire	GPIO_011;
input wire	GPIO_00;
input wire	GPIO_128;
input wire	GPIO_132;
input wire	[3:0] KEY;
input wire	[9:0] SW;
output wire	DRAM_CLK;
output wire	DRAM_CKE;
output wire	DRAM_CS_N;
output wire	DRAM_CAS_N;
output wire	DRAM_RAS_N;
output wire	DRAM_WE_N;
output wire	DRAM_UDQM;
output wire	DRAM_LDQM;
output wire	VGA_HS;
output wire	VGA_VS;
output wire	VGA_BLANK_N;
output wire	VGA_SYNC_N;
output wire	VGA_CLK;
output wire	[12:0] DRAM_ADDR;
output wire	[1:0] DRAM_BA;
inout wire	[15:0] DRAM_DQ;
output wire	GPIO_05;
output wire	[6:0] HEX0;
output wire	[6:0] HEX1;
output wire	[6:0] HEX2;
output wire	[6:0] HEX3;
output wire	[6:0] HEX4;
output wire	[6:0] HEX5;
output wire	[9:0] LEDR;
output wire	[7:0] VGA_B;
output wire	[7:0] VGA_G;
output wire	[7:0] VGA_R;

wire	gpio_10;
wire	gpio_11;
wire	gpio_111;
wire	gpio_113;
wire	gpio_115;
wire	gpio_12;
wire	GPIO_126;
wire	gpio_13;
wire	GPIO_134;
wire	gpio_14;
wire	gpio_15;
wire	gpio_16;
wire	gpio_17;
wire	[11:10] GPIO_ALTERA_SYNTHESIZED0;
wire	[1:0] IO_byte_enable;
wire	[15:0] IO_read_data;
wire	[15:0] IO_write_data;
wire	SYNTHESIZED_WIRE_18;
wire	SYNTHESIZED_WIRE_1;
wire	SYNTHESIZED_WIRE_19;
wire	[15:0] SYNTHESIZED_WIRE_20;
wire	[7:0] SYNTHESIZED_WIRE_4;
wire	[7:0] SYNTHESIZED_WIRE_5;
wire	[7:0] SYNTHESIZED_WIRE_6;
wire	SYNTHESIZED_WIRE_7;
wire	SYNTHESIZED_WIRE_9;
reg	DFF_inst14;
wire	SYNTHESIZED_WIRE_11;
wire	SYNTHESIZED_WIRE_21;
wire	SYNTHESIZED_WIRE_13;
wire	SYNTHESIZED_WIRE_14;

assign	VGA_SYNC_N = 0;
assign	SYNTHESIZED_WIRE_7 = 1;
assign	SYNTHESIZED_WIRE_9 = 1;
wire	[7:0] GDFX_TEMP_SIGNAL_0;


assign	GDFX_TEMP_SIGNAL_0 = {gpio_17,gpio_16,gpio_15,gpio_14,gpio_13,gpio_12,gpio_11,gpio_10};


OnChipM68xxIO	b2v_inst(
	.Clock_50Mhz(CLOCK_50),
	.IOSelect_H(SYNTHESIZED_WIRE_18),
	.ByteSelect_L(SYNTHESIZED_WIRE_1),
	.WE_L(SYNTHESIZED_WIRE_19),
	.Reset_L(KEY[0]),
	.RS232_RxData(GPIO_00),
	.GPS_RxData(GPIO_128),
	.BlueTooth_RxData(GPIO_132),
	.TouchScreen_RxData(GPIO_ALTERA_SYNTHESIZED0[11]),
	.Address(SYNTHESIZED_WIRE_20),
	.DataIn(IO_write_data[7:0]),
	.ACIA_IRQ(SYNTHESIZED_WIRE_11),
	.RS232_TxData(GPIO_05),
	
	
	.TouchScreen_TxData(GPIO_ALTERA_SYNTHESIZED0[10]),
	.DataOut(IO_read_data[7:0]));


HexDisplays	b2v_inst1(
	.Hex0_1(SYNTHESIZED_WIRE_4),
	.Hex2_3(SYNTHESIZED_WIRE_5),
	.Hex4_5(SYNTHESIZED_WIRE_6),
	.HEX0(HEX0),
	.HEX1(HEX1),
	.HEX2(HEX2),
	.HEX3(HEX3),
	.HEX4(HEX4),
	.HEX5(HEX5));


always@(posedge CLOCK_50 or negedge SYNTHESIZED_WIRE_7 or negedge SYNTHESIZED_WIRE_9)
begin
if (!SYNTHESIZED_WIRE_7)
	begin
	DFF_inst14 <= 0;
	end
else
if (!SYNTHESIZED_WIRE_9)
	begin
	DFF_inst14 <= 1;
	end
else
	begin
	DFF_inst14 <= SYNTHESIZED_WIRE_18;
	end
end




assign	SYNTHESIZED_WIRE_21 =  ~SYNTHESIZED_WIRE_18;

assign	SYNTHESIZED_WIRE_14 =  ~IO_byte_enable[0];


NIOS_II_SYSTEM	b2v_inst3(
	.CLOCK_50(CLOCK_50),
	.IO_acknowledge(DFF_inst14),
	.IO_irq(SYNTHESIZED_WIRE_11),
	.DRAM_DQ(DRAM_DQ),
	.IO_read_data(IO_read_data),
	.KEY(KEY[0]),
	.lcd_data(GDFX_TEMP_SIGNAL_0),
	.Push_Buttons(KEY[3:1]),
	.SW(SW),
	.DRAM_CLK(DRAM_CLK),
	.DRAM_CKE(DRAM_CKE),
	.DRAM_CS_N(DRAM_CS_N),
	.DRAM_CAS_N(DRAM_CAS_N),
	.DRAM_RAS_N(DRAM_RAS_N),
	.DRAM_WE_N(DRAM_WE_N),
	.DRAM_UDQM(DRAM_UDQM),
	.DRAM_LDQM(DRAM_LDQM),
	.IO_bus_enable(SYNTHESIZED_WIRE_18),
	.IO_rw(SYNTHESIZED_WIRE_19),
	
	
	
	
	
	.DRAM_ADDR(DRAM_ADDR),
	.DRAM_BA(DRAM_BA),
	
	.Hex0_1(SYNTHESIZED_WIRE_4),
	.Hex2_3(SYNTHESIZED_WIRE_5),
	.Hex4_5(SYNTHESIZED_WIRE_6),
	.IO_address(SYNTHESIZED_WIRE_20),
	.IO_byte_enable(IO_byte_enable),
	.IO_write_data(IO_write_data),
	
	.LEDR(LEDR));

assign	SYNTHESIZED_WIRE_13 =  ~IO_byte_enable[1];

assign	SYNTHESIZED_WIRE_1 =  ~IO_byte_enable[0];


Graphics_and_Video_Controller	b2v_inst4(
	.Reset_L(KEY[0]),
	.Clock_50Mhz(CLOCK_50),
	.IOEnable_L(SYNTHESIZED_WIRE_21),
	.UpperByteSelect_L(SYNTHESIZED_WIRE_13),
	.LowerByteSelect_L(SYNTHESIZED_WIRE_14),
	.WriteEnable_L(SYNTHESIZED_WIRE_19),
	.GraphicsCS_L(SYNTHESIZED_WIRE_21),
	.Address(SYNTHESIZED_WIRE_20),
	.DataIn(IO_write_data),
	.VGA_CLK(VGA_CLK),
	.VGA_HS(VGA_HS),
	.VGA_VS(VGA_VS),
	.VGA_BLANK_N(VGA_BLANK_N),
	.DataOut(IO_read_data),
	.VGA_B(VGA_B),
	.VGA_G(VGA_G),
	.VGA_R(VGA_R));

assign	gpio_10 = GPIO_1[7..0][0];
assign	gpio_11 = GPIO_1[7..0][1];
assign	gpio_12 = GPIO_1[7..0][2];
assign	gpio_13 = GPIO_1[7..0][3];
assign	gpio_14 = GPIO_1[7..0][4];
assign	gpio_15 = GPIO_1[7..0][5];
assign	gpio_16 = GPIO_1[7..0][6];
assign	gpio_17 = GPIO_1[7..0][7];
assign	GPIO_ALTERA_SYNTHESIZED0[11] = GPIO_011;

endmodule
