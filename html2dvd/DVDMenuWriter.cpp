//////////////////////////////////////////////////////////////////////
//
// DVDMenuWriter.cpp: implementation of the DVDMenuWriter class.
//
// (c) 2004 Jory Stone (jcsston@toughguy.net), All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////

#include "DVDMenuWriter.h"

#define DVD_SECTOR_SIZE 2048

DVDMenuWriter::DVDMenuWriter()
{
	m_FrameCount = 0;
	m_FrameCount = 10;
}

DVDMenuWriter::~DVDMenuWriter()
{

}

void DVDMenuWriter::Write()
{
  // DVD Sector size 2048
	off_t len;
	wxUint8 buffer[DVD_SECTOR_SIZE];
	
	wxFileOutputStream fVMGOut(m_DestFolder+wxT("VIDEO_TS.IFO"));
	wxDataOutputStream dVMGOut(fVMGOut);
	dVMGOut.BigEndianOrdered(true);

	fVMGOut.Write("DVDVIDEO-VMG", 12);
	
	// 0020	2	version number
	fVMGOut.SeekO(0x0020, wxFromStart);
	// Version Number
	// --------------------------------
	// | byte | 7 6 5 4   |   3 2 1 0 |
	// |  0   |       reserved = 0    |
	// |  1	  | Major     | Minor     |
	// --------------------------------
	// byte 0, reserved = 0
	buffer[0] = 0;
	// byte 1, version 1.1 == 00010001 == 0x11
	buffer[1] = 0x11;
	fVMGOut.Write(buffer, 2);


	// 0022	4	VMG category  
	fVMGOut.SeekO(0x0022, wxFromStart);
	// byte1 = prohibited region mask
	dVMGOut.Write32(0);

	// 0026	2	number of volumes	 
	fVMGOut.SeekO(0x0026, wxFromStart);
	dVMGOut.Write16(1);

	// 0028	2	volume number	 
	fVMGOut.SeekO(0x0028, wxFromStart);
	dVMGOut.Write16(1);

	// 002A	1	side ID	 
	fVMGOut.SeekO(0x002a, wxFromStart);
	dVMGOut.Write8(1);

	// 003E	2	number of title sets	 
	fVMGOut.SeekO(0x003E, wxFromStart);
	dVMGOut.Write16(1);

	// 0040	32	Provider ID
	fVMGOut.SeekO(0x0040, wxFromStart);
	fVMGOut.Write("DVDMENUWRITER-JCSSTON-001       ", 32);

	// 0060	8	VMG POS
	// This is 0 in my DVDLab created .ifo files
	fVMGOut.SeekO(0x0060, wxFromStart);
	dVMGOut.Write64(0);

	// 0080	4	end byte address of VMGI_MAT
	// <-- What is this?
	fVMGOut.SeekO(0x0080, wxFromStart);
	dVMGOut.Write32(0);

	
	len = _SeekToEndDVDSector(buffer, fVMGOut);
	_Write_FP_PGC(buffer, fVMGOut, dVMGOut);
	// 0084	4	start address of FP_PGC (First Play program chain)
	fVMGOut.SeekO(0x0084, wxFromStart);
	dVMGOut.Write32(len*DVD_SECTOR_SIZE);


	// 00C0	4	start sector of Menu VOB
	// <-- What is this?
	fVMGOut.SeekO(0x00C0, wxFromStart);
	dVMGOut.Write32(0);

  
  len = _SeekToEndDVDSector(buffer, fVMGOut);
	_Write_TT_SRPT(buffer, fVMGOut, dVMGOut);
	// 00C4	4	sector pointer to TT_SRPT (table of titles)
	fVMGOut.SeekO(0x00C4, wxFromStart);
	dVMGOut.Write32(len);	
	
	len = _SeekToEndDVDSector(buffer, fVMGOut);
	_Write_VMGM_PGCI_UT(buffer, fVMGOut, dVMGOut);
  // 00C8	4	sector pointer to VMGM_PGCI_UT (Menu Program Chain table)  
	fVMGOut.SeekO(0x00C8, wxFromStart);
	dVMGOut.Write32(len);

  // 00CC	4	sector pointer to VMG_PTL_MAIT (Parental Management masks)
  // This is not needed
	fVMGOut.SeekO(0x00CC, wxFromStart);
	dVMGOut.Write32(0);
	
  // 00D0	4	sector pointer to VMG_VTS_ATRT (copies of VTS audio/sub-picture attributes)
  // <-- What is this?
	fVMGOut.SeekO(0x00D0, wxFromStart);
	dVMGOut.Write32(0);

  // 00D4	4	sector pointer to VMG_TXTDT_MG (text data)
  // Don't need this
	fVMGOut.SeekO(0x00D4, wxFromStart);
	dVMGOut.Write32(0);
	
  // 00DC	4	sector pointer to VMGM_VOBU_ADMAP (menu VOBU address map)
  // <-- What is this?
	fVMGOut.SeekO(0x00DC, wxFromStart);
	dVMGOut.Write32(0);
	
  // 00D8	4	sector pointer to VMGM_C_ADT (menu cell address table)
  // <-- What is this?
	fVMGOut.SeekO(0x00D8, wxFromStart);
	dVMGOut.Write32(0);
	
	// 0100	2	video attributes of VMGM_VOBS
	fVMGOut.SeekO(0x0100, wxFromStart);	
	// Video Attributes
	// ------------------------------------------------------
	// | byte | bits | desc
	// |   0  |  7-6 | Coding mode, 0 = Mpeg-1, 1 = Mpeg-2
  // |      |  5-4 | Standard, 0 = NTSC, 1 = PAL
	// |      |  3-2 | Aspect, 0 = 4:3, 1 = not specified, 2 = reserved, 3 = 16:9,	1 & 2 should not be used
	// |      |    1 | Automatic Pan/Scan, 1 = disallowed
	// |      |    0 | Automatic Letterbox, 1 = disallowed
	// |   1  |    7 | CC for line 21 field 1 in GOP (NTSC only)
	// |      |    6 | CC for line 21 field 2 in GOP (NTSC only)     
	// |      |    5 |  
	// |      |    4 | Bit Rate, 0 = VBR, 1 = CBR
	// |      |  3-2 | Resolution NTSC (PAL),	0 = 720x480 (720x576), 1 = 704x480 (704x576), 2 = 352x480 (352x576), 3 = 352x240 (352x288) 
	// |      |    1 | Letterboxed, 1=top and bottom cropped
	// |      |    0 | Film, 0=camera, 1=film (PAL only)
	// ------------------------------------------------------
  // Byte 0
  // Coding mode: Mpeg-2
  // Standard: NTSC
  // Aspect: 4:3
  // Automatic Pan/Scan: disallowed
  // Automatic Letterbox: disallowed
  buffer[0] = 0x43;
  // Byte 1
  // Bit Rate: VBR
  // Resolution: 720x480
  // Letterboxed: FALSE
  // Film: camera
  buffer[1] = 0x00;
	fVMGOut.Write(buffer, 2);

  // 0102	2	number of audio streams in VMGM_VOBS
	fVMGOut.SeekO(0x0102, wxFromStart);
	dVMGOut.Write16(1);

  // 0104	8*8	audio attributes of VMGM_VOBS
	fVMGOut.SeekO(0x0104, wxFromStart);
	// Audio Attributes
	// ------------------------------------------------------
	// | byte | bits | desc
  // |  0   |  7-5 | Coding mode, 0 AC3, 1 ???, 2 Mpeg-1, 3 Mpeg-2ext, 4 LPCM, 5 ???, 6 DTS, 7 ???
  // |      |    4 | Multichannel extension present
  // |      |  3-2 | Language type, 0 = unspecified, 1 = per bytes 2&3
  // |      |  1-0 | Application mode, 0 = unspecified, 1 = karaoke, 2 = surround
  // |  1   |  7-6 | Quantization/DRC, 0 = 16bps, 1 = 20bps, 2 = 24bps, 3 = DRC (dynamic range control)
  // |      |  5-4 | Sample rate, 0 = 48Kbps
  // |      |    3 |
  // |      |  2-0 | channels-1
  // |  2   |  7-0 | 1st character of language code
  // |  3   |  7-0 | 2nd character of language code
  // |  4   |  7-0 | reserved for language code extension
  // |  5   |  7-0 | code extension, 0 = unspecified, 1 = normal, 2 = for visually impaired, 3 = director's comments, 4 = alternate director's comments - see SPRM #17
  // |  6   |  7-0 |
  // Application information = Karaoke mode
  // |  7   |    7 | 0
  // |      |  6-4 | Channel assignments
  // |      |      | 0 = 1+1 (not valid), 1 = 1/0 (not valid),
  // |      |      | 2 = 2/0 L,R , 3 = 3/0 L,M,R ,
  // |      |      | 4 = 2/1 L,R,V1 , 5 = 3/1 L,M,R,V1 ,
  // |      |      | 6 = 2/2 L,R,V1,V2 , 7 = 3/2 L,M,R,V1,V2
  // |      |  3-2 | Karaoke version
  // |      |    1 | MC intro present
  // |      |    0 | 0=solo, 1=duet
  // Application information = Surround mode
  // |  7   |  7-4 | reserved
  // |      |    3 | 1 = suitable for Dolby surround decoding	
  // |      |  2-0 | reserved
  // ------------------------------------------------------
  // Byte 0
  // Coding mode: AC3
  // Multichannel extension present: FALSE
  // Language type: unspecified
  // Application mode: unspecified
  buffer[0] = 0x00;
  // Byte 1
  // Quantization/DRC: DRC (dynamic range control)
  // Sample rate: 48khz
  // Channels: 2
  buffer[1] = 0x00;
  
	fVMGOut.Write(buffer, 8);
	
  // 0154	2	number of subpicture streams in VMGM_VOBS (0 or 1)
	fVMGOut.SeekO(0x0154, wxFromStart);
	dVMGOut.Write16(0);
	
  // 0156	6	subpicture attributes of VMGM_VOBS
	fVMGOut.SeekO(0x0154, wxFromStart);
	memset(buffer, 0, 6);
	fVMGOut.Write(buffer, 6);
	
	
	// Now we have to seek back to the begining and write the total sizes
	len = fVMGOut.TellO();
  
	// 000C	4	last sector of VMG set (last sector of BUP)
	// Total Size of VIDEO_TS.BUP + VIDEO_TS.IFO + VIDEO_TS.VOB
	fVMGOut.SeekO(0x000C, wxFromStart);
	dVMGOut.Write32(len);

	len = _SeekToEndDVDSector(buffer, fVMGOut);

	// 001C	4	last sector of IFO
	fVMGOut.SeekO(0x001C, wxFromStart);
	dVMGOut.Write32(len);
	
	wxFileOutputStream fVTSOut(m_DestFolder+wxT("VTS_0_0.IFO"));
	wxDataOutputStream dVTSOut(fVTSOut);
	dVTSOut.BigEndianOrdered(true);

	fVTSOut.Write("DVDVIDEO-VTS", 12);

}

