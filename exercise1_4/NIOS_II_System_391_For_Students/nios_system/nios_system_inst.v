	nios_system u0 (
		.clk_clk             (<connected-to-clk_clk>),             //          clk.clk
		.hex0_1_export       (<connected-to-hex0_1_export>),       //       hex0_1.export
		.hex2_3_export       (<connected-to-hex2_3_export>),       //       hex2_3.export
		.hex4_5_export       (<connected-to-hex4_5_export>),       //       hex4_5.export
		.io_acknowledge      (<connected-to-io_acknowledge>),      //           io.acknowledge
		.io_irq              (<connected-to-io_irq>),              //             .irq
		.io_address          (<connected-to-io_address>),          //             .address
		.io_bus_enable       (<connected-to-io_bus_enable>),       //             .bus_enable
		.io_byte_enable      (<connected-to-io_byte_enable>),      //             .byte_enable
		.io_rw               (<connected-to-io_rw>),               //             .rw
		.io_write_data       (<connected-to-io_write_data>),       //             .write_data
		.io_read_data        (<connected-to-io_read_data>),        //             .read_data
		.lcd_data_DATA       (<connected-to-lcd_data_DATA>),       //     lcd_data.DATA
		.lcd_data_ON         (<connected-to-lcd_data_ON>),         //             .ON
		.lcd_data_BLON       (<connected-to-lcd_data_BLON>),       //             .BLON
		.lcd_data_EN         (<connected-to-lcd_data_EN>),         //             .EN
		.lcd_data_RS         (<connected-to-lcd_data_RS>),         //             .RS
		.lcd_data_RW         (<connected-to-lcd_data_RW>),         //             .RW
		.leds_export         (<connected-to-leds_export>),         //         leds.export
		.push_buttons_export (<connected-to-push_buttons_export>), // push_buttons.export
		.reset_reset_n       (<connected-to-reset_reset_n>),       //        reset.reset_n
		.sdram_addr          (<connected-to-sdram_addr>),          //        sdram.addr
		.sdram_ba            (<connected-to-sdram_ba>),            //             .ba
		.sdram_cas_n         (<connected-to-sdram_cas_n>),         //             .cas_n
		.sdram_cke           (<connected-to-sdram_cke>),           //             .cke
		.sdram_cs_n          (<connected-to-sdram_cs_n>),          //             .cs_n
		.sdram_dq            (<connected-to-sdram_dq>),            //             .dq
		.sdram_dqm           (<connected-to-sdram_dqm>),           //             .dqm
		.sdram_ras_n         (<connected-to-sdram_ras_n>),         //             .ras_n
		.sdram_we_n          (<connected-to-sdram_we_n>),          //             .we_n
		.sdram_clk_clk       (<connected-to-sdram_clk_clk>),       //    sdram_clk.clk
		.switches_export     (<connected-to-switches_export>)      //     switches.export
	);

