#include "Checksum.h"

// https://de.wikipedia.org/wiki/FNV_(Informatik)

#include <unistd.h>

Checksum updateFnv1a( const void* data, LengthType len, Checksum hval )
{
  if( len > 0 )
  {
    auto ptr = static_cast<const unsigned char*>( data );
    while( len-- )
    {
      hval ^= *ptr++;
      hval *= magicChecksumPrime;
    }
  }
  return hval;
}
