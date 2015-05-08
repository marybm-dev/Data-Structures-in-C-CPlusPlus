//
//  chkfile.cpp
//  updateFiles
//
//  Created by Mary on 5/24/14.
//  Copyright (c) 2014 Mary. All rights reserved.
//

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdlib.h>

using namespace std;

int
main( int argc, char ** argv )
{
    void chkfile( ifstream & );
    
    ifstream fi;
    
    if ( argc != 2 ){
        cout << "** use is >chkfile structfile **\n";
        exit(1);
    }
    
    fi.open ( *++argv, ios::in | ios::binary );
    if ( fi.fail() ) {
        cout << "** can't open \"" << *argv << "\" **\n";
        exit(1);
    }
    
    chkfile( fi );
    fi.close();
    return(0);
}

void
chkfile( ifstream & f ){
    
    struct {
        int acct;
        char name[25];
        float dolr;
    } x;
    
    
    while ( f.read( (char *)&x, sizeof(x) ) ){
        cout << setfill('0') << setw(9) << x.acct << "  ";
        cout.setf( ios::left, ios::adjustfield );
        cout << x.name << setfill(' ') << setw(26) << x.name;
        cout.setf( ios::right, ios::adjustfield );
        cout.setf( ios::fixed, ios::floatfield );
        cout.setf( ios::showpoint );
        cout.precision(2);
        cout << setw(10) << x.dolr << '\n';
    }
}








