//	mkjobs.cpp	1999 oct 07	last modified 2010 nov 04
//
//	Makes jobs for bank program.
//	User is prompted for njobs, start, stop, mintim, maxtim, and filename.
//

#include <fstream>
#include <iostream>
#include <iomanip>
#include <new>
#include <stdlib.h>

using namespace std;

	struct {
		unsigned short enter;
		unsigned short tim;
	} job;

int
main()
{
	//int exist( const char * );
    void newerr();
	void sort( unsigned short *,unsigned short );

	ofstream file;
	unsigned short *enter,i,*jobtim,m,maxtim,mintim,njobs,start,stop;
	char s[81];

	cout << "\t\t    njobs: ";
	cin.getline( s,81 );
	njobs = atoi( s );
	cout << "\t\t    start: ";
	cin.getline( s,81 );
	start = atoi( s );
	cout << "\t\t     stop: ";
	cin.getline( s,81 );
	stop = atoi( s );
	cout << "\t\t   mintim: ";
	cin.getline( s,81 );
	mintim = atoi( s );
	cout << "\t\t   maxtim: ";
	cin.getline( s,81 );
	maxtim = atoi( s );
	if ( (njobs < 1) || (stop < start) || (maxtim < mintim) ) {
		cout << "** bad data **\n";
		exit( 1 );
	}
	cout << "(binary) output file name: ";
	cin.getline( s,81 );
	/*if ( exist( s ) ) {
		cout << "\n-- ok: \"" << s << "\" unchanged --\n";
		exit( 1 );
	}*/
	file.open( s,ios::out | ios::binary );
	if ( file.fail() ) {
		cout << "** can't open \"" << s << "\" for writing **\n";
		exit( 1 );
	}
	set_new_handler( newerr );
	enter = new unsigned short int [njobs+njobs];
	jobtim = enter+njobs;
	for ( m = stop-start+1, i = 0;  i < njobs;  i++ ) {
		enter[i] = start+((unsigned short)rand() % m);
	}
	sort( enter,njobs );
	for ( m = maxtim-mintim+1, i = 0;  i < njobs;  i++ ) {
		jobtim[i] = mintim+((unsigned short)rand() % m);
	}
	for ( i = 0;  i < njobs;  i++ ) {
		job.enter = enter[i];
		job.tim = jobtim[i];
		file.write( (char *)&job,sizeof( job ) );
		if ( file.fail() ) {
			cout << "** can't write record **\n";
			delete [] enter;
			exit( 1 );
		}
	}
	file.close();
	if ( file.fail() ) {
		cout << "** can't close \"" << s << "\" **\n";
		delete [] enter;
		exit( 1 );
	}
	delete [] enter;
	return( 0 );
}
void
newerr()
{
	cout << "** new has failed **\n";
	exit( 1 );
}
void
sort( unsigned short *a,unsigned short n )
{
	void qqsort( unsigned short *,short int,short int );

	qqsort( a,0,n-1 );
}
void
qqsort( unsigned short *a,short int l,short int r )
{
	unsigned short m,x;
	short int i,j;

	if ( l < r ) {
		m = a[l];
		i = l-1;
		j = r+1;
		do {
			while ( (i < r) && (a[++i] < m) ) {
				;
			}
			while ( m < a[--j] ) {
				;
			}
			if ( i < j ) {
				x = a[i];
				a[i] = a[j];
				a[j] = x;
			}
		} while ( i < j );
		if ( i == j ) {
			i++;
			--j;
		}
		qqsort( a,l,j );
		qqsort( a,i,r );
	}
}