void DVDMenuWriter::SetDestFolder(const wxString &_destFolder)
{
	wxFileName destFolder(_destFolder);
	destFolder.AppendDir(wxT("VIDEO_TS"));
	if (!destFolder.DirExists())
		destFolder.Mkdir();

	m_DestFolder = destFolder.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);
}

off_t DVDMenuWriter::_SeekToEndDVDSector(wxUint8 *buffer, wxOutputStream &fOut)
{
	fOut.SeekO(0, wxFromEnd);	
	off_t len = fOut.TellO();
	if (len % DVD_SECTOR_SIZE != 0) {
		// We need to pad the end of the .IFO to the next sector
		if (len > DVD_SECTOR_SIZE)
			len = (len/DVD_SECTOR_SIZE+1)*DVD_SECTOR_SIZE - len;
		else
			len = DVD_SECTOR_SIZE - len;

		memset(buffer, 0, len);		
		fOut.Write(buffer, len);		
	}

	// Calc the ending DVD sector
	len = fOut.TellO()/DVD_SECTOR_SIZE;

	return len;
}

void DVDMenuWriter::_Write_VMGM_PGC_Palette(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut)
{
	int Y = 0;
	int Cr = 0;
	int Cb = 0;
	// 16*4	palette (0, Y, Cr, Cb)
	for (int p = 0; p < 16; p++) {
		buffer[0] = 0x00; // 0
		buffer[1] = Y++;  // Y
		buffer[2] = Cr++; // Cr
		buffer[3] = Cb++; // Cb
		fVMGOut.Write(buffer, 4);
	}
}

