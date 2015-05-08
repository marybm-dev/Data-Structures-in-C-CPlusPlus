//	hash2.org	2013 may 28	last modified 2014 feb 28
//
//	Shows organization of instructor's "hash2.cpp".
//	Example of hashing.
//	Command line is >hash2 file disp size1 [size2 size3 ... sizen]
//	Here disp ("display mode") must be 0 or 1.
//	Then for tables of size from size1 to sizen a hash table of length
//	size is created; a count of the number of occurrences of each word
//	(string of alphas, all converted to lower case) in textfile is made.
//	Some results are shown; if disp is 1, then words are shown, along
//	with their counts; stats are shown for each size.
//

#include <ctype.h>			// isalpha()
#include <fstream>			// for processing files
#include <iomanip>			// setw()
#include <iostream>
#include <new>
#include <stdlib.h>			// exit(), etc.
#include <stdio.h>
#include <string.h>

using namespace std;

struct hasht {
    char *p;		// pointer into string space
    int k;			// count of words
} *table;

int const maxs=18;		// max string length
int const maxsp=1048576;	// string space size, 2**20
int const maxw=65000;		// max table size, no. diff. words
char * const nill=(char *)0;	// null pointer to char/string
char *ssp,*ssp1,*strng;		// string space, and pointers into it

