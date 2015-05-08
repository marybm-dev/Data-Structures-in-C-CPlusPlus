//
//  ritefiles.cpp
//  updateFiles
//
//  Created by Mary on 5/24/14.
//  Copyright (c) 2014 Mary. All rights reserved.
//

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

int
main ( int argc, char **argv )
{
    void makefile( ifstream &, ofstream &);
    
    ifstream fi;
    ofstream fo;
    char ch;
    
    if ( argc != 3 ) {
        cout << "** use if >rightfiles textfile structfile **\n";
    }
    
    fi.open( *++argv, ios::in );
    if ( fi.fail() ) {
        cout << "** can't open " << *argv << "**\n";
        exit(1);
    }
    
    fo.open( *++argv, ios::in | ios::binary );
    if ( fo.fail() ){
        cout << "** \"" << *argv << "\" exists **  overwrite? ";
        cin.get(ch);
        
        if ( ch != '\n' ){
            while ( cin.get() != '\n' ){
                ;
            }
        }
        if ( (ch != 'y') && (ch != 'Y') ){
            cout << "ok: -- \"" << *argv << "\" is unchanged --\n";
            exit(0);
        } else {
            fo.close();
        }
    }
    
    fo.clear();
    fo.open( *argv, ios::out );
    if ( fo.fail() ) {
        cout << "** can't open \"" << *argv << "\" **\n";
        exit(1);
    }
    
    makefile( fi, fo );
    fi.close();
    fo.close();
    return(0);
}

void
makefile( ifstream & fi, ofstream & fo ){
    
    struct{
        int acct;
        char name[25];
        float dolr;
    } x;
    
    int i;
    char line[81], *p, *q;
    
    while ( fi.getline( line, 81 ) ) {
        x.acct = atoi( line );
        p = line;
        while ( *p++ != ' ' ){
            ;
        }
        for ( q = x.name, i = 24 ; i-- ; ){
            *q++ = *p++;                    // *q = '\0';
        }
        while ( *--q == ' ' ){              // find end of string
            ;
        }
        *++q = '\0';                        // mark end of name string
        x.dolr = atof(p);
        fo.write( (char*)&x, sizeof(x) );
    }
    
}

