void DVDMenuWriter::_Write_VMGM_PGC_CommandTable(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut)
{
	/*
		Offset	Label	size	Contents
		0000	 	2	number of pre commands
		0002	 	2	number of post commands
		0004	 	2	number of cell commands
		0006	 	2	end address relative to command table
		0008	 	pre*8	pre commands
		0008+pre*8	 	post*8	post commands
		0008+(pre+post)*8	 	cell*8	cell commands
		* The total number of pre+post+cell commands must be 128 or less
	*/
	
	// 2	number of pre commands
	dVMGOut.Write16(1);

	// 2	number of post commands
	dVMGOut.Write16(1);

	// 2	number of cell commands
	dVMGOut.Write16(0);

	// Pre Command(s)
	// JumpSS VTS1:Root Menu
	buffer[0] = 0x30;
	buffer[1] = 0x06;
	buffer[2] = 0x00;
	buffer[3] = 0x01;
	buffer[4] = 0x01;
	buffer[5] = 0x83;
	buffer[6] = 0x00;
	buffer[7] = 0x00;
	fVMGOut.Write(buffer, 8);

	// Post Command(s)
	// JumpSS VTS1:Root Menu
	buffer[0] = 0x30;
	buffer[1] = 0x06;
	buffer[2] = 0x00;
	buffer[3] = 0x01;
	buffer[4] = 0x01;
	buffer[5] = 0x83;
	buffer[6] = 0x00;
	buffer[7] = 0x00;
	fVMGOut.Write(buffer, 8);

}

