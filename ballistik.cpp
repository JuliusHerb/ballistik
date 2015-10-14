/*
* ballistik. For description see https://github.com/jgherb/ballistik/blob/master/README.md
* @author: Julius Herb
* @version: 2015_10_14 (v0.1)
*/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <math.h> 
#include <cstring>
#include <sstream>
#include <algorithm>
#include <vector>
#include <complex>
using namespace std;
#define PI 3.14159265

int main(int argc, char* argv[])
{
    //Parameter START
    long iterations = 100000000; //Maximale Iterationen (Schutz vor Dauerschleife)
    double g = 9.81; //Erdbeschleunigung (in N/kg)
    double m = 0.08; //Masse des Objekts (in kg)
    double cw = 0.45;//Widerstandsbeiwert des Objects
    double rho = 1.29;//Dichte des Objekts (in kg/m3)
    double A = 0.0028;//Angriffsfläche des Objekts (in m2)
    double alpha = 45;//Abwurfwinkel (in Grad)
    double ta = 0.1; //Abwurfzeit, in der Fa wirkt (in s)
    double Fa = 20; //Abwurfkraft (in N)
    double sx0 = 0; //Startpunkt X-Koordinate (in m)
    double sy0 = 1.7+0.4; //Startpunkt Y-Koordinate (in m)
    double dt = 0.0001; //(Zeitintervall in s)
    bool luftwiderstand = true;
    bool detailoutput = true;
    bool shelloutput = false;
    bool fileoutput = true;
    string filepath = "output.csv";
    bool speedmode = true; //no storage of acceleration
    int outputmode = 0; //0=s;1=sx,sy;2=v;3=vx,vy;4=a;5=ax,ay
    //Parameter END
    double K = rho*cw*A/2;
    double vx0 = cos(alpha*PI/180)*ta*Fa/m;
    double vy0 = ta*(sin(alpha*PI/180)*Fa/m-g);
    double symax = 0;
    vector<double> t;
    vector<double> vx;
    vector<double> vy;
    vector<double> sx;
    vector<double> sy;
    vector<double> ax;
    vector<double> ay;
    //for(double vx_ = 0; vx_ < 100; vx_+=10) {
    //for(double vy_ = 0; vy_ < 100; vy_+=10) {
    t.clear();
    vx.clear();
    vy.clear();
    sx.clear();
    sy.clear();
    if(!speedmode)
    {
        ax.clear();
        ay.clear();
    }
    //vx0 = vx_;
    //vy0 = vy_;
    t.push_back(0);
    sx.push_back(sx0);
    sy.push_back(sy0);
    vx.push_back(vx0);
    vy.push_back(vy0);
    if(!speedmode)
    {
        ax.push_back(0);
        ay.push_back(0);
    }
    bool breakout = false;
    for(long i = 1; /*true*/i < iterations; i++)
    {
        double _lsx = sx[i-1];
        double _lsy = sy[i-1];
        double _lvx = vx[i-1];
        double _lvy = vy[i-1];
        double _ax = 0;
        double _ay = -g;
        if(luftwiderstand)
        {
            _ax += -cos(atan(_lvy/_lvx))*(_lvx*_lvx+_lvy*_lvy)*K/m;
            _ay += -sin(atan(_lvy/_lvx))*(_lvx*_lvx+_lvy*_lvy)*K/m;
        }
        double _vx = _lvx+_ax*dt;
        double _vy = _lvy+_ay*dt;
        double _sx = _lsx+_vx*dt;
        double _sy = _lsy+_vy*dt;
        if(_sy<0)
        {
            break;
        }
        if(_sx<0)
        {
            breakout = true;
            break;
        }
        if(_sx!=_sx)
        {
            breakout = true;
            break;
        }
        if(_sy!=_sy)
        {
            breakout = true;
            break;
        }
        if(_sy>symax)
        {
            symax = _sy;
        }
        t.push_back(i*dt);
        sx.push_back(_sx);
        sy.push_back(_sy);
        vx.push_back(_vx);
        vy.push_back(_vy);
        if(!speedmode)
        {
            ax.push_back(_ax);
            ay.push_back(_ay);
        }
    }
    if((sx[t.size()-1]>0)&&!breakout)
    {
        cout << "# vx0=" << vx0 << " ; vy0=" << vy0 << " ; alpha=" << alpha << "° ; xmax=" << sx[t.size()-1] << " ; ymax=" << symax << " ; dt=" << dt << " ; tmax=" << t[t.size()-1]<< "\n"; 
        if(detailoutput)
        {
            ofstream myfile;
            myfile.open (filepath.c_str());
            switch(outputmode)
                {
                    case 0:
                        if(shelloutput)
                        {
                            cout << "syntax: t;s" << "\n";
                        }
                        if(fileoutput)
                        {
                            myfile << "syntax: t;s" << "\n";
                        }
                        break;
                    case 1:
                        if(shelloutput)
                        {
                            cout << "syntax: t;sx;sy" << "\n";
                        }
                        if(fileoutput)
                        {
                            myfile << "syntax: t;sx;sy" << "\n";
                        }
                        break;
                    case 2:
                        if(shelloutput)
                        {
                            cout << "syntax: t;v" << "\n";
                        }
                        if(fileoutput)
                        {
                            myfile << "syntax: t;v" << "\n";
                        }
                        break;
                    case 3:
                        if(shelloutput)
                        {
                            cout << "syntax: t;vx;vy" << "\n";
                        }
                        if(fileoutput)
                        {
                            myfile << "syntax: t;vx;vy" << "\n";
                        }
                        break;
                    case 4:
                        if(shelloutput)
                        {
                            cout << "syntax: t;a" << "\n";
                        }
                        if(fileoutput)
                        {
                            myfile << "syntax: t;a" << "\n";
                        }
                        break;
                    case 5:
                        if(shelloutput)
                        {
                            cout << "syntax: t;ax;ay" << "\n";
                        }
                        if(fileoutput)
                        {
                            myfile << "syntax: t;ax;ay" << "\n";
                        }
                        break;
                    default:
                        break;
                }
            for(long i = 0; i < t.size(); i++)
            {
                switch(outputmode)
                {
                    case 0:
                        if(shelloutput)
                        {
                            double results = sqrt(sx[i]*sx[i]+sy[i]*sy[i]);
                            cout << t[i] << ";" << results << "\n";
                        }
                        if(fileoutput)
                        {
                            double results = sqrt(sx[i]*sx[i]+sy[i]*sy[i]);
                            myfile << t[i] << ";" << results << "\n";
                        }
                        break;
                    case 1:
                        if(shelloutput)
                        {
                            cout << t[i] << ";" << sx[i] << ";" << sy[i] << "\n";
                        }
                        if(fileoutput)
                        {
                            myfile << t[i] << ";" << sx[i] << ";" << sy[i] << "\n";
                        }
                        break;
                    case 2:
                        if(shelloutput)
                        {
                            double resultv = sqrt(vx[i]*vx[i]+vy[i]*vy[i]);
                            cout << t[i] << ";" << resultv << "\n";
                        }
                        if(fileoutput)
                        {
                            double resultv = sqrt(vx[i]*vx[i]+vy[i]*vy[i]);
                            myfile << t[i] << ";" << resultv << "\n";
                        }
                        break;
                    case 3:
                        if(shelloutput)
                        {
                            cout << t[i] << ";" << vx[i] << ";" << vy[i] << "\n";
                        }
                        if(fileoutput)
                        {
                            myfile << t[i] << ";" << vx[i] << ";" << vy[i] << "\n";
                        }
                        break;
                    case 4:
                        if(shelloutput)
                        {
                            double resulta = sqrt(ax[i]*ax[i]+ay[i]*ay[i]);
                            cout << t[i] << ";" << resulta << "\n";
                        }
                        if(fileoutput)
                        {
                            double resulta = sqrt(ax[i]*ax[i]+ay[i]*ay[i]);
                            myfile << t[i] << ";" << resulta << "\n";
                        }
                        break;
                    case 5:
                        if(shelloutput)
                        {
                            cout << t[i] << ";" << ax[i] << ";" << ay[i] << "\n";
                        }
                        if(fileoutput)
                        {
                            myfile << t[i] << ";" << ax[i] << ";" << ay[i] << "\n";
                        }
                        break;
                    default:
                        break;
                }
            myfile.close();
            }
        }
    }
//}
//}
    return 0;
}
