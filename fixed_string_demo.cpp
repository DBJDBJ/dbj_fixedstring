
#include "fixed_string.h"

int main (void)
{
    
    auto fixie = dbj::fixed_string::make(0xFF) ;

    assert( ! fixie.empty() ) ;

    fixie[0] = '0';
    fixie.at(1) = '1';
    fixie.data()[2] = '2';

    // should assert
    // fixie[fixie.size()] = '!';

    return 42;
}