void DVDMenuWriter::_Write_VMGM_PGC_ProgramMap(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut)
{
	// program map entry (one per program, pad to word boundary with zero)
	// 1	entry cell number (* Program and Cell numbers begin at 1)
	dVMGOut.Write8(1);
	// Pad with 0's
	dVMGOut.Write8(0);
}

void DVDMenuWriter::_Write_BCD_PlaybackTime(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut)
{
	// 4 playback time, BCD, hh:mm:ss:ff with bits 7&6 of frame (last) byte indicating frame rate
	//   11 = 30 fps, 10 = illegal, 01 = 25 fps, 00 = illegal
	wxUint8 hh, mm, ss, ff;
	wxUint32 val = m_FrameCount / 30;
	hh = val / 3600;
	mm = (val % 3600) / 60;
	ss = val % 60;
	ff = m_FrameCount % 30;
	buffer[0] = hh;
	buffer[1] = mm;
	buffer[2] = ss;
	buffer[3] = (ff & 0x3F) | 0xC0 /* 30fps */;
	fVMGOut.Write(buffer, 4);
}

void DVDMenuWriter::_Write_VMGM_PGC_CellPlayInfoTable(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut)
{	
	// cell playback information table entry (one per cell)

	// 4	cell category
	// STC discontinuity = 1
	buffer[0] = 0x02;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	fVMGOut.Write(buffer, 4);

	_Write_BCD_PlaybackTime(buffer, fVMGOut, dVMGOut);

	// 4	first ILVU (VOBU?) start sector
	dVMGOut.Write32(0);
	// 4	first ILVU end sector
	dVMGOut.Write32(0);
	// 4	last VOBU start sector
	dVMGOut.Write32(0);
	// 4	last VOBU end sector
	dVMGOut.Write32(0);
}

void DVDMenuWriter::_Write_VMGM_PGC_CellPosInfoTable(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut)
{
	// cell position information table entry (one per cell)
	// 2	VOB id
	dVMGOut.Write16(1);
	// 1	reserved
	dVMGOut.Write8(0);
	// 1	Cell id
	dVMGOut.Write8(1);
}

