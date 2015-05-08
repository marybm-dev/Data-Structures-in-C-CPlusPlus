//
//  bank.cpp
//  bank
//
//  Created by Mary on 5/3/14.
//  Copyright (c) 2014 Mary. All rights reserved.
//

#include <fstream>
#include <iomanip>
#include <iostream>
#include <new>
#include <stdlib.h>
#include <string.h>

using namespace std;

struct cust {
    int cusn;       // customer number
    int entr;       // when entered
    int jtim;       // time needed by clerk to do customer's job
    cust *link;
} *custh;           // pointer to head node

struct evnt {
    int time;       // cust enters, or clerk done
    int what;       // 1 customer enters ; 2 clerk finishes job
    int whom;       // 1 customer ; 2 clerk
    int data;       // 1 customer # ; 2 clerk #
    evnt *link;
} *evnth;           // pointer to head node

struct idlq {
    int clrk;       // which clerk
    int time;       // when began idle
    idlq *link;
} *idlqh;           // pointer to head node

struct waitq {
    int custn;      // customer #
    int entr;       // when entered
    int jtim;       // time needed by clerk to do customer's job
    waitq *link;
} *waitqh, *waitqt; // pointer to head and tail nodes

int const infinity = 2147483647;    // 2^(31)-1
int *idle, maxwait, minwait, now, totwait;

int main(int argc, char **argv)
{
    void newerr();
    void readjobs(char *, int &);
    void simul(int, int, int, int);
    void stats(int, int);
    
    cust *p, *q;
    int i, m, m0, m1, n, v=0;       // m: # of clerks, n: # of customer
    
    if ((argc < 4) || (5 < argc)) {
        // [] optional ; nclerk0 min# of clerk ; nclerk1 max# of clerk ; file name of new file to create
        cout << "** use is >bank [-v] nclerk0 nclerk1 file **\n";
        exit(1);
    }
    set_new_handler(newerr);
    if ( strcmp( *++argv,"-v") == 0 ) {
        v++; // first string is "bank" so increment to see if in verbose mode (this is the next string), if 0 then in verbose mode.
    } else {
        --argv; // not in verbose so go back to the first argument
    }
    m0 = atoi(*++argv);         // min# of clerks; ascii to char
    m1 = atoi(*++argv);         // max# of clerks; ascii to char
    if ( (m0 < 1) || (m1 < m0) ) { // if no clerk or max < min then error
        cout << "** bad nclerk0 and/or nclerk1 **\n";
        exit(1);
    }
    idle = new int[m1];         // total of time for clerk prior to current break
    readjobs(*++argv, n);       // n is passed by reference
    cout.setf(ios::fixed, ios::floatfield);
    cout.setf(ios::showpoint);
    for (m = m0; m <= m1; m++) {
        cout << setw(3) << m << " clerk" << (m < 2 ? "" : "s") << " --\n";
        for (i = 0; i < 2; i++) {
            simul(i, v, n, m);
            cout << (i == 0 ? "FCFC:" : " SJF:") << " finish at " << now << '\n';
            stats(n, m);        // n customers ; m clerks
        }
        cout << '\n';
    }
    for (p = custh; p != (cust *)0; ) {
        q = p->link;
        delete p;
        p = q;
    }
    delete [] idle;
    return 0;
}

void
addevnt(evnt *pevnt) {
    evnt *p, *q;
    
    if ( evnth == (evnt *)0 ) { // if events are empty, make this the head
        evnth = pevnt;
        pevnt->link = (evnt *)0;
        
    } else {
        p = (evnt *)0;
        q = evnth;
        
        while (q != (evnt *)0) {
            
            if ( q->time <= pevnt->time){ // add to tail
                p = q;
                q = q->link;
                
            } else {
                pevnt->link = q;
                if (p == (evnt *)0) // add to head
                    evnth = pevnt;
                else
                    p->link = pevnt;
                
                break;
            }
        }
        
        if ( q == (evnt *)0 ) {
            p->link = pevnt;
            pevnt->link = (evnt *)0;
        }
    }
    
    
}

