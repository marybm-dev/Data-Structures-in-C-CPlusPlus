//	ritbigs.cpp	2001 jul 14	last modified 2008 jul 25
//
//	Makes binary data files for 3240as04.
//	Uses text files "first" and "last".
//	If user enters <n>, output files are named "mf<n>" and "tf<n+2>";
//		example: if <n>=1000, files are "mf1000" and "tf1002".
//	Here mf<n> is sorted by account number, and tf<n+2> is sorted by name.
//

#include <fstream>
#include <iostream>
#include <new>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace std;

	struct {
		int acct;
		char name[25];
		float dolr;
	} rec;
	float *tran;
	int const maxl=11;		// max name length (first or last)
	int const maxn=1200;		// max number of names (first or last)
	int const maxs=80;		// max string size
	struct names {
		char s[maxl+maxl+3];	// strlen( "last, first" ) <= maxl+maxl+2
	} *name;
	int *acct,*indx,n=0,nf=0,nl=0;
	char frst[maxn][maxl+1],last[maxn][maxl+1];

int
main()
{
	void newerr();
	void ckfile( const char * ),itoa( int,char * ),newacct( int & ),
		newrec( int,const char *,double,int ),sort( int );

	ifstream mfi;			// master (and first and last, temporarily)
	ofstream mfo,tfo;		// master, transaction
	time_t t;
	double tran0;
	int acct0,h,i,j,l,size;
	char mfn[maxs+1]="mf",s[maxs+1],tfn[maxs+1]="tf";

	set_new_handler(newerr);
	cout << "number of records in master file = ";
	cin.getline( s,maxs+1 );
	t = clock();
	size = atoi( s );
	if ( size < 3 ) {
		cout << "** too few records for master file **\n";
		exit( 1 );
	}
	acct = new int [size+2];
	name = new names [size+2];
	tran = new float [size+2];
	indx = new int [size+2];
	itoa( size,s );
	strcpy( mfn+2,s );
	itoa( size+2,s );
	strcpy( tfn+2,s );
	ckfile( mfn );
	ckfile( tfn );
	mfi.open( "first",ios::in );
	if ( mfi.fail() ) {
		cout << "** can't open \"first\" **\n";
		exit( 1 );
	}
	while ( mfi.getline( s,maxs+1 ) ) {
		if ( maxn <= nf ) {
			cout << "** too many first names **\n";
			exit( 1 );
		}
		strcpy( frst[nf++],s );
	}
	mfi.close();
	mfi.clear();
	mfi.open( "last",ios::in );
	if ( mfi.fail() ) {
		cout << "** can't open \"last\" **\n";
		exit( 1 );
	}
	while ( mfi.getline( s,maxs+1 ) ) {
		if ( maxn <= nl ) {
			cout << "** too many last names **\n";
			exit( 1 );
		}
		strcpy( last[nl++],s );
	}
	mfi.close();
	mfi.clear();
	mfo.open( mfn,ios::out | ios::binary );
	if ( mfo.fail() ) {
		cout << "** can't open \"" << mfn << "\" for output **\n";
		exit( 1 );
	}
	for ( acct0 = 0, h = 900000000/size, i = size;  i--; ) {
		j = rand() % nl;
		strcpy( s,last[j] );
		l = strlen( last[j] );
		strcpy( s+l,", " );
		strcpy( s+l+2,frst[rand() % nf] );
		acct0 += h;
		rec.acct = acct0+(rand() % (h-1));
		strcpy( rec.name,s );
		rec.dolr = (float)(100.0+9900.0/32767.0*(double)rand()+
			(double)(rand() % 100));
		mfo.write( (char *)&rec,sizeof( rec ) );
		if ( mfo.fail() ) {
			cout << "** can't write record to \"" << mfn << "\" **\n";
			exit( 1 );
		}
	}
	mfo.close();
	mfo.clear();
	if ( mfo.fail() ) {
		cout << "** can't close \"" << mfn << "\" **\n";
		exit( 1 );
	}
	cout << "-- wrote " << size << " records to \"" << mfn << "\" --\n";
	mfi.open( mfn,ios::in | ios::binary );
	if ( mfi.fail() ) {
		cout << "** can't open \"" << mfn << "\" for input **\n";
		exit( 1 );
	}
	newacct( acct0 );
	while (	mfi.read( (char *)&rec,sizeof( rec ) ) ) {
		acct0 = rec.acct;
		strcpy( s,rec.name );
		tran0 = (double)rec.dolr;
		i = rand() % 4;
		if ( i < 3 ) {
			newrec( acct0,s,tran0,i );
		} else {
			newacct( acct0 );
		}
	}
	mfi.close();
	newacct( acct0 );
	sort( n );
	tfo.open( tfn,ios::out | ios::binary );
	if ( tfo.fail() ) {
		cout << "** can't open \"" << tfn << "\" for writing **\n";
		exit( 1 );
	}
	for ( i = 0;  i < n;  i++ ) {
		j = indx[i];
		rec.acct = acct[j];
		strcpy( rec.name,name[j].s );
		rec.dolr = tran[j];
		tfo.write( (char *)&rec,sizeof( rec ) );
		if ( tfo.fail() ) {
			cout << "** can't write to \"" << tfn << "\" **\n";
			exit( 1 );
		}
	}
	tfo.close();
	if ( tfo.fail() ) {
		cout << "** can't close \"" << tfn << "\" **\n";
		exit( 1 );
	}
	cout << "-- wrote " << n << " records to \"" << tfn << "\" --\n";
	delete [] indx;
	delete [] tran;
	delete [] name;
	delete [] acct;
	cout << "-- job took " << (double)(clock()-t)/CLOCKS_PER_SEC
			<< " seconds --\n";
	return( 0 );
}
void
ckfile( const char *fn )
{
	ifstream f;
	char s[maxs+1];

	f.open( fn,ios::in );
	if ( !f.fail() ) {
		f.close();
		cout << "-- \"" << fn << "\" exists --  overwrite? (y/n) ";
		cin.getline( s,maxs+1 );
		if ( (*s != 'y') && (*s != 'Y') ) {
			cout << "-- ok, \"" << fn << "\" unchanged --\n";
			exit( 0 );
		}
	}
	f.clear();
}
void
itoa( int x,char *s )
{
	int l;
	char *p,t[maxs+1];

	l = 0;
	p = t;
	if ( x < 0 ) {
		*p++ = '-';
		l++;
		x = -x;
	}
	do {
		*p++ = (char)('0'+(x % 10));
		l++;
		x /= 10;
	} while ( 0 < x );
	for ( ;  l--; ) {
		*s++ = *--p;
	}
	*s = '\0';
}
void
newacct( int & acct0 )
{
	int j0,j1;
	char s[maxs+1];

//	rand();						// WHY THIS ?
	j0 = rand() % nl;
	j1 = rand() % nf;
	strcpy( s,last[j0] );
	strcpy( s+strlen( s ),", " );
	strcpy( s+strlen( s ),frst[j1] );
	acct[n] = ++acct0;
	strcpy( name[n].s,s );
	tran[n++] = (float)(100.0+990.0/32767.0*(double)rand());
}
void
newerr()
{
	cout << "** new failed **\n";
	exit( 1 );
}
void
newrec( int acct0,const char *s,double tran0,int mode )
{
	double x;

	if ( mode < 2 ) {
		x = 20.0+(double)(rand() % 10001)/100.0;
	}
	if ( mode == 1 ) {
		x = -x;
	}
	if ( (mode == 2 ) || ((x <= 0.0) && (tran0 < -x)) ) {
		x = -tran0;
	}
	acct[n] = acct0;
	strcpy( name[n].s,s );
	tran[n++] = (float)x;
}
void
q( int l,int r )
{
	int i,j,x;
	char m[maxs+1];

	if ( l < r ) {
		strcpy( m,name[indx[l]].s );
		i = l-1;
		j = r+1;
		do {
			while ( (i < r) && (strcmp( name[indx[++i]].s,m) ) < 0 ) {
				;
			}
			while ( strcmp( m,name[indx[--j]].s ) < 0 ) {
				;
			}
			if ( i < j ) {
				x = indx[i];
				indx[i] = indx[j];
				indx[j] = x;
			}
		} while ( i < j );
		if ( i == j ) {
			i++;
			--j;
		}
		q( l,j );
		q( i,r );
	}
}
void
sort( int n )
{
	void q( int,int );

	int i;

	for ( i = 0;  i < n;  i++ ) {
		indx[i] = i;
	}
	q( 0,n-1 );
}