void DVDMenuWriter::_Write_VMGM_PGCI_UT(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut)
{	
	off_t len;
	// Write the VMGM_PGCI_UT which lists the VMGM_LU (Language Units)
	off_t start_VMGM_PGCI_UT = fVMGOut.TellO();
	// 2 number of Language Units
	dVMGOut.Write16(1);
	// 2 reserved
	dVMGOut.Write16(0);
	
	// 4 end address (last byte of last PGC in last LU) relative to VMGM_PGCI_UT	
	dVMGOut.Write32(0); // <-- Actual value written later
	
	// 2 ISO639 language code
	fVMGOut.Write("en", 2);
	// 1 reserved for language code extension
	dVMGOut.Write8(0);
	// 1 Menu existence flag 80 = title
	dVMGOut.Write8(0x80);
	// 4 offset to VMGM_LU, relative to VMGM_PGCI_UT
	dVMGOut.Write32(8*2);

	// Write the VMGM_LU which lists the PGC (Program Chains)
	off_t start_VMGM_LU = fVMGOut.TellO();
	// 2 number of Program Chains
	dVMGOut.Write16(1);
	// 2 reserved
	dVMGOut.Write16(0);
	// 4 end address (last byte of last PGC in this LU) relative to VMGM_LU
	dVMGOut.Write32(0); // <-- Actual value written later
	// 4 PGC category
	// ---------------------------------------------------
	// | byte	| bits | desc
	// |   0  |    7 | 1=entry PGC
	// |    	|  6-4 | reserved
	// |    	|  3-0 | menu type (entry only) 2 = title
	// |   1  |      |
	// |   2  |      | parental management mask
	// |   3  |      | parental management mask
	// ---------------------------------------------------
	buffer[0] = 0x82;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	fVMGOut.Write(buffer, 4);
	// 4 offset to VMGM_PGC, relative to VMGM_LU
	dVMGOut.Write32(8*2);

	// Write VMGM_PGC
	off_t start_VMGM_PGC = fVMGOut.TellO();
	// 2 Unknown
	dVMGOut.Write16(0);
	// 1 number of programs
	dVMGOut.Write8(1);
	// 1 number of cells
	dVMGOut.Write8(1);

	_Write_BCD_PlaybackTime(buffer, fVMGOut, dVMGOut);

	len = fVMGOut.TellO();

	// 4 prohibited user ops, none
	dVMGOut.Write32(0);

	// 8*2 Audio Stream Control
	memset(buffer, 0, 8*2);
	fVMGOut.Write(buffer, 8*2);

	// 32*4 Subpicture Stream Control
	memset(buffer, 0, 32*4);
	fVMGOut.Write(buffer, 32*4);

	// 2 next PGCN
	dVMGOut.Write16(0);

	// 2 previous PGCN
	dVMGOut.Write16(0);

	// 2 goup PGCN
	dVMGOut.Write16(0);

	// 1 PGC still time, 255=infinite
	dVMGOut.Write8(0);

	// 1 PG playback mode, 0=sequential, otherwise bit7 indicates random (0) or shuffle (1), and the program count is in bits6-0
	dVMGOut.Write8(0);

	len = fVMGOut.TellO();

	// Write the palette
	_Write_VMGM_PGC_Palette(buffer, fVMGOut, dVMGOut);

	// Preserve offset start
	len = fVMGOut.TellO();

	// Reserve space for the offsets
	memset(buffer, 0, 2*4);
	fVMGOut.Write(buffer, 2*4);

	off_t start_VMGM_PGC_CommandTable = fVMGOut.TellO();
	_Write_VMGM_PGC_CommandTable(buffer, fVMGOut, dVMGOut);	
	
	off_t start_VMGM_PGC_ProgramMap = fVMGOut.TellO();
	_Write_VMGM_PGC_ProgramMap(buffer, fVMGOut, dVMGOut);	
	
	off_t start_VMGM_PGC_CellPlayInfoTable = fVMGOut.TellO();
	_Write_VMGM_PGC_CellPlayInfoTable(buffer, fVMGOut, dVMGOut);	

	off_t start_VMGM_PGC_CellPosInfoTable = fVMGOut.TellO();
	_Write_VMGM_PGC_CellPosInfoTable(buffer, fVMGOut, dVMGOut);	

	fVMGOut.SeekO(len, wxFromStart);
	// 2	offset within PGC to commands
	dVMGOut.Write16(start_VMGM_PGC_CommandTable - start_VMGM_PGC);

	// 2	offset within PGC to program map
	dVMGOut.Write16(start_VMGM_PGC_ProgramMap - start_VMGM_PGC);

	// 2	offset within PGC to cell playback information table
	dVMGOut.Write16(start_VMGM_PGC_CellPlayInfoTable - start_VMGM_PGC);

	// 2	offset within PGC to cell position information table
	dVMGOut.Write16(start_VMGM_PGC_CellPosInfoTable - start_VMGM_PGC);

	// Get back to the end where we belong
	fVMGOut.SeekO(0, wxFromEnd);

	// TODO: I may need to add 1 byte to these end addresses, on m DVDLab created VIDEO_TS.IFO the offset were one byte larger
	off_t end_VMGM_LU = fVMGOut.TellO();
	fVMGOut.SeekO(start_VMGM_LU+4, wxFromStart);
	// Now we write the VMGM_LU end address (last byte of last PGC in this LU) relative to VMGM_LU
	dVMGOut.Write32(end_VMGM_LU - start_VMGM_LU);
	fVMGOut.SeekO(end_VMGM_LU, wxFromStart);


	off_t end_VMGM_PGCI_UT = fVMGOut.TellO();
	fVMGOut.SeekO(start_VMGM_PGCI_UT+4, wxFromStart);
	// Now we write the VMGM_PGCI_UT end address (last byte of last PGC in last LU) relative to VMGM_PGCI_UT	
	dVMGOut.Write32(end_VMGM_PGCI_UT - start_VMGM_PGCI_UT);
	fVMGOut.SeekO(end_VMGM_PGCI_UT, wxFromStart);
}