void getcust(int clrk) {        // make a new node and put it in the right place of the list
    void addevnt(evnt *);
    evnt *pevnt;
    waitq *q;
    int wait;
    
    pevnt = new evnt;
    pevnt->time = now + waitqh->jtim;
    pevnt->what = 2;
    pevnt->whom = clrk;
    pevnt->data = waitqh->custn;
    addevnt(pevnt);
    wait = now - waitqh->entr;
    
    if (wait < minwait)
        minwait = wait;
    
    totwait += (unsigned int)wait;
    
    if (maxwait < wait)
        maxwait = wait;
    
    q = waitqh;
    waitqh = waitqh->link;
    delete q;
    
    if (waitqh == (waitq *)0)
        waitqt = (waitq *)0;
}

void
init(int n, int m) {
    cust *pcust;
    evnt *pevnt, *q = nullptr;
    idlq *pidlq, *r = nullptr;
    int i;
    
    for (i = 0; i < m; ) {      // all clerks start with zero idle time
        idle[i] = 0;
        pidlq = new idlq;
        pidlq->clrk = ++i;
        pidlq->time = 0;
        if (i == 1)
            idlqh = pidlq;
        else
            r->link = pidlq;
        r = pidlq;
    }
    
    r->link = (idlq *)0;
    waitqh = waitqt = (waitq *)0;
    totwait = 0;
    now = maxwait = 0;
    minwait = infinity;
    
    pcust = custh;
    
    for (i = 0; i < n ; ) {     // for n customers, make list of event nodes and transfer data
        pevnt = new newerr() {
    cout << "** new has failed **\n";
    exit(1);
}

void
readjobs(char *fname, int & n) { // name of file is passed by reference, # of customers
    ifstream file;
    struct {
        unsigned short int entr;
        unsigned short int jtim;
    } rec;
    cust *pcust, *q = nullptr;            // pointer to cust node
    
    file.open(fname, ios::in | ios::binary);
    if ( file.fail() ) {
        cout << "** readjobs: can't open\"" << fname << "\" **\n";
        exit(1);
    } else {
        cout << "\t\t\tbank: data file is \"" << fname << "\"\n";
        n = 0;
    }
    
    while ( file.read( (char *)&rec, sizeof(rec) ) ) { // read job file into ram
        n++;
        pcust = new cust;
        pcust->entr = rec.entr;
        pcust->jtim = rec.jtim;
        
        if ( n == 1)
            custh = pcust;
        else
            q->link = pcust;
        q = pcust;
    }
    
    file.close();   // nothing left to read so close the file
    q->link = (cust *)0;
}

void
simul(int mode, int v, int n, int m) { // mode: 1 FCFS , 2 SJF
    void getcust(int), init(int, int), showevnt(), toclrk(int, int), toidlq(int), towaitq(int, int, int);
    evnt *q = new evnt;                    // pointer to event
    int data, what, whom;
    
    init(n, m);     // n customers, m clerk
    
    while ( evnth != (evnt *)0 ) { // while there is an event
        now = evnth->time;
        what = evnth->what;
        whom = evnth->whom;
        data = evnth->data;
        q = evnth;
        evnth = evnth->link;
        delete q;
        
        if ( v ) // in verbose mode?
            cout << setw(8) << now << ':';
        
        if ( what == 1 ) { // customer enters
            if ( v )
                cout << " customer" << setw(5) << whom << " enters; ";
            
            if ( idlqh == (idlq *)0 ) {
                if ( v )
                    cout << "to wait queue\n";
                towaitq(mode, whom, data);
            } else {
                if ( v )
                    cout << "to clerk" << setw(4) << idlqh->clrk << '\n';
                toclrk(whom, data);
            }
        } else { // clerk finishes with a customer
            if ( v )
                cout << " clerk" << setw(5) << whom << " is done with customer" << setw(5) << data << "; ";
            if ( waitqh == (waitq *)0 ) {
                if ( v )
                    cout << "to idle queue\n";
                toidlq(whom);
            } else {
                if ( v )
                    cout << "gets customer " << setw(5) << waitqh->custn << '\n';
                getcust(whom);
            }
        }
        
    }
}

void
stats(int n, int m) {
    idlq *p = new idlq;
    int i, maxidle = 0, minidle = 0, totidle;
    
    for ( ; idlqh != (idlq *)0 ; ) {
        idle[idlqh->clrk-1] += now - idlqh->time;
        p = idlqh;
        idlqh = idlqh->link;
        delete p;
    }
    
    for ( i = 0 ; i < m ; i++ ) {
        if ( idle[i] < minidle )
            minidle = idle[i];
        
        totidle += (unsigned int)idle[i];
        
        if ( maxidle < idle[i] )
            maxidle = idle[i];
    }
    
    cout << "minwait =" << setw(6) << minwait << " meanwait =" << setw(12) << setprecision(4) << (double)totwait/(double)n << " maxwait =" << setw(6) << maxwait << "\nminidle =" << setw(6) << minidle << " meanidle =" << setw(12) << setprecision(4) << (double)totidle/(double)m << " maxidle =" << setw(6) << maxidle << '\n';
}

void
toclrk(int cusn, int jtim) {
    void addevnt(evnt *);
    evnt *p;
    idlq *q;
    int clrk;
    
    clrk = idlqh->clrk;
    p = new evnt;
    p->time = now + jtim;
    p->what = 2;
    p->whom = clrk;
    p->data = cusn;
    idle[clrk-1] += now - idlqh->time;      // clerk's current break plus idle clerk's total breaks
    q = idlqh;
    idlqh = idlqh->link;
    delete q;
    addevnt(p);
    minwait = 0;
}

void
toidlq(int clrk) {
    idlq *p, *pidlq, *q;
    
    pidlq = new idlq;
    pidlq->clrk = clrk;
    pidlq->time = now;
    
    if ( idlqh == (idlq *)0 ) {
        pidlq->link = (idlq *)0;
        idlqh = pidlq;
    } else {
        //p = q = new idlq;
        p = (idlq *)0;
        q = idlqh;
        while ( q != (idlq *)0 ) {
            if ( idle[clrk-1] <= idle[q->clrk-1] + now - q->time ) {
                p = q;
                q = q->link;
            } else {
                if ( p == (idlq *)0 ) {
                    idlqh = pidlq;
                    idlqh->link = q;
                } else {
                    p->link = pidlq;
                    pidlq->link = q;
                }
                break;
            }
            
        }
        if ( q == (idlq *)0 ) {
            p->link = pidlq;
            pidlq->link = (idlq *)0;
        }
    }
    
}

void
towaitq(int mode, int cusn, int jtim) {
    waitq *p, *pwaitq, *q;
    
    pwaitq = new waitq;
    pwaitq->custn = cusn;
    pwaitq->entr = now;
    pwaitq->jtim = jtim;
    
    if ( waitqt == (waitq *)0 ) { // if wait is zero then this is the head and tail
        waitqh = waitqt = pwaitq;
        waitqt->link = (waitq *)0;
    } else {
        if ( mode == 0 ) { // FCFS - new guy goes to the end of queue
            waitqt->link = pwaitq;
            waitqt = pwaitq;
            waitqt->link = (waitq *)0;
        } else { // SJF
            p = (waitq *)0;
            q = waitqh;
            
            while ( q != (waitq *)0 ) {
                if ( q->jtim <= jtim ) {
                    p = q;
                    q = q->link;
                } else {
                    pwaitq->link = q;
                    
                    if ( p == (waitq *)0 )
                        waitqh = pwaitq;
                    else
                        p->link = pwaitq;
                    break;
                }
            }
            
            if ( q == (waitq *)0 ) {
                waitqt->link = pwaitq;
                pwaitq->link = (waitq *)0;
                waitqt = pwaitq;
            }
        }
    }
}


































