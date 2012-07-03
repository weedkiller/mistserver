/// \file ts_packet.h
/// Holds all headers for the TS Namespace.

#pragma once
#include <string>
#include <cmath>
#include <stdint.h> //for uint64_t
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

#include "dtsc.h"

/// Holds all TS processing related code.
namespace TS {
  /// Class for reading and writing TS Streams
  class Packet {
    public:
      Packet();
      ~Packet();
      bool FromString( std::string & Data );
      void PID( int NewPID );
      int PID();
      void ContinuityCounter( int NewContinuity );
      int ContinuityCounter();
      void Clear();
      void PCR( int64_t NewVal );
      int64_t PCR();
      void AdaptationField( int NewVal );
      int AdaptationField( );
      int AdaptationFieldLen( );
      void DefaultPAT();
      void DefaultPMT();
      int UnitStart( );
      void UnitStart( int NewVal );
      int RandomAccess( );
      void RandomAccess( int NewVal );
      int BytesFree();
      
      void Print();
      std::string ToString();
      void PESVideoLeadIn( int NewLen );
      void PESAudioLeadIn( int NewLen, uint64_t PTS = 0 );
      void FillFree( std::string & PackageData );
      void AddStuffing( int NumBytes );
      void FFMpegHeader( );
      
      int PESTimeStamp( );
      
      //For output to DTSC
      int ProgramMapPID( );
      void UpdateStreamPID( int & VideoPid, int & AudioPid );
      DTSC::DTMI toDTSC(DTSC::DTMI & metadata, std::string Type);
    private:
      int Free;
      char Buffer[188];///< The actual data
  };//TS::Packet class
  
  /// Constructs an audio header to be used on each audio frame.
  /// The length of this header will ALWAYS be 7 bytes, and has to be 
  /// prepended on each audio frame.
  /// \param FrameLen the length of the current audio frame.
  static inline std::string GetAudioHeader( int FrameLen ) {
    char StandardHeader[7] = {0xFF,0xF1,0x4C,0x80,0x00,0x1F,0xFC};
    FrameLen += 7;
    StandardHeader[3] = ( StandardHeader[3] & 0xFC ) + ( ( FrameLen & 0x00001800 ) >> 11 );
    StandardHeader[4] = ( ( FrameLen & 0x000007F8 ) >> 3 );
    StandardHeader[5] = ( StandardHeader[5] & 0x3F ) + ( ( FrameLen & 0x00000007 ) << 5 );
    return std::string(StandardHeader,7);
  }
  
  /// A standard Program Association Table, as generated by FFMPEG.
  /// Seems to be independent of the stream.
  static char PAT[188] = {
    0x47,0x40,0x00,0x10, 0x00,0x00,0xB0,0x0D, 0x00,0x01,0xC1,0x00, 0x00,0x00,0x01,0xF0,
    0x00,0x2A,0xB1,0x04, 0xB2,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF
  };
  
  /// A standard Program Mapping Table, as generated by FFMPEG.
  /// Contains both Audio and Video mappings, works also on video- or audio-only streams.
  static char PMT[188] = {
    0x47,0x50,0x00,0x10, 0x00,0x02,0xB0,0x1D, 0x00,0x01,0xC1,0x00, 0x00,0xE1,0x00,0xF0,
    0x00,0x1B,0xE1,0x00, 0xF0,0x00,0x0F,0xE1, 0x01,0xF0,0x06,0x0A, 0x04,0x65,0x6E,0x67,
    0x00,0x8D,0x82,0x9A, 0x07,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF
  };
  
  /// A standard Sevice Description Table, as generated by FFMPEG.
  /// Not used in our connector, provided for compatibility means
  static char SDT[188] = {
    0x47,0x40,0x11,0x10, 0x00,0x42,0xF0,0x25, 0x00,0x01,0xC1,0x00, 0x00,0x00,0x01,0xFF,
    0x00,0x01,0xFC,0x80, 0x14,0x48,0x12,0x01, 0x06,0x46,0x46,0x6D, 0x70,0x65,0x67,0x09,
    0x53,0x65,0x72,0x76, 0x69,0x63,0x65,0x30, 0x31,0xA7,0x79,0xA0, 0x03,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF
  };
  
  /// A standard Picture Parameter Set, as generated by FFMPEG.
  /// Seems to be stream-independent.
  static char PPS[24] = {
    0x00,0x00,0x00,0x01,
    0x27,0x4D,0x40,0x1F,
    0xA9,0x18,0x0A,0x00,
    0xB7,0x60,0x0D,0x40,
    0x40,0x40,0x4C,0x2B,
    0x5E,0xF7,0xC0,0x40
  };
  
  /// A standard Sequence Parameter Set, as generated by FFMPEG.
  /// Seems to be stream-independent.
  static char SPS[8] = {
    0x00,0x00,0x00,0x01,
    0x28,0xCE,0x09,0xC8
  };
  
  /// The full Bytesteam Nal-Header.
  static char NalHeader[4] = {
    0x00,0x00,0x00,0x01
  };
  
  /// The shortened Bytesteam Nal-Header.
  static char ShortNalHeader[3] = {
    0x00,0x00,0x01
  };
};//TS namespace