void DVDMenuWriter::_Write_FP_PGC_CommandTable(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut)
{
	/*
		Offset	Label	size	Contents
		0000	 	2	number of pre commands
		0002	 	2	number of post commands
		0004	 	2	number of cell commands
		0006	 	2	end address relative to command table
		0008	 	pre*8	pre commands
		0008+pre*8	 	post*8	post commands
		0008+(pre+post)*8	 	cell*8	cell commands
		* The total number of pre+post+cell commands must be 128 or less
	*/
	
	// 2	number of pre commands
	dVMGOut.Write16(2);

	// 2	number of post commands
	dVMGOut.Write16(0);

	// 2	number of cell commands
	dVMGOut.Write16(0);

	// Pre Command(s)
	// Mov GPRM11,1
	buffer[0] = 0x70;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x0B;
	buffer[4] = 0x00;
	buffer[5] = 0x01;
	buffer[6] = 0x00;
	buffer[7] = 0x00;
	fVMGOut.Write(buffer, 8);

	// JumpSS VTS1:Root Menu
	buffer[0] = 0x30;
	buffer[1] = 0x06;
	buffer[2] = 0x00;
	buffer[3] = 0x01;
	buffer[4] = 0x01;
	buffer[5] = 0x83;
	buffer[6] = 0x00;
	buffer[7] = 0x00;
	fVMGOut.Write(buffer, 8);

}