int
main( int argc,char **argv )
{
	int count( ifstream &,int );
    void newerr();
	void display( int,int );
    
	ifstream f;
	int disp,first=1,i,size,y;
    int s = 0;
    int w = 0;
    
	if ( argc < 4 ) {
		cout << "** use is >hash file disp size1 [size2 ... ] **\n";
		exit( 1 );
	}
	f.open( *++argv,ios::in );
	if ( f.fail() ) {
		cout << "** can't open \"" << *argv << "\" **\n";
		exit( 1 );
	}
	disp = atoi( *++argv );
	if ( (disp != 0) && (disp != 1) ) {
		cout << "** illegal display option; must be 0 or 1 **\n";
		f.close();
		exit( 1 );
	}
	set_new_handler( newerr );
	strng = new char [maxsp];
	table = new hasht [maxw];
	cout.setf( ios::fixed,ios::floatfield );
	cout.setf( ios::showpoint );
	cout.precision( 4 );
	ssp1 = strng+maxsp;
	while ( *++argv ) {
		f.clear();
		f.seekg( 0 );
		size = atoi( *argv );
		if ( (size < 1) || (maxw < size) ) {
			cout << "size =" << setw( 7 ) << size
            << ", ** bad size **\n";
			continue;
		}
		ssp = strng;
		y = count( f,size );
		if ( y == -1 ) {
			cout << "** insufficient string space **\n";
			break;
		}
		if ( y == -2 ) {
			cout << "size =" << setw( 7 ) << size
            << ", ** too small: hash table overflow **\n";
			continue;
		}
		if ( first ) {
			--first;
			for ( s = w = i = 0;  i < size;  i++ ) {
				if ( table[i].k != 0 ) {
					w++;
					s += table[i].k;
				}
			}
			if ( w == 0 ) {
				cout << "** ??? - no words !?? **\n";
				break;
			}
			if ( disp ) {
				display( size,w );
			}
			cout << '\n' << s << " words, " << w << " different wo"
            "rds, " << ssp-strng << " bytes of string spa"
            "ce used\n\n";
		}
		cout << "size =" << setw( 5 ) << size << ", alpha ="
        << setw( 7 ) << (double)w/(double)size << ','
        << setw( 8 ) << y << " collisions,"
        << setw( 10 ) << (double)y/(double)s << "/word\n";
	}
	f.close();
	delete [] table;
	delete [] strng;
	return( 0 );
}
int
count( ifstream & fs,int size )
{
	int find( const char *,int,int );
    
	int c,f,i,l,y;
	char ch,*p,s[maxs+1];

    for ( y = 0, l = i = 0 ; i < size ; i++ ){
        table[i].k = 0;             // put zero in every k spot
        table[i].p = nill;          // put nill in every pptss spot
    }
    p = s;
    
    while ( fs.get(ch) ) {            // file source
        if ( isalpha(ch) ){
            if ( l < maxs ){
                l++;
                *p++ = (char)(ch | 0x20); // put this ch (lower case) into this string
            }
        }
        else {
            if ( l ){
                *p = '\0';
                if ( (f = find(s,size,l)) < 0 )
                    return (f);
                y += f;             // y = result of find + x
                p = s;              // put p back to beginning of string
                l = 0;
            }
        }
    }
    return (y);
}
void
display( int size,int w )
{
	void qsort( int,int,int *,int );
    
	hasht *p,*q;
	int i,i0,i1,*index1,*index2,*index3,k;
    
	for ( p = q = table;  size--;  q++ ) {
		if ( q->k != 0 ) {
			*p++ = *q;
		}
	}
	index1 = new int [w+w+w];
	if ( index1 == (int *)0 ) {
		cout << "** display: can't allocate space for index[] **\n";
		return;
	}
	index2 = index1+w;
	index3 = index2+w;
	for ( i = 0;  i < w;  i++ ) {
		index1[i] = index2[i] = index3[i] = i;
	}
	qsort( 0,w-1,index1,1 );
	qsort( 0,w-1,index2,2 );
	qsort( 0,w-1,index3,3 );
	i1 = -1;
	while ( i1 < w-1 ) {
		i0 = ++i1;
		k = table[index2[i0]].k;
		for ( i = i0+1;  i < w;  i++ ) {
			if ( table[index2[i]].k == k ) {
				i1++;
			} else {
				break;
			}
		}
		if ( i0 < i1 ) {
			qsort( i0,i1,index2,1 );
		}
	}
	i1 = -1;
	while ( i1 < w-1 ) {
		i0 = ++i1;
		k = strlen( table[index3[i0]].p );
		for ( i = i0+1;  i < w;  i++ ) {
			if ( strlen( table[index3[i]].p ) == k ) {
				i1++;
			} else {
				break;
			}
		}
		if ( i0 < i1 ) {
			qsort( i0,i1,index3,1 );
		}
	}
	for ( i = 0;  i < w;  i++ ) {
		cout << setw( 4 ) << i+1 << ".  ";
		cout.setf( ios::left,ios::adjustfield );
		cout << setw( maxs ) << table[index1[i]].p;
		cout.setf( ios::right,ios::adjustfield );
		cout << setw( 5 ) << table[index1[i]].k << " |  ";
		cout.setf( ios::left,ios::adjustfield );
		cout << setw( maxs ) << table[index2[i]].p;
		cout.setf( ios::right,ios::adjustfield );
		cout << setw( 5 ) << table[index2[i]].k << " |  ";
		cout.setf( ios::left,ios::adjustfield );
		cout << setw( maxs ) << table[index3[i]].p;
		cout.setf( ios::right,ios::adjustfield );
		cout << setw( 5 ) << table[index3[i]].k << '\n';
	}
	delete [] index1;
}
int
find( const char *s,int size,int len )
{
	int h( const char *,int );
	int c,i;
	char *p;
    
    c = 0;
    i = h(s, size);
    
    while (1) {
        p = table[i].p;     // p is pointer in table at location i
        if ( (p != nill) && (strcmp(s,p) == 0) )    // p is not nill AND same string
            break;
        else {
            if ( p == nill ) {
                if ( ssp1 <= ssp + len )
                    return (-1);
                else {
                    strcpy((table[i].p = ssp),s);   // copy string into that location
                    ssp += len + 1;                 // increment ssp to point to nxt spot
                    break;
                }
            }
            else {
                c++;
                if ( size <= c )
                    return (-2);
                else {
                    i++;
                    if ( size <= i )
                        i = 0;
                }
            }
        }
    }
    table[i].k++;
    return (c);
}
int
h( const char *s,int m )
{
	int y;
    for ( y = 1 ; *s ; )
        y = (y*(*s++)) % m;
    return (y);
}
void
newerr()
{
	cout << "** new has failed **\n";
	exit( 1 );
}
void
qsort( int l,int r,int *index,int mode )
{
	int i,j,x;
    int m2 = 0;
	char *m1 = nullptr;
    
	if ( l < r ) {
		if ( mode == 1 ) {
			m1 = table[index[l]].p;
		}
		if ( mode == 2 ) {
			m2 = table[index[l]].k;
		}
		if ( mode == 3 ) {
			m2 = strlen( table[index[l]].p );
		}
		i = l-1;
		j = r+1;
		do {
			if ( mode == 1 ) {
				while ( strcmp( table[index[++i]].p,m1 ) < 0 ){
					;
				}
				while ( strcmp( m1,table[index[--j]].p ) < 0 ){
					;
				}
			}
			if ( mode == 2 ) {
				while ( m2 < table[index[++i]].k ) {
					;
				}
				while ( table[index[--j]].k < m2 ) {
					;
				}
			}
			if ( mode == 3 ) {
				while ( strlen( table[index[++i]].p ) < m2 ) {
					;
				}
				while ( m2 < strlen( table[index[--j]].p ) ) {
					;
				}
			}
			if ( i < j ) {
				x = index[i];
				index[i] = index[j];
				index[j] = x;
			}
		} while ( i < j );
		if ( i == j ) {
			i++;
			--j;
		}
		qsort( l,j,index,mode );
		qsort( i,r,index,mode );
	}
}