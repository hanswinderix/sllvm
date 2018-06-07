MEMORY {
  sfr              : ORIGIN = 0x0000, LENGTH = 0x0010 /* END=0x0010, size 16 */
  peripheral_8bit  : ORIGIN = 0x0010, LENGTH = 0x00f0 /* END=0x0100, size 240 */
  peripheral_16bit : ORIGIN = 0x0100, LENGTH = 0x0100 /* END=0x0200, size 256 */

  ram (wx)         : ORIGIN = 0x0200, LENGTH = 16384 /* END=0x0300, size 768 */
  rom (rx)         : ORIGIN = 23552, LENGTH = 41984-32 /* END=0xffe0, size 4064 */

  /*sllvm (wx)       : ORIGIN = 0xfe00, LENGTH = 0x0100*/

  vectors          : ORIGIN = 0xffe0, LENGTH = 32 /* END=0x10000, size 32 as 16 2-byte segments */

  /* Remaining banks are absent */
  bsl              : ORIGIN = 0x0000, LENGTH = 0x0000
  infomem          : ORIGIN = 0x0000, LENGTH = 0x0000 /* END=0x1100, size 256 as 2 128-byte segments */
  infob            : ORIGIN = 0x0000, LENGTH = 0x0000 /* END=0x1080, size 128 */
  infoa            : ORIGIN = 0x0000, LENGTH = 0x0000 /* END=0x1100, size 128 */
  infoc            : ORIGIN = 0x0000, LENGTH = 0x0000
  infod            : ORIGIN = 0x0000, LENGTH = 0x0000
  ram2 (wx)        : ORIGIN = 0x0000, LENGTH = 0x0000
  ram_mirror (wx)  : ORIGIN = 0x0000, LENGTH = 0x0000
  usbram (wx)      : ORIGIN = 0x0000, LENGTH = 0x0000
  far_rom          : ORIGIN = 0x00000000, LENGTH = 0x00000000
}
REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_FAR_ROM", far_rom);