void DVDMenuWriter::_Write_FP_PGC(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut)
{	
	off_t len;

	// Write VMGM_PGC
	off_t start_FP_PGC = fVMGOut.TellO();
	// 2 Unknown
	dVMGOut.Write16(0);
	// 1 number of programs
	dVMGOut.Write8(0);
	// 1 number of cells
	dVMGOut.Write8(0);

	_Write_BCD_PlaybackTime(buffer, fVMGOut, dVMGOut);

	len = fVMGOut.TellO();

	// 4 prohibited user ops, none
	dVMGOut.Write32(0);

	// 8*2 Audio Stream Control
	memset(buffer, 0, 8*2);
	fVMGOut.Write(buffer, 8*2);

	// 32*4 Subpicture Stream Control
	memset(buffer, 0, 32*4);
	fVMGOut.Write(buffer, 32*4);

	// 2 next PGCN
	dVMGOut.Write16(0);

	// 2 previous PGCN
	dVMGOut.Write16(0);

	// 2 goup PGCN
	dVMGOut.Write16(0);

	// 1 PGC still time, 255=infinite
	dVMGOut.Write8(0);

	// 1 PG playback mode, 0=sequential, otherwise bit7 indicates random (0) or shuffle (1), and the program count is in bits6-0
	dVMGOut.Write8(0);

	len = fVMGOut.TellO();

	// Write a blank palette
	memset(buffer, 0, 16*4);
	fVMGOut.Write(buffer, 16*4);	

	// Preserve offset start
	len = fVMGOut.TellO();

	// Reserve space for the offsets
	memset(buffer, 0, 2*4);
	fVMGOut.Write(buffer, 2*4);

	off_t start_FP_PGC_CommandTable = fVMGOut.TellO();
	_Write_FP_PGC_CommandTable(buffer, fVMGOut, dVMGOut);	
	
	fVMGOut.SeekO(len, wxFromStart);
	// 2	offset within PGC to commands
	dVMGOut.Write16(start_FP_PGC_CommandTable - start_FP_PGC);

	// Get back to the end where we belong
	fVMGOut.SeekO(0, wxFromEnd);
}

void DVDMenuWriter::_Write_TT_SRPT(wxUint8 *buffer, wxOutputStream &fVMGOut, wxDataOutputStream &dVMGOut)
{	
	off_t len;

	// Write TT_SRPT
	off_t start_TT_SRPT = fVMGOut.TellO();

	// 2 number of titles
	dVMGOut.Write16(1);

	// 2 reserved
	dVMGOut.Write16(0);

	len = fVMGOut.TellO();
	// 2 end address (last byte of last entry)
	dVMGOut.Write32(0);  // <-- Actual value written later


	// Title Entry
	// ----------------------------------------
	// | byte | bits | desc
	// |    0 |      | title type - see below
	// |      |   7  | reserved
	// |      |   6  | 0=one_sequential_pgc 1=not one_sequential (random, shuffle, stills, loops, or more than one pgc) 	
	// |      |      | Jump/Link/Call commands, where these commands can appear
	// |      |      | 0000 None 0001 invalid 
	// |      |      | 0011 only in button 
	// |      |      | 0101 only in pre/post 
	// |      |      | 0111 in button and pre/post 
	// |      |      | 1001 only in cell 
	// |      |      | 1011 in cell and button 
	// |      |      | 1101 in cell and pre/post 
	// |      |      | 1111 in all places
	// |      |   5  | cell
	// |      |   4  | pre/post
	// |      |   3  | button 
	// |      |   2  | exist
	// |      |   1  | Uop1 - PTT play or search	
	// |      |   0  | Uop0 - Time play or search
	// |    1 |      | 1 number of angles
	// |  1-2 |      | 2 number of chapters (PTTs)
	// |  2-3 |      | 2 parental management mask
	// |    4 |      | 1 Video Title Set number, VTSN
	// |    5 |      | 1 title number within VTS, VTS_TTN
	// | 6-10 |      | 4 start sector for VTS, referenced to whole disk (video_ts.ifo starts at sector 00000000)
	// ----------------------------------------
	
	// We only have to contruct the title type
	buffer[0] = /* exist */ 0x04 & /* button */ 0x08 & /* pre/post */ 0x10 & /* cell */0x20;
	fVMGOut.Write(buffer, 1);

	// 1 number of angles
	dVMGOut.Write8(1);

	// 2 number of chapters (PTTs)
	dVMGOut.Write16(0);

	// 2 parental management mask
	dVMGOut.Write16(0);

	// 1 Video Title Set number, VTSN
	dVMGOut.Write8(1);

	// 1 title number within VTS, VTS_TTN
	dVMGOut.Write8(1);

	// 4 start sector for VTS, referenced to whole disk (video_ts.ifo starts at sector 00000000)
	dVMGOut.Write32(0);


	off_t end_TT_SRPT = fVMGOut.TellO();
	fVMGOut.SeekO(len, wxFromStart);
	// 2	end address (last byte of last entry)
	dVMGOut.Write16(end_TT_SRPT - start_TT_SRPT);

	// Get back to the end where we belong
	fVMGOut.SeekO(0, wxFromEnd);
}
